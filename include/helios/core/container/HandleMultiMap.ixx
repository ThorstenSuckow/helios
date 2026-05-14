/**
 * @file HandleMultiMap.ixx
 * @brief Dense 1:N association map between two handle types.
 */
module;

#include <vector>
#include <span>
#include <cassert>
#include <concepts>
#include <iterator>
#include <cstddef>

export module helios.core.container.HandleMultiMap;

import helios.ecs.types.EntityHandle;

using namespace helios::ecs;
export namespace helios::core::container {

    /**
     * @brief Stores a one-to-many relation between handle domains.
     *
     * @details The map keeps two dense lookup structures:
     * - `values_`: key handle id -> list of associated value handles.
     * - `keys_`: value handle id -> owning key handle.
     *
     * The implementation uses `entityId` as direct index and therefore expects
     * stable id semantics for both handle types.
     *
     * @tparam TOneHandle Handle type representing the "one" side.
     * @tparam TManyHandle Handle type representing the "many" side.
     */
    template <typename TOneHandle, typename TManyHandle>
    class HandleMultiMap {

        /**
         * @brief Forward mapping from key handle id to associated value handles.
         */
        std::vector<std::vector<TManyHandle>> values_;

        /**
         * @brief Reverse mapping from value handle id to owning key handle.
         */
        std::vector<TOneHandle> keys_;

        /**
         * @brief Dense list of all currently bound value handles.
         *
         * @details Used by the iterator to avoid scanning holes in `keys_`.
         */
        std::vector<TManyHandle> denseValues_;

    public:

        /**
         * @brief Represents one valid binding of the one-to-many relation.
         */
        struct Binding {
            /** @brief Handle on the "one" side. */
            TOneHandle key;
            /** @brief Associated handle on the "many" side. */
            TManyHandle value;
        };

        /**
         * @brief Forward iterator over dense bindings.
         */
        class ConstIterator {
            const HandleMultiMap* map_;
            std::size_t current_idx_;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type        = Binding;
            using difference_type   = std::ptrdiff_t;
            using pointer           = void;
            using reference         = value_type; // By-Value Return

            /**
             * @brief Constructs an iterator at the given dense value index.
             *
             * @param map Owning map.
             * @param start_idx Start index within `denseValues_`.
             */
            ConstIterator(const HandleMultiMap* map, std::size_t start_idx) noexcept
                : map_(map), current_idx_(start_idx) {}

            /** @brief Returns the current binding by value. */
            [[nodiscard]] reference operator*() const noexcept {
                assert(current_idx_ < map_->denseValues_.size());

                const auto value = map_->denseValues_[current_idx_];
                const auto valueIdx = value.entityId;

                assert(valueIdx < map_->keys_.size());
                assert(map_->keys_[valueIdx].isValid());

                return Binding{
                    map_->keys_[valueIdx],
                    value
                };
            }

            /** @brief Advances to the next dense binding. */
            ConstIterator& operator++() noexcept {
                current_idx_++;
                return *this;
            }

            /** @brief Advances to the next dense binding and returns the previous iterator state. */
            ConstIterator operator++(int) noexcept {
                ConstIterator tmp = *this;
                ++(*this);
                return tmp;
            }

            /** @brief Checks iterator equality. */
            [[nodiscard]] bool operator==(const ConstIterator& other) const noexcept {
                return current_idx_ == other.current_idx_
                    && map_ == other.map_;
            }

            /** @brief Checks iterator inequality. */
            [[nodiscard]] bool operator!=(const ConstIterator& other) const noexcept {
                return !(*this == other);
            }
        };

        /** @brief Returns an iterator to the first dense binding. */
        [[nodiscard]] ConstIterator begin() const noexcept {
            return ConstIterator(this, 0);
        }

        /** @brief Returns the end iterator for dense binding iteration. */
        [[nodiscard]] ConstIterator end() const noexcept {
            return ConstIterator(this, denseValues_.size());
        }

        /**
         * @brief Binds two entities by forwarding their handles.
         *
         * @tparam TOneEntity Entity type on the "one" side.
         * @tparam TManyEntity Entity type on the "many" side.
         * @param oneEntity Entity owning the relation.
         * @param manyEntity Entity associated with the owner.
         * @return `true` if the binding was inserted, otherwise `false`.
         */
        template<typename TOneEntity, typename TManyEntity>
        requires std::same_as<TOneHandle, typename TOneEntity::Handle_type> && std::same_as<TManyHandle, typename TManyEntity::Handle_type>
        bool bind(TOneEntity oneEntity, TManyEntity manyEntity) {
            return bind(oneEntity.handle(), manyEntity.handle());
        }

        /**
         * @brief Binds a value handle to a key handle.
         *
         * @details Inserts the value into the key bucket and stores reverse
         * ownership in `keys_`. Returns `false` if the value is already bound.
         *
         * @param key Key handle on the "one" side.
         * @param value Value handle on the "many" side.
         *
         * @return `true` if the binding was inserted, otherwise `false`.
         */
        bool bind(const TOneHandle key, const TManyHandle value) {

            const auto keyIdx = key.entityId;
            const auto valueIdx = value.entityId;

            // assert that the value was not already registered.
            if (valueIdx < keys_.size()
                && keys_[valueIdx].isValid()) {
                assert(false && "Value already exists");
                return false;
            }

            // one idx to many handles
            if (values_.size() <= keyIdx) {
                values_.resize(keyIdx + 1);
            }
            values_[keyIdx].push_back(value);

            // many idx to one handles
            if (keys_.size() <= valueIdx) {
                keys_.resize(valueIdx + 1);
            }
            keys_[valueIdx] = key;

            // dense value iteration
            denseValues_.push_back(value);

            return true;
        }

        /**
         * @brief Returns the owning key handle for a value handle.
         *
         * @param value Value handle from the "many" side.
         *
         * @return Owning key handle, or default-constructed handle if not found.
         */
        [[nodiscard]] TOneHandle key(const TManyHandle value) const noexcept {
            const auto valueIdx = value.entityId;

            if (keys_.size() <= valueIdx) {
                return TOneHandle{};
            }

            return keys_[valueIdx];
        }

        /**
         * @brief Returns all value handles bound to a key handle.
         *
         * @param key Key handle from the "one" side.
         *
         * @return Read-only span of associated value handles. Empty if key is unknown.
         */
        [[nodiscard]] std::span<const TManyHandle> values(const TOneHandle key) const noexcept {

            const auto keyIdx = key.entityId;

            if (values_.size() <= keyIdx) {
                return {};
            }

            return values_[keyIdx];
        }

        /** @brief Removes all bindings and clears all internal lookup structures. */
        void clear() {
            values_.clear();
            keys_.clear();
            denseValues_.clear();
        }

        /**
         * @brief Reserves capacity for dense key/value storage.
         *
         * @param keyCapacity Expected number of distinct keys.
         * @param valueCapacity Expected number of bound values.
         */
        void reserve(std::size_t keyCapacity, std::size_t valueCapacity) {
            keys_.reserve(valueCapacity);
            values_.reserve(keyCapacity);
            denseValues_.reserve(valueCapacity);
        }

    };

}