/**
 * @file StateToViewportPolicy.ixx
 * @brief Policy mapping game/match states to active viewports.
 */
module;

#include <vector>
#include <span>
#include <helios/helios_config.h>
#include <cassert>
#include <bit>

export module helios.engine.modules.rendering.viewport.types.StateToViewportPolicy;


import helios.engine.mechanics.gamestate.types;
import helios.engine.mechanics.match.types;

import helios.engine.core.data.ViewportId;
import helios.core.types;

export namespace helios::engine::modules::rendering::viewport::types {

    using namespace helios::engine::core::data;
    using namespace helios::engine::mechanics::gamestate::types;
    using namespace helios::engine::mechanics::match::types;

    /**
     * @brief Defines which viewports are active for specific game/match states.
     *
     * @details StateToViewportPolicy maps GameState and MatchState combinations
     * to lists of ViewportIds. States are treated as bitmasks, allowing a single
     * viewport to be associated with multiple states.
     *
     * The policy uses sparse vectors indexed by the bit position of each state,
     * enabling O(1) lookup for single-bit states.
     *
     * ## Usage
     *
     * ```cpp
     * StateToViewportPolicy policy;
     * policy.add(GameState::Running, gameViewportId)
     *       .add(GameState::Paused, menuViewportId)
     *       .add(MatchState::Playing, hudViewportId);
     *
     * auto viewports = policy.viewportIds(GameState::Running, MatchState::Playing);
     * // Returns: [gameViewportId, hudViewportId]
     * ```
     *
     * @see StateToViewportPolicyUpdateSystem
     */
    class StateToViewportPolicy {

        /**
         * @brief Viewport IDs indexed by GameState bit position.
         */
        std::vector<std::vector<ViewportId>> gameStates_;

        /**
         * @brief Viewport IDs indexed by MatchState bit position.
         */
        std::vector<std::vector<ViewportId>> matchStates_;

        /**
         * @brief Temporary buffer for combining viewport IDs.
         */
        mutable std::vector<ViewportId> combined_;

        /**
         * @brief Updates the state-to-viewport mapping for a given bitmask.
         *
         * @param st State bitmask to update.
         * @param viewportId Viewport ID to associate with the state(s).
         * @param states Target vector to update (gameStates_ or matchStates_).
         */
        void update(size_t st, const ViewportId viewportId, std::vector<std::vector<ViewportId>>& states) {
           
            size_t idx = 0;
            while (st > 0) {

                if ((st & 1) != 0) {
                    if (states.size() <= idx) {
                        states.resize(idx + 1);
                    }
                    if (std::ranges::find(states[idx], viewportId) == states[idx].end()) {
                        states[idx].push_back(viewportId);
                    }
                }

                idx++;
                st >>= 1;
            }

            if (combined_.size() <= states.size()) {
                combined_.resize(states.size() + 1);
            }

        }
        
    public:

        /**
         * @brief Default constructor with pre-allocated capacity.
         */
        StateToViewportPolicy() {
            gameStates_.reserve(STATE_TO_VIEWPORT_DEFAULT_CAPACITY);
            matchStates_.reserve(STATE_TO_VIEWPORT_DEFAULT_CAPACITY);
            combined_.reserve(STATE_TO_VIEWPORT_DEFAULT_CAPACITY);
        };

        /**
         * @brief Associates a viewport with one or more match states.
         *
         * @param state MatchState bitmask to associate.
         * @param viewportId Viewport ID to add.
         *
         * @return Reference to this policy for method chaining.
         */
        StateToViewportPolicy& add(MatchState state, ViewportId viewportId) noexcept {
            const auto st = static_cast<size_t>(state);

            if (st == 0) {
                return *this;
            }
            
            update(st, viewportId, matchStates_);
            
            return *this;
        }

        /**
         * @brief Associates a viewport with one or more game states.
         *
         * @param state GameState bitmask to associate.
         * @param viewportId Viewport ID to add.
         *
         * @return Reference to this policy for method chaining.
         */
        StateToViewportPolicy& add(GameState state, ViewportId viewportId) noexcept {
            const auto st = static_cast<size_t>(state);

            if (st == 0) {
                return *this;
            }

            update(st, viewportId, gameStates_);
            
            return *this;
        }

        /**
         * @brief Returns viewport IDs for the given game/match state combination.
         *
         * @details Combines viewports associated with the GameState and MatchState.
         * GameState must be a single-bit (power of 2) value. MatchState can be
         * Undefined, in which case only GameState viewports are returned.
         *
         * @param gameState The current game state (must be a single state, not bitmask).
         * @param matchState The current match state (may be Undefined).
         *
         * @return Reference to internal vector of combined viewport IDs.
         */
        [[nodiscard]] const std::vector<ViewportId>& viewportIds(const GameState gameState, const MatchState matchState) const noexcept {
            combined_.clear();

            assert(gameState != GameState::Undefined && "GameState must be defined");

            assert((std::to_underlying(gameState) & (std::to_underlying(gameState) -1)) == 0 && "GameState must be a power of 2");
            assert((std::to_underlying(matchState) & (std::to_underlying(matchState) -1)) == 0 && "MatchState must be a power of 2");

            const auto gameIdx = static_cast<size_t>(std::countr_zero(std::to_underlying(gameState)));
            const auto matchIdx = matchState != MatchState::Undefined
                                    ? static_cast<size_t>(std::countr_zero(std::to_underlying(matchState)))
                                    : 0;

            if (gameStates_.size() <= gameIdx) {
                return combined_;
            }

            for (const auto viewportId : gameStates_[gameIdx]) {
                combined_.push_back(viewportId);
            }

            if (matchStates_.size() <= matchIdx || matchIdx == 0) {
                return combined_;
            }

            for (const auto viewportId : matchStates_[matchIdx]) {
                if (std::ranges::find(combined_, viewportId) == combined_.end()) {
                    combined_.push_back(viewportId);
                }
            }

            return combined_;
        }


    };


}