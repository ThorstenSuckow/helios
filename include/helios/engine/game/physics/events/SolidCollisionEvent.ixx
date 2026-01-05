/**
 * @file SolidCollisionEvent.ixx
 * @brief Event dispatched when two solid colliders physically collide.
 */
module;

export module helios.engine.game.physics.events.SolidCollisionEvent;

import helios.engine.game.GameObject;
import helios.math;

export namespace helios::engine::game::physics::events {

    /**
     * @brief Event representing a physical collision between two solid objects.
     *
     * @details SolidCollisionEvent is dispatched by the collision detection system
     * when two GameObjects with solid colliders (non-trigger) intersect. Unlike
     * TriggerCollisionEvent, this event indicates a physical interaction that may
     * require resolution such as position correction, velocity reflection, or
     * damage calculation.
     *
     * The event carries references to both participating GameObjects and the
     * contact point in world space where the collision occurred.
     */
    class SolidCollisionEvent {

        /**
         * @brief The primary GameObject that initiated or detected the collision.
         */
        const helios::engine::game::GameObject* source_;

        /**
         * @brief The secondary GameObject that was collided with.
         */
        const helios::engine::game::GameObject* match_;

        /**
         * @brief The world-space contact point of the collision.
         */
        const helios::math::vec3f contact_;

    public:

        /**
         * @brief Constructs a SolidCollisionEvent.
         *
         * @param source The GameObject that detected or initiated the collision.
         * @param match The GameObject that was collided with.
         * @param contact The world-space position where the collision occurred.
         */
        explicit SolidCollisionEvent(
            const helios::engine::game::GameObject* source,
            const helios::engine::game::GameObject* match,
            const helios::math::vec3f contact
        ) : source_(source), match_(match), contact_(contact) {}

        /**
         * @brief Returns the contact point in world space.
         *
         * @return The 3D position where the collision occurred.
         */
        [[nodiscard]] helios::math::vec3f contact() const noexcept {
            return contact_;
        }

        /**
         * @brief Returns the source GameObject.
         *
         * @return Non-owning pointer to the GameObject that detected the collision.
         */
        [[nodiscard]] const helios::engine::game::GameObject* source() const noexcept {
            return source_;
        }

        /**
         * @brief Returns the matched GameObject.
         *
         * @return Non-owning pointer to the GameObject that was collided with.
         */
        [[nodiscard]] const helios::engine::game::GameObject* match() const noexcept {
            return match_;
        }


    };

}
