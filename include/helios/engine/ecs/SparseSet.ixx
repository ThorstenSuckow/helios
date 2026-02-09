/**
 * @file SparseSet.ixx
 * @brief Generic sparse set data structure for efficient entity-component storage.
 */
module;

#include <cassert>
#include <functional>
#include <vector>

export module helios.engine.ecs.SparseSet;

import helios.engine.core.data;
import helios.engine.ecs.Traits;

export namespace helios::engine::ecs {

    /**
     * @brief Abstract base class for type-erased sparse set access.
     *
     * `SparseSetBase` provides a non-templated interface for polymorphic
     * operations on sparse sets. This enables containers like `EntityManager`
     * to store heterogeneous component pools and perform common operations
     * (e.g., removal) without knowing the concrete component type.
     *
     * @see SparseSet
     * @see EntityManager
     */
    class SparseSetBase {

        public:

        /**
         * @brief Virtual destructor for proper cleanup of derived classes.
         */
        virtual ~SparseSetBase() = default;

        /**
         * @brief Removes the element at the given index.
         *
         * @param id The EntityId of the element to remove.
         *
         * @return `true` if the element was removed, `false` if not found or
         *         removal was cancelled.
         */
        virtual bool remove(helios::engine::core::data::EntityId id) = 0;

        /**
         * @brief Checks whether an element exists for the specified EntityId.
         *
         * @param id The EntityId to test.
         *
         * @return `true` if the set contains the EntityId, `false` otherwise.
         */
        [[nodiscard]] virtual bool contains(helios::engine::core::data::EntityId id) const = 0;

        /**
         * @brief Returns a raw void pointer to the element at the given index.
         *
         * @param id The EntityId to look up.
         *
         * @return Raw pointer to the element, or `nullptr` if not found.
         */
        [[nodiscard]] virtual void* raw(helios::engine::core::data::EntityId id) = 0;
    };


    /**
     * @brief Sentinel value indicating an empty slot in the sparse array.
     *
     * Aliased from `helios::engine::core::data::EntityTombstone`.
     */
    constexpr auto Tombstone = helios::engine::core::data::EntityTombstone;

    using namespace helios::engine::core::data;

    /**
     * @brief A generic sparse set providing O(1) insertion, lookup, and removal.
     *
     * `SparseSet` implements the sparse set data structure pattern, commonly used
     * in Entity Component Systems (ECS) for efficient component storage. It maps
     * `EntityId` indices to densely packed data of type `T`.
     *
     * ## Data Structure
     *
     * ```
     * ┌─────────────────────────────────────────────────────────────┐
     * │                    SPARSE SET LAYOUT                        │
     * ├─────────────────────────────────────────────────────────────┤
     * │                                                             │
     * │  SPARSE ARRAY (indexed by EntityId)                         │
     * │  ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┐                │
     * │  │  2  │  ×  │  0  │  ×  │  1  │  ×  │  ×  │  (dense idx)   │
     * │  └──┬──┴─────┴──┬──┴─────┴──┬──┴─────┴─────┘                │
     * │     │           │           │                               │
     * │     │           │  ┌────────┘                               │
     * │     │   ┌───────┘  │                                        │
     * │     └───│──────────│────────┐                               │
     * │         ▼          ▼        ▼                               │
     * │  DENSE STORAGE (contiguous)                                 │
     * │  ┌─────────┬─────────┬─────────┐                            │
     * │  │  T[0]   │  T[1]   │  T[2]   │                            │
     * │  │ (id=2)  │ (id=4)  │ (id=0)  │                            │
     * │  └─────────┴─────────┴─────────┘                            │
     * │                                                             │
     * │  DENSE-TO-SPARSE (reverse mapping for swap-and-pop)         │
     * │  ┌─────┬─────┬─────┐                                        │
     * │  │  2  │  4  │  0  │  (EntityId)                            │
     * │  └─────┴─────┴─────┘                                        │
     * │                                                             │
     * │  × = Tombstone (empty slot)                                 │
     * └─────────────────────────────────────────────────────────────┘
     * ```
     *
     * ## Complexity
     *
     * | Operation | Time    | Space        |
     * |-----------|---------|--------------|
     * | emplace   | O(1)*   | O(max_id)    |
     * | insert    | O(1)*   | O(max_id)    |
     * | get       | O(1)    | -            |
     * | remove    | O(1)    | -            |
     *
     * *Amortized due to potential sparse array resize.
     *
     * ## Usage
     *
     * ```cpp
     * SparseSet<TransformComponent> transforms;
     *
     * // In-place construction
     * auto* transform = transforms.emplace(42, glm::vec3{0.0f}, glm::quat{}, glm::vec3{1.0f});
     *
     * // Or insert a pre-constructed object
     * auto* inserted = transforms.insert(43, TransformComponent{...});
     *
     * // Mutable access
     * if (auto* t = transforms.get(42)) {
     *     t->position = glm::vec3{10.0f, 0.0f, 0.0f};
     * }
     *
     * // Const access
     * const auto& constSet = transforms;
     * if (const auto* t = constSet.get(42)) { }
     *
     * // Remove component
     * transforms.remove(42);
     * ```
     *
     * @tparam T The type of elements stored in the set. Must be move-assignable.
     *
     * @see EntityId
     * @see EntityTombstone
     */
    template <typename T>
    class SparseSet : public SparseSetBase {


