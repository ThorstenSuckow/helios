/**
 * @file Move2DSystem.ixx
 * @brief System for processing 2D movement physics.
 */
module;


#include <algorithm>
#include <cassert>
#include <cmath>

export module helios.physics.motion.systems.Move2DSystem;



import helios.math;

import helios.runtime.world.GameWorld;
import helios.physics.motion.components.Move2DComponent;
import helios.spatial.transform.components.TranslationStateComponent;
import helios.physics.motion.components.DirectionComponent;

import helios.runtime.world.UpdateContext;

import helios.ecs.components.Active;

import helios.runtime.world.tags.SystemRole;

export namespace helios::physics::motion::systems {

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
    template<typename THandle>
    class Move2DSystem {

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
            helios::physics::motion::components::Move2DComponent<THandle>* cmp,
            helios::math::vec3f currentDirection,
            float deltaTime
        ) noexcept {

            assert(cmp != nullptr && "Unexpected invalid Move2DComponent passed");
            assert(deltaTime >= 0 && "Unexpected negative value for deltaTime");

            if (deltaTime == 0) {
                return helios::math::vec3f{0.0f};
            }

            bool movementStateChanged = cmp->throttle() > helios::math::EPSILON_LENGTH;


            auto velocity = cmp->velocity();

            if (!movementStateChanged) {
                float movementDampening = cmp->movementDampening();

                // Apply exponential drag when no input is active.
                // This creates a smooth deceleration effect (velocity approaches zero over time).
                const float drag  = std::pow(movementDampening, deltaTime);
                velocity = velocity * drag;

                if (velocity.length() <= helios::math::EPSILON_LENGTH) {
                    velocity = {0.0f, 0.0f, 0.0f};
                }

            } else {

                if (cmp->useInstantAcceleration()) {
                    velocity = currentDirection * cmp->movementSpeed() * cmp->throttle();
                } else {
                    // Accelerate in the current facing direction.
                    // Uses throttle (input intensity) to scale acceleration.
                    velocity = velocity + (currentDirection  *  (cmp->movementAcceleration() * cmp->throttle() * deltaTime));
                }

            }

            // Clamp velocity to maximum speed to prevent unlimited acceleration.
            if (velocity.length() > cmp->movementSpeed()) {
                velocity = velocity.normalize() * cmp->movementSpeed();
            }

            cmp->setVelocity(velocity);

            return (velocity + cmp->inheritedVelocity()) * deltaTime;
        }


    public:

        using EngineRoleTag = helios::runtime::tags::SystemRole;
        /**
         * @brief Updates movement for all applicable entities.
         *
         * @details For each entity with Move2DComponent, computes translation
         * changes and applies them to the TranslationStateComponent.
         *
         * @param updateContext Context containing deltaTime and other frame data.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, m2d, dc, tsc, active] : updateContext.view<
                THandle,
                helios::physics::motion::components::Move2DComponent<THandle>,
                helios::physics::motion::components::DirectionComponent<THandle>,
                helios::spatial::transform::components::TranslationStateComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {

                helios::math::vec3f translationDelta;

                translationDelta = moveGameObject(m2d, dc->direction(), updateContext.deltaTime());

                tsc->translateBy(translationDelta);
            }

        }


    };

};
