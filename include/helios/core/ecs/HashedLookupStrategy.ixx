/**
 * @file HashedLookupStrategy.ixx
 * @brief Hash-based lookup strategy for strong ID collision detection.
 */
module;

#include <unordered_set>

export module helios.core.ecs.HashedLookupStrategy;

import helios.core.types;

using namespace helios::core::types;
export namespace helios::core::ecs {

    /**
     * @brief Lookup strategy using an unordered set for O(1) amortized lookups.
     *
     * `HashedLookupStrategy` maintains a set of registered strong IDs
     * and provides constant-time insertion, removal, and membership tests.
     * Used as the default lookup strategy in `EntityRegistry`.
     *
     * @see EntityRegistry
     * @see LinearLookupStrategy
     */
    class HashedLookupStrategy {

        /**
         * @brief Set of registered strong IDs.
         */
        std::unordered_set<StrongId_t> strongIds_;

    public:

        /**
         * @brief Constructs a strategy with pre-allocated capacity.
         *
         * @param capacity The initial bucket count to reserve.
         */
        explicit HashedLookupStrategy(const size_t capacity) {
            strongIds_.reserve(capacity);
        };

        /**
         * @brief Registers a strong ID.
         *
         * @param id The strong ID to add.
         *
         * @return True if inserted, false if already present.
         */
        [[nodiscard]] bool add(const StrongId_t id) {
            return strongIds_.insert(id).second;
        }

        /**
         * @brief Removes a strong ID.
         *
         * @param id The strong ID to remove.
         *
         * @return True if removed, false if not found.
         */
        [[nodiscard]] bool remove(const StrongId_t id) {
            return strongIds_.erase(id) > 0;
        }

        /**
         * @brief Checks whether a strong ID is registered.
         *
         * @param id The strong ID to test.
         *
         * @return True if the ID is present.
         */
        [[nodiscard]] bool has(const StrongId_t id) const {
            return strongIds_.contains(id);
        }


    };


}