        /**
         * @brief Maps EntityId to dense storage index.
         *
         * Contains `Tombstone` for empty slots.
         */
        std::vector<size_t> sparse_;

        /**
         * @brief Reverse mapping from dense index to EntityId.
         *
         * Used during swap-and-pop removal to update the sparse array.
         */
        std::vector<EntityId> denseToSparse_;

        /**
         * @brief Contiguous storage of elements.
         */
        std::vector<T> storage_;

    public:

        /**
         * @brief Default constructor creating an empty sparse set.
         */
        SparseSet() = default;

        /**
         * @brief Constructs a sparse set with pre-allocated capacity.
         *
         * @param capacity The initial capacity to reserve for all internal vectors.
         */
        explicit SparseSet(const size_t capacity) {
            sparse_.reserve(capacity);
            storage_.reserve(capacity);
            denseToSparse_.reserve(capacity);
        };

        /**
         * @brief Copy operations are deleted to prevent accidental duplication.
         */
        SparseSet(const SparseSet&) = delete;

        /**
         * @brief Copy assignment is deleted.
         */
        SparseSet& operator=(const SparseSet&) = delete;

        /**
         * @brief Move constructor.
         */
        SparseSet(SparseSet&&) noexcept = default;

        /**
         * @brief Move assignment operator.
         */
        SparseSet& operator=(SparseSet&&) noexcept = default;


        /**
         * @brief Constructs and inserts an element at the given index.
         *
         * Forwards arguments to construct `T` in-place.
         *
         * @tparam Args Constructor argument types.
         *
         * @param idx The EntityId to associate with the element.
         * @param args Arguments forwarded to the `T` constructor.
         *
         * @return Pointer to the inserted element, or `nullptr` if the index is already occupied.
         */
        template <typename... Args>
        [[nodiscard]] T* emplace(const EntityId idx, Args&& ...args) {

            // already in use
            if (idx < sparse_.size() && sparse_[idx] != Tombstone) {
                return nullptr;
            }

            if (idx >= sparse_.size()) {
                sparse_.resize(idx + 1, Tombstone);
            }

            const auto denseIndex = storage_.size();

            denseToSparse_.push_back(idx);
            storage_.emplace_back(std::forward<Args>(args)...);

            sparse_[idx] = denseIndex;

            return &storage_.back();
        }

        /**
         * @brief Inserts an element at the given index.
         *
         * If the sparse array is too small, it is resized to accommodate the index.
         * Empty slots are filled with `Tombstone`.
         *
         * @param idx The EntityId to associate with the element.
         * @param obj The element to insert (moved).
         *
         * @return Pointer to the inserted element, or `nullptr` if the index is already occupied.
         */
        [[nodiscard]] T* insert(const EntityId idx, T&& obj) {

            // already in use
            if (idx < sparse_.size() && sparse_[idx] != Tombstone) {
                return nullptr;
            }

            if (idx >= sparse_.size()) {
                sparse_.resize(idx + 1, Tombstone);
            }

            const auto denseIndex = storage_.size();

            denseToSparse_.push_back(idx);
            storage_.emplace_back(std::move(obj));

            sparse_[idx] = denseIndex;

            return &storage_.back();
        }


        /**
         * @brief Removes the element at the given index using swap-and-pop.
         *
         * @details Uses the swap-and-pop technique for O(1) removal:
         * 1. Move the last element to the position of the removed element
         * 2. Update the sparse array entry for the moved element
         * 3. Pop the last element from dense storage
         * 4. Mark the removed slot as Tombstone
         *
         * @param idx The EntityId of the element to remove.
         *
         * @return `true` if the element was removed, `false` if not found.
         */
        [[nodiscard]] bool remove(const EntityId idx) override {

            if (idx >= sparse_.size() || sparse_[idx] == Tombstone) {
                return false;
            }

            const auto denseIndex = sparse_[idx];
            const auto sparseIdx  = denseToSparse_[denseIndex];

            assert(sparseIdx == idx && "Sparse index mismatch");

            if (denseIndex != storage_.size() - 1) {
                storage_[denseIndex] = std::move(storage_.back());
                const auto newSparseIndex = denseToSparse_.back();
                sparse_[newSparseIndex] = denseIndex;
                denseToSparse_[denseIndex] = newSparseIndex;
            }

            storage_.pop_back();
            denseToSparse_.pop_back();

            sparse_[idx] = Tombstone;

            return true;
        }

