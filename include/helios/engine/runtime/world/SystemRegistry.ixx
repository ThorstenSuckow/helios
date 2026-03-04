/**
 * @file SystemRegistry.ixx
 * @brief Type-indexed registry for managing System instances within a game loop pass.
 */
module;

#include <cassert>
#include <memory>
#include <span>
#include <vector>


export module helios.engine.runtime.world.SystemRegistry;

import helios.engine.core.data;
import helios.engine.runtime.world.System;
import helios.engine.runtime.world.TypedSystem;

using namespace helios::engine::core::data;

export namespace helios::engine::runtime::world {

    /**
     * @brief Type-indexed registry for managing System instances within a game loop pass.
     *
     * @details SystemRegistry owns TypedSystem<T> wrappers internally and
     * provides O(1) access to the underlying concrete system via
     * SystemTypeId. Systems are registered once via `add<T>()` and then
     * iterated each frame through the `systems()` span.
     *
     * @see System
     * @see TypedSystem
     * @see SystemTypeId
     */
    class SystemRegistry {

        /**
         * @brief Owning storage for wrapped System instances, indexed by SystemTypeId.
         */
        std::vector<std::unique_ptr<System>> systems_;

        /**
         * @brief Raw pointers to the underlying T instances for O(1) typed access.
         */
        std::vector<void*> underlyingSystems_;

        /**
         * @brief Cached non-owning view for iteration; rebuilt when dirty.
         */
        std::vector<System*> systemsView_;


    public:

        /**
         * @brief Returns a span of all registered systems for iteration.
         *
         * @return Span of System pointers.
         */
        [[nodiscard]] std::span<System* const> systems() noexcept {
            return systemsView_;
        }

        /**
         * @brief Registers a new system of type T.
         *
         * @details Creates a TypedSystem<T> wrapper, stores it in the
         * registry indexed by SystemTypeId, and returns a reference to the
         * underlying T instance.
         *
         * @tparam T The concrete system type. Must satisfy HasUpdate.
         * @tparam Args Constructor argument types for T.
         *
         * @param args Arguments forwarded to the T constructor.
         *
         * @return Reference to the registered system instance.
         *
         * @pre No system of type T is already registered.
         */
        template<typename T, typename... Args>
        requires helios::engine::runtime::world::HasUpdate<T>
        T& add(Args&&... args) {
            assert(!hasSystem<T>() && "System already registered with GameLoopPhase");
            auto system_ptr = std::make_unique<helios::engine::runtime::world::TypedSystem<T>>(
                    std::forward<Args>(args)...
            );

            const auto idx = SystemTypeId::id<T>().value();

            if (systems_.size() <= idx) {
                systems_.resize(idx + 1);
            }
            if (underlyingSystems_.size() <= idx) {
                underlyingSystems_.resize(idx + 1);
            }

            T& underlying = system_ptr->underlying();

            underlyingSystems_[idx] = &underlying;
            /**
             * SystemsView returns systems in the order they were registered.
             * The game loop guarantees this: calling addSystem() on a pass preserves
             * deterministic execution by maintaining the insertion order.
             * Once removeSystem() is implemented, updating systemsView_ must be
             * reviewed.
             */
            systemsView_.push_back(system_ptr.get());
            systems_[idx] = std::move(system_ptr);


            return underlying;
        }

        /**
         * @brief Checks whether a system of type T is registered.
         *
         * @tparam T The system type to check.
         *
         * @return True if registered, false otherwise.
         */
        template<typename T>
        [[nodiscard]] bool hasSystem() const {
            return getSystem<T>() != nullptr;
        }

        /**
         * @brief Returns a pointer to the underlying system of type T.
         *
         * @tparam T The system type to retrieve.
         *
         * @return Pointer to the system, or nullptr if not registered.
         */
        template<typename T>
        [[nodiscard]] T* getSystem() const {

            const auto idx = SystemTypeId::id<T>().value();
            if (systems_.size() <= idx || underlyingSystems_.size() <= idx) {
                return nullptr;
            }

            return underlyingSystems_[idx] ? static_cast<T*>(underlyingSystems_[idx]) : nullptr;
        }

    };


}