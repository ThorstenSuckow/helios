/**
 * @file MatchStateComponent.ixx
 * @brief Component for storing match state on an entity.
 */
module;

export module helios.engine.mechanics.match.components.MatchStateComponent;

import helios.engine.mechanics.match.types.MatchState;

export namespace helios::engine::mechanics::match::components {

    /**
     * @brief Stores the current match state associated with an entity.
     *
     * Typically attached to a session or global entity to track the
     * current state of the match (e.g., Warmup, Playing, PlayerDeath).
     */
    struct MatchStateComponent  {


        helios::engine::mechanics::match::types::MatchState matchState_;

    public:

        /**
         * @brief Sets the match state.
         *
         * @param matchState The new match state.
         */
        void setMatchState(helios::engine::mechanics::match::types::MatchState matchState) noexcept {
            matchState_ = matchState;
        }

        /**
         * @brief Returns the current match state.
         */
        [[nodiscard]] helios::engine::mechanics::match::types::MatchState matchState() const noexcept {
            return matchState_;
        }

    };


}