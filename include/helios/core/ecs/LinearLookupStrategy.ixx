/**
 * @file LinearLookupStrategy.ixx
 * @brief Linear-scan lookup strategy for strong ID collision detection.
 */
module;

#include <vector>
#include <algorithm>
#include <ranges>

export module helios.core.ecs.LinearLookupStrategy;

import helios.core.types;

using namespace helios::core::types;
export namespace helios::core::ecs {

    /**
     * @brief Lookup strategy using a flat vector with linear scan.
     *
     * `LinearLookupStrategy` stores strong IDs in a contiguous vector and
     * performs linear scans for membership tests. Swap-and-pop is used for
     * O(1) removal. Suitable for small registries where hash overhead is
     * undesirable.
     *
     * @tparam TCapacity Default initial capacity for pre-allocation.
     *
     * @see EntityRegistry
     * @see HashedLookupStrategy
     */
    template<size_t TCapacity>
    class LinearLookupStrategy {

        /**
         * @brief Vector of registered strong IDs.
         */
        std::vector<StrongId_t> strongIds_;

    public:

        /**
         * @brief Constructs a strategy with pre-allocated capacity.
         *
         * @param capacity The initial capacity to reserve.
         */
        explicit LinearLookupStrategy(const size_t capacity = TCapacity) {
            strongIds_.reserve(capacity);
        };

        /**
         * @brief Removes a strong ID using swap-and-pop.
         *
         * @param id The strong ID to remove.
         *
         * @return True if removed, false if not found.
         */
        [[nodiscard]] bool remove(const StrongId_t id) {

            const auto it = std::ranges::find(strongIds_, id);

            if (it == strongIds_.end()) {
                return false;
            }

            if (it == strongIds_.end() - 1) {
                strongIds_.pop_back();
                return true;
            }

            const auto idx = std::distance(strongIds_.begin(), it);
            const auto tmp = strongIds_.back();
            strongIds_[idx] = tmp;
            strongIds_.pop_back();

            return true;
        }

        /**
         * @brief Registers a strong ID.
         *
         * @param id The strong ID to add.
         *
         * @return True if inserted, false if already present.
         */
        [[nodiscard]] bool add(const StrongId_t id) {
            if (has(id)) {
                return false;
            }
            strongIds_.push_back(id);
            return true;
        }

        /**
         * @brief Checks whether a strong ID is registered.
         *
         * @param id The strong ID to test.
         *
         * @return True if the ID is present.
         */
        [[nodiscard]] bool has(const StrongId_t id) const {
            return std::ranges::find(strongIds_, id) != strongIds_.end();
        }


    };


}