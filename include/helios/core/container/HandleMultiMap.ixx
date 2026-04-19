/**
 * @file HandleMultiMap.ixx
 * @brief Dense 1:N association map between two handle types.
 */
module;

#include <vector>
#include <span>
#include <cassert>

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

    public:

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
            if (value.entityId < keys_.size()
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


    };

}