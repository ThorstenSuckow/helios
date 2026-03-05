/**
 * @file ConceptModelRegistry.ixx
 * @brief Generic type-indexed registry for type-erased wrappers.
 */
module;

#include <cassert>
#include <memory>
#include <span>
#include <vector>


export module helios.engine.core.data.ConceptModelRegistry;
;
import helios.engine.common.concepts;

using namespace helios::engine::common::concepts;

export namespace helios::engine::core::data {

    /**
     * @brief Generic type-indexed registry for type-erased wrapper instances.
     *
     * @details ConceptModelRegistry stores instances of a type-erased wrapper
     * `AnyT` (e.g. System, Manager) and indexes them by a compile-time
     * `IdProvider` (e.g. SystemTypeId, ResourceTypeId). This provides:
     *
     * - **O(1) lookup** by concrete type via `item<T>()`
     * - **Insertion-order iteration** via `items()` for deterministic processing
     * - **Type-safe registration** via `add<T>(args...)` returning a reference
     *   to the underlying concrete instance
     *
     * The registry is used as the backend for `SystemRegistry` and
     * `ManagerRegistry`.
     *
     * ## Instantiations
     *
     * | Alias | AnyT | IdProvider |
     * |-------|------|------------|
     * | `SystemRegistry` | `System` | `SystemTypeId` |
     * | `ManagerRegistry` | `Manager` | `ResourceTypeId` |
     *
     * @tparam AnyT The type-erased wrapper type (must support `underlying()`).
     * @tparam IdProvider Compile-time ID generator (must provide `id<T>().value()`).
     *
     * @see SystemRegistry
     * @see ManagerRegistry
     *
     * @todo this impl needs to make sure that memory allocations / moves do not invalidate
     * the fastAccess in ResourceRegistry
     */
    template<typename AnyT, typename IdProvider>
    class ConceptModelRegistry {

        /**
         * @brief Owning storage for wrapped AnyT instances, indexed by type ID.
         */
        mutable std::vector<AnyT> items_;

        /**
         * @brief Cached raw pointers to the underlying concrete instances.
         */
        std::vector<void*> underlyingAnyT_;

        /**
         * @brief Records insertion order for deterministic iteration.
         */
        std::vector<size_t> insertionOrder_;

        /**
         * @brief Lazily-built view of AnyT pointers in insertion order.
         */
        mutable std::vector<AnyT*> itemView_;

        /**
         * @brief Dirty flag indicating itemView_ needs rebuilding.
         */
        mutable bool needsUpdate_ = false;

        /**
         * @brief Rebuilds the insertion-ordered view if dirty.
         */
        void update() const {

            if (!needsUpdate_) {
                return;
            }

            itemView_.clear();
            itemView_.reserve(insertionOrder_.size());

            for (const auto insertionIndex : insertionOrder_) {
                itemView_.push_back(const_cast<AnyT*>(&items_[insertionIndex]));
            }

            needsUpdate_ = false;

        }


    public:

        /**
         * @brief Returns a read-only span of registered items in insertion order.
         *
         * @return Span of const AnyT pointers.
         */
        [[nodiscard]] std::span<AnyT* const> items() const noexcept {
            update();
            return itemView_;
        }

        /**
         * @brief Returns a mutable span of registered items in insertion order.
         *
         * @return Span of AnyT pointers.
         */
        [[nodiscard]] std::span<AnyT*> items() noexcept {
            update();
            return itemView_;
        }


        /**
         * @brief Registers and wraps a concrete instance of type T.
         *
         * @details Constructs T in-place from the forwarded arguments, wraps it
         * in an AnyT, and stores it at the index determined by `IdProvider::id<T>()`.
         * Returns a reference to the underlying concrete T instance.
         *
         * @tparam T The concrete type to register.
         * @tparam Args Constructor argument types for T.
         *
         * @param args Arguments forwarded to the T constructor.
         *
         * @return Reference to the registered T instance.
         *
         * @pre T must not already be registered.
         */
        template<typename T, typename... Args>
        T& add(Args&&... args) {

            assert(!has<T>() && "AnyT already registered with GameLoopPhase");

            AnyT wrapper{T{std::forward<Args>(args)...}};

            const auto idx = IdProvider::template id<T>().value();

            if (items_.size() <= idx) {
                items_.resize(idx + 1);
            }
            if (underlyingAnyT_.size() <= idx) {
                underlyingAnyT_.resize(idx + 1);
            }

            items_[idx] = std::move(wrapper);
            void* rawUnderlying = items_[idx].underlying();
            underlyingAnyT_[idx] = rawUnderlying;


            insertionOrder_.push_back(idx);

            needsUpdate_ = true;
            return *static_cast<T*>(rawUnderlying);
        }


        /**
         * @brief Checks whether a concrete type T is registered.
         *
         * @tparam T The type to check.
         *
         * @return True if T has been added.
         */
        template<typename T>
        [[nodiscard]] bool has() const {
            return item<T>() != nullptr;
        }

        /**
         * @brief Returns a pointer to the registered instance of type T.
         *
         * @tparam T The concrete type to look up.
         *
         * @return Pointer to T, or nullptr if not registered.
         */
        template<typename T>
        [[nodiscard]] T* item() const {

            const auto idx = IdProvider::template id<T>().value();
            if (items_.size() <= idx || underlyingAnyT_.size() <= idx) {
                return nullptr;
            }

            return underlyingAnyT_[idx] ? static_cast<T*>(underlyingAnyT_[idx]) : nullptr;
        }

    };


}

