/**
 * @file ResourceRegistry.ixx
 * @brief Type-indexed registry for engine resources with O(1) lookup.
 */
module;

#include <cassert>
#include <memory>
#include <span>
#include <vector>

export module helios.engine.runtime.world.ResourceRegistry;

import helios.engine.runtime.world.Manager;

import helios.core.memory.ErasedUnique;

import helios.engine.runtime.messaging.command.TypedCommandHandler;
import helios.engine.runtime.messaging.command.CommandHandler;
import helios.engine.runtime.messaging.command.CommandBuffer;

import helios.engine.runtime.world.ManagerRegistry;

import helios.engine.common.concepts;

import helios.engine.runtime.world.types.ResourceTypeId;

using namespace helios::engine::runtime::messaging::command;
using namespace helios::engine::runtime::world::types;
using namespace helios::core::memory;
using namespace helios::engine::common::concepts;

export namespace helios::engine::runtime::world {


    /**
     * @brief Type-indexed registry for engine resources with O(1) lookup.
     *
     * @details ResourceRegistry provides a central store for Managers,
     * CommandBuffers, and CommandHandlers. Each resource type receives a
     * unique compile-time index via ResourceTypeId, enabling O(1) access
     * through a flat `void*` array (`fastAccess_`).
     *
     * ## Ownership Model
     *
     * - **Managers and CommandBuffers** are heap-allocated and owned via
     *   `ErasedUnique` (type-erased unique ownership, 16 bytes per entry).
     * - **CommandHandlers** are registered by reference (non-owning). They
     *   typically live as part of a Manager that is already owned.
     *
     * ## Dual-Access Pattern
     *
     * Resources are stored in two parallel structures:
     * - `fastAccess_[idx]` — O(1) type-based lookup (hot path)
     * - `managers_` / `commandBuffers_` — linear iteration for batch operations
     *   (e.g., `GameWorld::flushManagers()`, `GameWorld::init()`)
     *
     * @see ResourceTypeId
     * @see ErasedUnique
     * @see Manager
     * @see CommandBuffer
     * @see TypedCommandHandler
     */
   class ResourceRegistry {

        /**
         * @brief Owning storage for all heap-allocated resources.
         */
        std::vector<ErasedUnique> owned_;

        /**
         * @brief Type-indexed registry for Manager instances.
         *
         * @details Delegates to ConceptModelRegistry for type-erased storage,
         * O(1) lookup, and insertion-order iteration.
         */
        ManagerRegistry managerRegistry_;

        /**
         * @brief Non-owning pointers to registered CommandBuffers for iteration.
         */
        std::vector<CommandBuffer*> commandBuffers_;

        /**
         * @brief Non-owning pointers to registered CommandHandlers.
         */
        std::vector<CommandHandler*> commandHandlers_;

        /**
         * @brief Flat array for O(1) type-indexed resource lookup.
         *
         * @details Indexed by `ResourceTypeId::id<T>().value()`. Each slot
         * holds a raw `void*` to the resource instance.
         */
        std::vector<void*> fastAccess_;


public:

    /**
     * @brief Registers an owning Manager resource.
     *
     * @details Constructs T in the ManagerRegistry (which wraps it in a
     * type-erased Manager) and stores a raw pointer in `fastAccess_` for
     * O(1) lookup. The ManagerRegistry owns the instance and provides
     * insertion-order iteration for batch init/flush/reset.
     *
     * @tparam T The Manager type. Must satisfy `IsManager<T>`.
     * @tparam Args Constructor argument types for T.
     *
     * @param args Arguments forwarded to the T constructor.
     *
     * @return Reference to the registered T instance.
     *
     * @pre No resource of type T is already registered.
     */
    template<class T, class... Args>
    requires IsManager<T>
    T& registerResource(Args&&... args) {

        auto& managerRef = managerRegistry_.template add<T>(std::forward<Args>(args)...);

        const size_t idx = ResourceTypeId::id<T>().value();

        if (idx >= fastAccess_.size()) {
            fastAccess_.resize(idx + 1, nullptr);
        }

        assert(fastAccess_[idx] == nullptr && "Resource already registered");
        fastAccess_[idx] = &managerRef;

        return managerRef;
    }


