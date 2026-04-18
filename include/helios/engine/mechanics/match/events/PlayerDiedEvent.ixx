/**
 * @file PlayerDiedEvent.ixx
 * @brief Event emitted when the player entity dies.
 */
module;

export module helios.engine.mechanics.match.events.PlayerDiedEvent;

import helios.ecs;
import helios.util.Guid;
import helios.math;
import helios.engine.mechanics.health.types.HealthChangeContext;
import helios.core.types;

using namespace helios::ecs::types;

export namespace helios::engine::mechanics::match::events {

    /**
     * @brief Event signalling that the player entity has died.
     *
     * Consumed by match-rule systems to trigger life loss, game-over
     * transitions, or respawn logic.
     */
    template<typename THandle>
    class PlayerDiedEvent {

        /**
         * @brief Handle of the player entity that died.
         */
        THandle source_;

    public:

        /**
         * @brief Constructs a PlayerDiedEvent.
         *
         * @param source Handle of the deceased player entity.
         */
        explicit PlayerDiedEvent(const THandle source) : source_(source) {}

        /**
         * @brief Returns the handle of the deceased player entity.
         *
         * @return The source THandle.
         */
        [[nodiscard]] THandle source() const noexcept {
            return source_;
        }

    };


}