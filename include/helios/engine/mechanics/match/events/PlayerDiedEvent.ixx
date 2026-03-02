/**
 * @file PlayerDiedEvent.ixx
 * @brief Event emitted when the player entity dies.
 */
module;

export module helios.engine.mechanics.match.events.PlayerDiedEvent;

import helios.engine.ecs;
import helios.util.Guid;
import helios.math;
import helios.engine.mechanics.health.types.HealthChangeContext;
import helios.core.types;
import helios.engine.ecs.EntityHandle;

using namespace helios::engine::ecs;

export namespace helios::engine::mechanics::match::events {

    /**
     * @brief Event signalling that the player entity has died.
     *
     * Consumed by match-rule systems to trigger life loss, game-over
     * transitions, or respawn logic.
     */
    class PlayerDiedEvent {

        /**
         * @brief Handle of the player entity that died.
         */
        EntityHandle source_;

    public:

        /**
         * @brief Constructs a PlayerDiedEvent.
         *
         * @param source Handle of the deceased player entity.
         */
        explicit PlayerDiedEvent(const EntityHandle source) : source_(source) {}

        /**
         * @brief Returns the handle of the deceased player entity.
         *
         * @return The source EntityHandle.
         */
        [[nodiscard]] EntityHandle source() const noexcept {
            return source_;
        }

    };


}