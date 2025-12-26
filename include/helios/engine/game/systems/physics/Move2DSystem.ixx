/**
 * @file Move2DSystem.ixx
 * @brief System for processing 2D movement and rotation physics.
 */
module;

#include <cassert>
#include <cmath>
#include <algorithm>

export module helios.engine.game.systems.physics.Move2DSystem;


import helios.engine.game.System;
import helios.math;

import helios.engine.game.GameWorld;
import helios.engine.game.components.physics.Move2DComponent;
import helios.engine.game.components.physics.TransformComponent;

import helios.engine.game.UpdateContext;

export namespace helios::engine::game::systems::physics {

    /**
     * @brief System that processes 2D movement and rotation for entities.
     *
     * @details This system reads from Move2DComponent and applies physics simulation
     * including rotation towards target angle, velocity integration, and
     * dampening when input is inactive.
     *
     * The system updates TransformComponent with the computed rotation and
     * translation changes each frame.
     *
     * Required components:
     * - Move2DComponent (physics configuration and state)
     * - TransformComponent (receives transform updates)
     */
    class Move2DSystem : public System {

    public:

        /**
         * @brief Called when the system is added to a GameWorld.
         *
         * @param gameWorld Pointer to the GameWorld this system belongs to.
         */
        void onAdd(helios::engine::game::GameWorld* gameWorld) noexcept override {
            System::onAdd(gameWorld);
        }

        /**
         * @brief Updates movement and rotation for all applicable entities.
         *
         * @details For each entity with Move2DComponent, computes rotation and translation
         * changes and applies them to the TransformComponent.
         *
         * @param updateContext Context containing deltaTime and other frame data.
         */
        void update(helios::engine::game::UpdateContext& updateContext) noexcept override {

            gameWorld_->find<
                helios::engine::game::components::physics::Move2DComponent,
                helios::engine::game::components::physics::TransformComponent
            >().each([&](auto* obj, auto& m2d, auto& tc) {

                auto rotation = rotateGameObject(&m2d, updateContext.deltaTime());
                auto translationDelta = moveGameObject(&m2d, updateContext.deltaTime());

                tc.setLocalRotation(rotation);
                tc.setLocalTranslation(tc.localTranslation() + translationDelta);
            });
        }

    private:

        /**
         * @brief Computes rotation for an entity based on its Move2DComponent state.
         *
         * @details Calculates the rotation matrix based on current rotation angle, applying
         * dampening when input is inactive. Rotation speed decreases exponentially
         * until the threshold is reached.
         *
         * @param cmp Reference to the Move2DComponent.
         * @param deltaTime Frame delta time in seconds.
         *
         * @return Rotation matrix to apply to the entity.
         */
        [[nodiscard]] helios::math::mat4f rotateGameObject(helios::engine::game::components::physics::Move2DComponent* cmp, float deltaTime) noexcept {

            assert(cmp != nullptr && "Unexpected invalid Move2DComponent passed");
            assert(deltaTime >= 0 && "Unexpected negative value for deltaTime");

            if (deltaTime == 0) {
                return helios::math::mat4f::identity();
            }

            float currentRotationSpeed = cmp->currentRotationSpeed();
            float currentRotationAngle = cmp->currentRotationAngle();
            float rotationAngleDelta   = cmp->rotationAngleDelta();
            bool isInputActive = cmp->isInputActive();

            // Apply rotation dampening when input is inactive
            if (!isInputActive) {
                currentRotationSpeed  *= std::pow(cmp->rotationDampening(), deltaTime);

                if (currentRotationSpeed < cmp->rotationSpeedThreshold()) {
                    currentRotationSpeed = 0.0f;
                }

                cmp->setCurrentRotationSpeed(currentRotationSpeed);
            }

            // Apply rotation step towards target angle
            if (currentRotationSpeed > 0.0f) {
                float rotationStep = std::copysign(
                 std::min(
                     std::abs(rotationAngleDelta),
                     currentRotationSpeed * deltaTime
                ),
                 rotationAngleDelta
                );
                currentRotationAngle = currentRotationAngle + rotationStep;
                rotationAngleDelta = std::fmod((cmp->targetRotationAngle() - currentRotationAngle) + 540.0f, 360.0f) - 180.0f;

                // Snap to target when close enough
                if (std::abs(rotationAngleDelta) <= 0.5f) {
                    currentRotationAngle = cmp->targetRotationAngle();
                    rotationAngleDelta   = 0.0f;
                    currentRotationSpeed = 0.0f;
                }

                cmp->setCurrentRotationAngle(currentRotationAngle);
                cmp->setRotationAngleDelta(rotationAngleDelta);
                cmp->setCurrentRotationSpeed(currentRotationSpeed);
            }

            return helios::math::rotate(
                helios::math::mat4f::identity(),
                helios::math::radians(currentRotationAngle),
                helios::math::vec3f(0.0f, 0.0f, 1.0f)
            );

        };

        /**
         * @brief Computes translation delta for an entity based on its Move2DComponent state.
         *
         * @details Calculates velocity changes based on input state. When input is active,
         * accelerates in the facing direction. When inactive, applies exponential
         * drag to slow down. Velocity is clamped to maximum speed.
         *
         * @param cmp Reference to the Move2DComponent.
         * @param deltaTime Frame delta time in seconds.
         *
         * @return Translation delta to apply to the entity this frame.
         */
        [[nodiscard]] helios::math::vec3f moveGameObject(helios::engine::game::components::physics::Move2DComponent* cmp, float deltaTime) {

            assert(cmp != nullptr && "Unexpected invalid Move2DComponent passed");
            assert(deltaTime >= 0 && "Unexpected negative value for deltaTime");

            if (deltaTime == 0) {
                return helios::math::vec3f{0.0f};
            }

            float currentRotationAngle = cmp->currentRotationAngle();
            bool isInputActive = cmp->isInputActive();


            auto velocity = cmp->velocity();

            if (!isInputActive) {
                float movementDampening = cmp->movementDampening();

                // Apply exponential drag when no input is active.
                // This creates a smooth deceleration effect (velocity approaches zero over time).
                const float drag  = std::pow(movementDampening, deltaTime);
                velocity = velocity * drag;

                if (drag <= helios::math::EPSILON_LENGTH) {
                    velocity = helios::math::vec3f{0.0f, 0.0f, 0.0f};
                }

            } else {
                // Compute the current facing direction from the actual rotation angle.
                // This converts the angle to a unit vector in the XY plane.
                const auto currentDirection = helios::math::vec3f(
                    cos(helios::math::radians(currentRotationAngle)), sin(helios::math::radians(currentRotationAngle)), 0.0f
                );

                // Accelerate in the current facing direction.
                // Uses throttle (input intensity) to scale acceleration.
                velocity = velocity +  currentDirection *  (cmp->movementAcceleration() * cmp->throttle() * deltaTime);
            }

            // Clamp velocity to maximum speed to prevent unlimited acceleration.
            if (velocity.length() > cmp->movementSpeed()) {
                velocity = velocity.normalize() * cmp->movementSpeed();
            }

            cmp->setVelocity(velocity);

            return velocity * deltaTime;
        }

    };

};
