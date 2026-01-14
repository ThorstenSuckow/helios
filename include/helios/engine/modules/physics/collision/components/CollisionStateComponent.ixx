/**
 * @file CollisionStateComponent.ixx
 * @brief Per-frame collision state component written by detection systems.
 */
module;

#include <format>
#include <iostream>
#include <optional>

export module helios.engine.game.physics.collision.components.CollisionStateComponent;

import helios.engine.game.physics.collision.types.CollisionBehavior;

import helios.engine.ecs.CloneableComponent;
import helios.engine.ecs.GameObject;
import helios.util.Guid;
import helios.math;

export namespace helios::engine::game::physics::collision::components {

    /**
     * @class CollisionStateComponent
     * @brief Stores per-frame collision state for a GameObject.
     *
     * @details This component is written by the collision detection system each frame
     * to record whether a collision occurred and provide details about the collision.
     * Game logic can query this component to react to collisions.
     *
     * The state should be reset at the start of each frame by the collision system or
     * when the owning GameObject is acquired from a pool.
     */
    class CollisionStateComponent : public helios::engine::ecs::CloneableComponent<CollisionStateComponent> {

        /**
         * @brief World-space contact point of the collision.
         */
        helios::math::vec3f contact_;

        /**
         * @brief Whether this collision is a solid (physical) collision.
         */
        bool isSolid_ = false;

        /**
         * @brief Whether this collision is a trigger (non-physical) collision.
         */
        bool isTrigger_ = false;

        /**
         * @brief Whether this entity is the collision reporter.
         */
        bool isCollisionReporter_ = false;

        /**
         * @brief GUID of the other entity involved in the collision.
         */
        std::optional<helios::util::Guid> other_;

        /**
         * @brief The collision behavior to apply.
         */
        helios::engine::game::physics::collision::types::CollisionBehavior collisionBehavior_;

        /**
         * @brief Flag indicating whether a collision was detected this frame.
         */
        bool hasCollision_ = false;

    public:

        /**
         * @brief Default constructor.
         */
        explicit CollisionStateComponent() {}

        /**
         * @brief Copy constructor (creates empty state).
         *
         * @param other The component to copy from (state is not copied).
         */
        explicit CollisionStateComponent(const CollisionStateComponent& other) {}

        /**
         * @brief Sets the collision state for this frame.
         *
         * @details This method is called by the collision detection system when a
         * collision is detected. Only the first collision per frame is recorded.
         *
         * @param contact World-space contact point.
         * @param isSolid Whether this is a solid collision.
         * @param isTrigger Whether this is a trigger collision.
         * @param collisionBehavior The behavior to apply for this collision.
         * @param isCollisionReporter Whether this entity reports the collision.
         * @param other GUID of the other entity (optional).
         *
         * @return True if the state was set, false if a collision was already recorded.
         */
        bool setState(
            helios::math::vec3f contact,
            const bool isSolid,
            const bool isTrigger,
            const helios::engine::game::physics::collision::types::CollisionBehavior collisionBehavior,
            const bool isCollisionReporter,
            std::optional<helios::util::Guid> other = std::nullopt) {

            if (hasCollision_) {
                return false;
            }

            hasCollision_ = true;
            isSolid_   = isSolid;
            isTrigger_ = isTrigger;

            collisionBehavior_ = collisionBehavior;

            isCollisionReporter_ = isCollisionReporter;

            other_ = other;
            contact_ = contact;

            return true;
        }

        /**
         * @brief Resets the collision state for a new frame.
         *
         * @details Clears all collision flags and data. Called at the start of
         * each frame or when the owning GameObject is acquired from a pool.
         */
        void reset() noexcept {
            hasCollision_ = false;
            isSolid_   = false;
            isTrigger_ = false;

            collisionBehavior_ = types::CollisionBehavior::None;

            isCollisionReporter_ = false;

            other_ = std::nullopt;
            contact_ = helios::math::vec3f{0.0f, 0.0f, 0.0f};
        }

        /**
         * @brief Called when the owning GameObject is acquired from a pool.
         *
         * @details Resets all collision state to prepare for a new lifecycle.
         */
        void onAcquire() noexcept override {
            reset();
        }

        /**
         * @brief Checks whether a collision was detected this frame.
         *
         * @return True if a collision occurred, false otherwise.
         */
        [[nodiscard]] bool hasCollision() const noexcept {
            return hasCollision_;
        }

        /**
         * @brief Returns the GUID of the other entity involved in the collision.
         *
         * @return Optional GUID of the other entity, or nullopt if not set.
         */
        [[nodiscard]] std::optional<helios::util::Guid> other() const noexcept {
            return other_;
        }

        /**
         * @brief Checks whether this entity is the collision reporter.
         *
         * @return True if this entity reported the collision.
         */
        [[nodiscard]] bool isCollisionReporter() const noexcept {
            return isCollisionReporter_;
        }

        /**
         * @brief Checks whether this is a solid (physical) collision.
         *
         * @return True if the collision is solid.
         */
        [[nodiscard]] bool isSolid() const noexcept {
            return isSolid_;
        }

        /**
         * @brief Checks whether this is a trigger (non-physical) collision.
         *
         * @return True if the collision is a trigger.
         */
        [[nodiscard]] bool isTrigger() const noexcept {
            return isTrigger_;
        }

        /**
         * @brief Returns the collision behavior for this collision.
         *
         * @return The CollisionBehavior enum value.
         */
        [[nodiscard]] helios::engine::game::physics::collision::types::CollisionBehavior collisionBehavior() const noexcept {
            return collisionBehavior_;
        }

        /**
         * @brief Returns the world-space contact point of the collision.
         *
         * @return The contact point as a 3D vector.
         */
        [[nodiscard]] helios::math::vec3f contact() const noexcept {
            return contact_;
        }

    };


}