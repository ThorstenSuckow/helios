/**
 * @file StateToIdMapPair.ixx
 * @brief Combined policy mapping two state types to IDs.
 */
module;

#include <vector>
#include <span>
#include <helios/helios_config.h>
#include <cassert>
#include <bit>
#include <algorithm>
#include <iterator>

export module helios.engine.state.StateToIdMapPair;

import helios.engine.state.StateToIdMap;

import helios.engine.mechanics.gamestate.types;
import helios.engine.mechanics.match.types;

import helios.engine.core.data.ViewportId;
import helios.core.types;

export namespace helios::engine::state {

    using namespace helios::engine::core::data;
    using namespace helios::engine::mechanics::gamestate::types;
    using namespace helios::engine::mechanics::match::types;

    /**
     * @brief Combines two StateToIdMap instances for dual-state lookups.
     *
     * @details Allows associating IDs with two different state types
     * (e.g., GameState and MatchState). When queried, returns the
     * union of IDs from both maps for the given state combination.
     *
     * @tparam LState The left/primary state type.
     * @tparam RState The right/secondary state type.
     * @tparam TId The ID type to associate with states.
     */
    template<typename LState, typename RState, typename TId>
    class StateToIdMapPair {

        /**
         * @brief Map for the left state type.
         */
        StateToIdMap<LState, TId> lft_;

        /**
         * @brief Map for the right state type.
         */
        StateToIdMap<RState, TId> rgt_;

        /**
         * @brief Buffer for merged results.
         */
        mutable std::vector<TId> combined_;

        /**
         * @brief Merges two sorted ID lists into the output.
         *
         * @param out The output vector.
         * @param lft IDs from the left map.
         * @param rgt IDs from the right map.
         */
        void mergeInto(std::vector<TId>& out, std::span<const TId> lft, std::span<const TId> rgt) const  {

            out.clear();

            out.reserve(lft.size() + rgt.size());

            std::set_union(lft.begin(), lft.end(), rgt.begin(), rgt.end(), std::back_inserter(out));
        }

    public:

        /**
         * @brief Adds an ID for the left state type.
         *
         * @param state The state to associate the ID with.
         * @param id The ID to add.
         *
         * @return Reference to this map for chaining.
         */
        StateToIdMapPair& add(LState state, TId id) {
            lft_.add(state, id);
            return *this;
        }

        /**
         * @brief Adds an ID for the right state type.
         *
         * @param state The state to associate the ID with.
         * @param id The ID to add.
         *
         * @return Reference to this map for chaining.
         */
        StateToIdMapPair& add(RState state, TId id) {
            rgt_.add(state, id);
            return *this;
        }

        /**
         * @brief Returns IDs for a state combination.
         *
         * @details Returns the sorted union of IDs from both maps.
         * Both maps must be frozen before calling this method.
         *
         * @param stateLft The left state to query.
         * @param stateRgt The right state to query.
         *
         * @return Span of IDs for the state combination.
         */
        [[nodiscard]] std::span<const TId> ids(const LState stateLft, const RState stateRgt) const  {

            assert(lft_.isFrozen() && rgt_.isFrozen() && "Cannot merge if sources are not frozen.");

            if (!lft_.isFrozen() || !rgt_.isFrozen()) {
                return {};
            }

            mergeInto(combined_, lft_.ids(stateLft), rgt_.ids(stateRgt));

            return combined_;
        }

        /**
         * @brief Freezes both underlying maps.
         */
        void freeze() {
            lft_.freeze();
            rgt_.freeze();
        }

    };


}