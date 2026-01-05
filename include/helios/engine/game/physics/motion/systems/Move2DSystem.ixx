/**
 * @file Move2DSystem.ixx
 * @brief System for processing 2D movement physics.
 */
module;

#include <cassert>
#include <cmath>
#include <algorithm>
#include <helios/engine/game/GameObjectView.h>

export module helios.engine.game.physics.motion.systems.Move2DSystem;


import helios.engine.game.System;
import helios.math;

import helios.engine.game.GameWorld;
import helios.engine.game.physics.motion.components.Move2DComponent;
import helios.engine.game.spatial.transform.components.TranslationStateComponent;
import helios.engine.game.physics.motion.components.DirectionComponent;

import helios.engine.game.UpdateContext;

export namespace helios::engine::game::physics::motion::systems {

    /**
     * @brief System that processes 2D movement for entities.
     *
     * @details This system reads from Move2DComponent and applies physics simulation
     * including velocity integration and dampening when input is inactive.
     *
     * The system updates TranslationStateComponent with the computed translation
     * changes each frame.
     *
     * Required components:
     * - Move2DComponent (physics configuration and state)
     * - DirectionComponent (current movement direction)
     * - TranslationStateComponent (receives translation updates)
     */
    class Move2DSystem : public System {

         private:



        /**
         * @brief Computes translation delta for an entity based on its Move2DComponent state.
         *
         * @details Calculates velocity changes based on input state. When input is active,
         * accelerates in the facing direction. When inactive, applies exponential
         * drag to slow down. Velocity is clamped to maximum speed.
         *
         * @param cmp Pointer to the Move2DComponent.
         * @param currentDirection The current direction vector from DirectionComponent.
         * @param deltaTime Frame delta time in seconds.
         *
         * @return Translation delta to apply to the entity this frame.
         */
        [[nodiscard]] static helios::math::vec3f moveGameObject(
            helios::engine::game::physics::motion::components::Move2DComponent* cmp,
            helios::math::vec3f currentDirection,
            float deltaTime
        ) noexcept {

            assert(cmp != nullptr && "Unexpected invalid Move2DComponent passed");
            assert(deltaTime >= 0 && "Unexpected negative value for deltaTime");

            if (deltaTime == 0) {
                return helios::math::vec3f{0.0f};
            }

            bool isInputActive = cmp->isInputActive();


            auto velocity = cmp->velocity();

            if (!isInputActive) {
                float movementDampening = cmp->movementDampening();

                // Apply exponential drag when no input is active.
                // This creates a smooth deceleration effect (velocity approaches zero over time).
                const float drag  = std::pow(movementDampening, deltaTime);
                velocity = velocity * drag;

                if (velocity.length() <= helios::math::EPSILON_LENGTH) {
                    velocity = {0.0f, 0.0f, 0.0f};
                }

            } else {

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
         * @brief Updates movement for all applicable entities.
         *
         * @details For each entity with Move2DComponent, computes translation
         * changes and applies them to the TranslationStateComponent.
         *
         * @param updateContext Context containing deltaTime and other frame data.
         */
        void update(helios::engine::game::UpdateContext& updateContext) noexcept override {

            for (auto [entity, m2d, dc, tsc] : gameWorld_->find<
                helios::engine::game::physics::motion::components::Move2DComponent,
                helios::engine::game::physics::motion::components::DirectionComponent,
                helios::engine::game::spatial::transform::components::TranslationStateComponent
            >().each()) {

                helios::math::vec3f translationDelta;

                translationDelta = moveGameObject(m2d, dc->direction(), updateContext.deltaTime());

                tsc->translateBy(translationDelta);
            }

        }


    };

};
