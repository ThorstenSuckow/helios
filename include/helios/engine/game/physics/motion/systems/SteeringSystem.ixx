/**
 * @file SteeringSystem.ixx
 * @brief System for updating entity heading and rotation.
 */
module;

#include <helios/engine/ecs/query/GameObjectView.h>
#include <algorithm>
#include <cassert>
#include <cmath>

export module helios.engine.game.physics.motion.systems.SteeringSystem;


import helios.engine.ecs.System;
import helios.math;

import helios.engine.ecs.GameWorld;
import helios.engine.game.physics.motion.components.SteeringComponent;
import helios.engine.game.spatial.transform.components.TransformComponent;
import helios.engine.game.physics.motion.components.DirectionComponent;
import helios.engine.game.physics.motion.components.RotationStateComponent;

import helios.engine.ecs.UpdateContext;

export namespace helios::engine::game::physics::motion::systems {

    /**
     * @brief System that processes heading and rotation physics.
     *
     * @details
     * This system updates the rotation state of entities based on their SteeringComponent.
     * It calculates the target rotation from input, applies rotation speed and dampening,
     * and updates the RotationStateComponent and DirectionComponent.
     */
    class SteeringSystem : public helios::engine::ecs::System {

        private:

        /**
         * @brief Performs the rotation physics update for a SteeringComponent.
         *
         * @details Applies dampening when input is inactive, calculates rotation steps,
         * and snaps to target when within threshold.
         *
         * @param cmp The SteeringComponent to update. Must not be nullptr.
         * @param deltaTime Time elapsed since last frame in seconds.
         */
        void updateHeading(
            helios::engine::game::physics::motion::components::SteeringComponent* cmp,
            float deltaTime
        ) noexcept {

            assert(cmp != nullptr && "Unexpected invalid SteeringComponent passed");
            assert(deltaTime >= 0 && "Unexpected negative value for deltaTime");

            if (deltaTime == 0) {
                return;
            }

            float currentRotationSpeed = cmp->currentRotationSpeed();
            float currentRotationAngle = cmp->currentRotationAngle();
            float rotationAngleDelta   = cmp->rotationAngleDelta();
            bool  headingStateChanged  = cmp->stateChanged();

            // Apply rotation dampening when input is inactive
            if (!headingStateChanged) {
                currentRotationSpeed  *= std::pow(cmp->rotationDampening(), deltaTime);

                if (currentRotationSpeed < cmp->rotationSpeedThreshold()) {
                    currentRotationSpeed = 0.0f;
                }

                cmp->setCurrentRotationSpeed(currentRotationSpeed);
            }

            // Apply rotation step towards target angle
            if (currentRotationSpeed > 0.0f) {

                if (cmp->useInstantRotation()) {
                    currentRotationAngle = cmp->targetRotationAngle();

                } else {
                    float rotationStep = std::copysign(
                     std::min(
                         std::abs(rotationAngleDelta),
                         currentRotationSpeed * deltaTime
                    ),
                     rotationAngleDelta
                    );
                    currentRotationAngle = currentRotationAngle + rotationStep;
                }

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

        }

    public:


        /**
         * @brief Updates the heading of entities.
         *
         * @param updateContext Context containing frame timing and game state.
         */
        void update(helios::engine::ecs::UpdateContext& updateContext) noexcept override {


            for (auto [entity, hc, rsc, dc] : gameWorld_->find<
                helios::engine::game::physics::motion::components::SteeringComponent,
                helios::engine::game::physics::motion::components::RotationStateComponent,
                helios::engine::game::physics::motion::components::DirectionComponent
            >().each()) {


                if (hc->stateChanged()) {
                    auto direction                = hc->steeringInput();
                    auto targetRotationAngle = helios::math::degrees(std::atan2(direction[1], direction[0]));
                    auto rotationAngleDelta  = std::fmod((targetRotationAngle - hc->currentRotationAngle()) + 540.0f, 360.0f) - 180.0f;

                    float turnBoostFactor = 0.5f;
                    float turnBoost = 1.0f + turnBoostFactor * std::clamp((abs(rotationAngleDelta))/180.f, 0.0f, 1.0f);
                    float currentRotationSpeed = turnBoost * hc->rotationSpeed() * hc->turnIntensity();

                    hc->setTargetRotationAngle(targetRotationAngle);
                    hc->setRotationAngleDelta(rotationAngleDelta);
                    hc->setCurrentRotationSpeed(currentRotationSpeed);

                }


                updateHeading(hc, updateContext.deltaTime());

                float rotationAngle = hc->currentRotationAngle();
                rsc->setHeadingRotationAngle(rotationAngle);
                rsc->setHeadingRotationAxis(hc->rotationAxis());

                float rad = helios::math::radians(rotationAngle);
                dc->setDirection(helios::math::vec3f{cos(rad), sin(rad), 0.0f});


            }

        }

    };

};