        /**
         * @brief Retrieves the element at the given index.
         *
         * @param idx The EntityId to look up.
         *
         * @return Pointer to the element, or `nullptr` if not found.
         */
        [[nodiscard]] T* get(const EntityId idx) {

            if (idx >= sparse_.size() || sparse_[idx] == Tombstone) {
                return nullptr;
            }

            return &storage_[sparse_[idx]];
        }

        /**
         * @brief Retrieves the element at the given index.
         *
         * @param idx The EntityId to look up.
         *
         * @return Const Pointer to the element, or `nullptr` if not found.
         */
        [[nodiscard]] const T* get(const EntityId idx) const  {

            if (idx >= sparse_.size() || sparse_[idx] == Tombstone) {
                return nullptr;
            }

            return &storage_[sparse_[idx]];
        }


        /**
         * @brief Checks whether an element is registered for the specified EntityId.
         *
         * @param idx The entityId to test.
         *
         * @return True if this sparse set contains the entityId, otherwise false.
         */
        [[nodiscard]] bool contains(const EntityId idx) const override {
            return idx < sparse_.size() && sparse_[idx] != Tombstone;
        }

        /**
         * @copydoc SparseSetBase::raw
         */
        [[nodiscard]] void* raw(const helios::engine::core::data::EntityId id) override {

            T* ptr = get(id);
            return static_cast<void*>(ptr);
        }


        /**
         * @brief Forward iterator for traversing the sparse set.
         *
         * @details Iterates over the dense storage array, providing access to both
         * the stored element and its associated EntityId.
         */
        struct Iterator {
            using DataIt = typename std::vector<T>::iterator;
            using IdIt = typename std::vector<EntityId>::iterator;

            DataIt dataIt_;
            IdIt idIt_;

            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;

            Iterator() = default;

            Iterator(DataIt dataIt, IdIt idIt) : dataIt_(dataIt), idIt_(idIt) {}

            reference operator*() const { return *dataIt_; }
            pointer operator->() const { return &*dataIt_; }

            /**
             * @brief Returns the EntityId for the current element.
             *
             * @return The EntityId associated with the current element.
             */
            [[nodiscard]] EntityId entityId() const { return *idIt_; }

            [[nodiscard]] bool operator==(const Iterator& other) const { return dataIt_ == other.dataIt_;}
            [[nodiscard]] bool operator!=(const Iterator& other) const { return dataIt_ != other.dataIt_;}

            Iterator& operator++(int) {
                Iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            Iterator& operator++() {
                ++dataIt_; ++idIt_; return *this;
            }
        };

        /**
         * @brief Const forward iterator for traversing the sparse set.
         *
         * @details Provides read-only access to elements and their EntityIds.
         */
        struct ConstIterator {
            using DataIt = typename std::vector<T>::const_iterator;
            using IdIt = typename std::vector<EntityId>::const_iterator;

            DataIt dataIt_;
            IdIt idIt_;

            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = const T*;
            using reference = const T&;

            ConstIterator() = default;

            ConstIterator(DataIt dataIt, IdIt idIt) : dataIt_(dataIt), idIt_(idIt) {}

            reference operator*() const { return *dataIt_; }
            pointer operator->() const { return &*dataIt_; }

            /**
             * @brief Returns the EntityId for the current element.
             *
             * @return The EntityId associated with the current element.
             */
            [[nodiscard]] EntityId entityId() const { return *idIt_; }

            [[nodiscard]] bool operator==(const ConstIterator& other) const { return dataIt_ == other.dataIt_;}
            [[nodiscard]] bool operator!=(const ConstIterator& other) const { return dataIt_ != other.dataIt_;}

            ConstIterator& operator++(int) {
                ConstIterator tmp = *this;
                ++(*this);
                return tmp;
            }

            ConstIterator& operator++() {
                ++dataIt_; ++idIt_; return *this;
            }
        };

        /**
         * @brief Returns an iterator to the beginning of the dense storage.
         *
         * @return Iterator pointing to the first element.
         */
        [[nodiscard]] Iterator begin() {
            return Iterator(storage_.begin(), denseToSparse_.begin());
        }

        /**
         * @brief Returns an iterator to the end of the dense storage.
         *
         * @return Iterator pointing past the last element.
         */
        [[nodiscard]] Iterator end() {
            return Iterator(storage_.end(), denseToSparse_.end());
        }

        /**
         * @brief Returns a const iterator to the beginning of the dense storage.
         *
         * @return ConstIterator pointing to the first element.
         */
        [[nodiscard]] ConstIterator begin() const {
            return ConstIterator(storage_.begin(), denseToSparse_.begin());
        }

        /**
         * @brief Returns a const iterator to the end of the dense storage.
         *
         * @return ConstIterator pointing past the last element.
         */
        [[nodiscard]] ConstIterator end() const {
            return ConstIterator(storage_.end(), denseToSparse_.end());
        }

    };



}
