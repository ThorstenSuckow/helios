/**
 * @file ErasedUnique.ixx
 * @brief Type-erased unique ownership wrapper.
 */
module;

#include <memory>

export module helios.core.memory.ErasedUnique;

export namespace helios::core::memory {

    /**
     * @brief Type-erased unique ownership wrapper.
     *
     * @details ErasedUnique provides `unique_ptr`-like ownership semantics
     * without requiring a common base class or virtual destructor. It stores
     * a `void*` and a stateless deleter function pointer, totaling 16 bytes.
     *
     * Used by ResourceRegistry to store heterogeneous resource types
     * (Managers, CommandBuffers) in a single owning collection without
     * vtable overhead for destruction.
     *
     * ## Move-Only Semantics
     *
     * ErasedUnique is move-only. Moving transfers ownership and nulls the
     * source. Copy construction and assignment are implicitly deleted.
     *
     * @see ResourceRegistry
     */
    struct ErasedUnique {

        /**
         * @brief Raw pointer to the owned object.
         */
        void* ptr = nullptr;

        /**
         * @brief Stateless deleter that knows the concrete type.
         */
        void (*destroy)(void*) noexcept = nullptr;

        /**
         * @brief Default constructor. Creates an empty (non-owning) instance.
         */
        ErasedUnique() = default;

        /**
         * @brief Constructs from a unique_ptr, taking ownership.
         *
         * @details Releases the unique_ptr and captures a typed deleter.
         *
         * @tparam T The owned type.
         *
         * @param p The unique_ptr to take ownership from.
         */
        template<class T>
        explicit ErasedUnique(std::unique_ptr<T> p) noexcept
            : ptr(p.release()),
        destroy([](void* q) noexcept { delete static_cast<T*>(q); })
        {}

        /**
         * @brief Move constructor. Transfers ownership.
         *
         * @param obj The source instance (nulled after move).
         */
        ErasedUnique(ErasedUnique&& obj) noexcept : ptr(obj.ptr), destroy(obj.destroy) {
            obj.ptr = nullptr;
            obj.destroy = nullptr;
        }

        /**
         * @brief Move assignment. Transfers ownership after releasing current.
         *
         * @param obj The source instance (nulled after move).
         *
         * @return Reference to this instance.
         */
        ErasedUnique& operator=(ErasedUnique&& obj) noexcept {
            if (this != &obj) {
                reset();
                ptr = obj.ptr; destroy = obj.destroy;
                obj.ptr = nullptr;
                obj.destroy = nullptr;
            }
            return *this;
        }

        /**
         * @brief Destructor. Destroys the owned object if present.
         */
        ~ErasedUnique() { reset(); }

        /**
         * @brief Destroys the owned object and resets to empty.
         */
        void reset() noexcept {
            if (ptr && destroy) {
                destroy(ptr);
            }
            ptr = nullptr;
            destroy = nullptr;
        }
    };


}