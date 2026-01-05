/**
 * @file TriggerCollisionEvent.ixx
 * @brief Event dispatched when a GameObject enters a trigger volume.
 */
module;

export module helios.engine.game.physics.collision.events.TriggerCollisionEvent;

import helios.engine.game.GameObject;
import helios.math;

export namespace helios::engine::game::physics::collision::events {

    /**
     * @brief Event representing a non-physical collision with a trigger volume.
     *
     * @details TriggerCollisionEvent is dispatched by the collision detection system
     * when a GameObject enters a trigger collider. Trigger colliders are non-solid
     * volumes used to detect presence without physical collision response. Common
     * use cases include pickup zones, damage areas, spawn triggers, and checkpoints.
     *
     * Unlike SolidCollisionEvent, trigger events do not imply any physics-based
     * resolution. The receiving system is responsible for defining the gameplay
     * response (e.g., collecting an item, activating a trap, starting a cutscene).
     *
     * @see SolidCollisionEvent
     */
    class TriggerCollisionEvent {

        /**
         * @brief The GameObject that entered the trigger volume.
         */
        const helios::engine::game::GameObject* source_;

        /**
         * @brief The trigger volume GameObject that was entered.
         */
        const helios::engine::game::GameObject* match_;

        /**
         * @brief The world-space contact point where the trigger was activated.
         */
        const helios::math::vec3f contact_;

    public:

        /**
         * @brief Constructs a TriggerCollisionEvent.
         *
         * @param source The GameObject that entered the trigger.
         * @param match The trigger volume GameObject.
         * @param contact The world-space position where the trigger was activated.
         */
        explicit TriggerCollisionEvent(
            const helios::engine::game::GameObject* source,
            const helios::engine::game::GameObject* match,
            const helios::math::vec3f contact
        ) : source_(source), match_(match), contact_(contact) {}

        /**
         * @brief Returns the contact point in world space.
         *
         * @return The 3D position where the trigger was activated.
         */
        [[nodiscard]] helios::math::vec3f contact() const noexcept {
            return contact_;
        }

        /**
         * @brief Returns the source GameObject.
         *
         * @return Non-owning pointer to the GameObject that entered the trigger.
         */
        [[nodiscard]] const helios::engine::game::GameObject* source() const noexcept {
            return source_;
        }

        /**
         * @brief Returns the trigger volume GameObject.
         *
         * @return Non-owning pointer to the trigger volume that was entered.
         */
        [[nodiscard]] const helios::engine::game::GameObject* match() const noexcept {
            return match_;
        }


    };

}
