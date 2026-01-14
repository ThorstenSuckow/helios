/**
 * @file TriggerCollisionEvent.ixx
 * @brief Event dispatched when two solid colliders physically collide.
 */
module;

export module helios.engine.game.physics.collision.events.TriggerCollisionEvent;

import helios.util.Guid;
import helios.math;

export namespace helios::engine::game::physics::collision::events {

    /**
     * @brief Event representing a physical collision between two solid objects.
     *
     * @details TriggerCollisionEvent is used to communicate trigger (non-solid) collisions
     * between GameObjects. Unlike SolidCollisionEvent, this event does not indicate a
     * direct physical interaction that may require resolution such as position correction,
     * velocity reflection, or damage calculation.
     *
     * The GridCollisionDetectionSystem detects collisions and stores the result in
     * CollisionStateComponent. Downstream systems can then create and dispatch
     * TriggerCollisionEvent instances based on the collision state.
     *
     * The event carries GUIDs of both participating GameObjects and the
     * contact point in world space where the collision was recorded.
     *
     * Example usage (reading from event bus):
     * ```cpp
     * for (const auto& event : eventBus.readPass<TriggerCollisionEvent>()) {
     *     auto* sourceObj = gameWorld.find(event.source());
     *     auto* matchObj = gameWorld.find(event.match());
     *     // Handle trigger collision response...
     * }
     * ```
     *
     * @see SolidCollisionEvent
     * @see CollisionStateComponent
     * @see GridCollisionDetectionSystem
     * @see CollisionComponent
     */
    class TriggerCollisionEvent {

        /**
         * @brief GUID of the GameObject that initiated or detected the collision.
         */
        const helios::util::Guid source_;

        /**
         * @brief GUID of the GameObject that was collided with.
         */
        const helios::util::Guid match_;

        /**
         * @brief The world-space contact point of the collision.
         */
        const helios::math::vec3f contact_;

    public:

        /**
         * @brief Constructs a TriggerCollisionEvent.
         *
         * @param source GUID of the GameObject that detected or initiated the collision.
         * @param match GUID of the GameObject that was collided with.
         * @param contact The world-space position where the collision occurred.
         */
        explicit TriggerCollisionEvent(
            const helios::util::Guid source,
            const helios::util::Guid match,
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
         * @brief Returns the GUID of the source GameObject.
         *
         * @details The source is the entity that detected/reported the collision.
         * Use `GameWorld::find()` to retrieve the actual GameObject.
         *
         * @return GUID of the collision reporter.
         */
        [[nodiscard]] helios::util::Guid source() const noexcept {
            return source_;
        }

        /**
         * @brief Returns the GUID of the matched GameObject.
         *
         * @details The match is the entity that was collided with.
         * Use `GameWorld::find()` to retrieve the actual GameObject.
         *
         * @return GUID of the collided entity.
         */
        [[nodiscard]] helios::util::Guid match() const noexcept {
            return match_;
        }


    };

}