    /**
     * @brief Registers an owning CommandBuffer or plain resource.
     *
     * @details Heap-allocates T via `std::make_unique`, stores a raw pointer
     * in `fastAccess_` for O(1) lookup, and transfers ownership to `owned_`.
     * If T derives from CommandBuffer, the pointer is also appended to
     * `commandBuffers_` for batch flushing.
     *
     * @tparam T The resource type. Must not satisfy `IsManager`.
     * @tparam Args Constructor argument types for T.
     *
     * @param args Arguments forwarded to the T constructor.
     *
     * @return Reference to the registered T instance.
     *
     * @pre No resource of type T is already registered.
     */
    template<class T, class... Args>
    T& registerResource(Args&&... args) {

        auto uniquePtr = std::make_unique<T>(std::forward<Args>(args)...);
        T* raw = uniquePtr.get();

        const size_t idx = ResourceTypeId::id<T>().value();

        if (idx >= fastAccess_.size()) {
            fastAccess_.resize(idx + 1, nullptr);
        }

        assert(fastAccess_[idx] == nullptr && "Resource already registered");

        fastAccess_[idx] = raw;

        if constexpr (std::derived_from<T, CommandBuffer>) {
            commandBuffers_.push_back(static_cast<CommandBuffer*>(raw));
        }

        owned_.emplace_back(ErasedUnique{std::move(uniquePtr)});

        return *raw;
    }

    /**
     * @brief Registers a non-owning CommandHandler reference.
     *
     * @details Stores a pointer to an externally owned CommandHandler in
     * `fastAccess_` and `commandHandlers_`. The handler must outlive this
     * registry (typically guaranteed because the handler is a Manager that
     * is already owned by this registry).
     *
     * @tparam T The CommandHandler type. Must derive from CommandHandler.
     *
     * @param ref Reference to the handler to register.
     *
     * @return Reference to the registered handler.
     *
     * @pre No resource of type T is already registered.
     */
    template<class T>
    requires IsCommandHandler<T>
    T& registerResource(T& ref) {

        const size_t idx = ResourceTypeId::id<T>().value();

        if (idx >= fastAccess_.size()) {
            fastAccess_.resize(idx + 1, nullptr);
        }

        assert(fastAccess_[idx] == nullptr && "Resource already registered");

        fastAccess_[idx] = &ref;


        if constexpr (std::derived_from<T, CommandHandler>) {

            if (idx >= commandHandlers_.size()) {
                commandHandlers_.resize(idx + 1, nullptr);
            }

            commandHandlers_[idx] = static_cast<CommandHandler*>(&ref);
        }

        return ref;
    }

    /**
     * @brief Checks whether a resource of type T is registered.
     *
     * @tparam T The resource type to check.
     *
     * @return True if registered, false otherwise.
     */
    template<class T>
    bool has() const noexcept {
        const size_t idx = ResourceTypeId::id<T>().value();
        return idx < fastAccess_.size() && fastAccess_[idx] != nullptr;
    }

    /**
     * @brief Returns a reference to a registered resource.
     *
     * @tparam T The resource type.
     *
     * @return Reference to the resource.
     *
     * @pre A resource of type T must be registered.
     */
    template<class T>
    T& resource() noexcept {
        const size_t idx = ResourceTypeId::id<T>().value();
        assert(idx < fastAccess_.size() && fastAccess_[idx] != nullptr);
        return *static_cast<T*>(fastAccess_[idx]);
    }

    /**
     * @copydoc resource()
     */
    template<class T>
    const T& resource() const noexcept {
        const size_t idx = ResourceTypeId::id<T>().value();
        assert(idx < fastAccess_.size() && fastAccess_[idx] != nullptr);
        return *static_cast<const T*>(fastAccess_[idx]);
    }

    /**
     * @brief Returns a pointer to a registered resource, or nullptr.
     *
     * @tparam T The resource type.
     *
     * @return Pointer to the resource, or nullptr if not registered.
     */
    template<class T>
    T* tryResource() noexcept {
        const size_t idx = ResourceTypeId::id<T>().value();
        if (idx < fastAccess_.size() && fastAccess_[idx] != nullptr) {
            return static_cast<T*>(fastAccess_[idx]);
        }
        return nullptr;
    }

    /**
     * @copydoc tryResource()
     */
    template<class T>
    const T* tryResource() const noexcept {
        const size_t idx = ResourceTypeId::id<T>().value();
        if (idx < fastAccess_.size() && fastAccess_[idx] != nullptr) {
            return static_cast<const T*>(fastAccess_[idx]);
        }
        return nullptr;
    }


    /**
     * @brief Returns a read-only span of registered Managers.
     *
     * @return Span of Manager pointers.
     */
    [[nodiscard]] std::span<Manager* const> managers() const noexcept {
        return managerRegistry_.items();
    }

    /**
     * @brief Returns a mutable span of registered Managers.
     *
     * @return Span of Manager pointers.
     */
    std::span<Manager*> managers() noexcept {
        return managerRegistry_.items();
    }

    /**
     * @brief Returns a read-only span of registered CommandBuffers.
     *
     * @return Span of CommandBuffer pointers.
     */
    std::span<CommandBuffer* const> commandBuffers() const noexcept {
        return commandBuffers_;
    }

    /**
     * @brief Returns a mutable span of registered CommandBuffers.
     *
     * @return Span of CommandBuffer pointers.
     */
    std::span<CommandBuffer*> commandBuffers() noexcept {
        return commandBuffers_;
    }
};
}

