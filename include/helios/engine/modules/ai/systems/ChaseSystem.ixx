/**
 * @file ChaseSystem.ixx
 * @brief System for updating entity steering to chase targets.
 */
module;

#include <helios/engine/ecs/query/GameObjectView.h>

export module helios.engine.modules.ai.systems.ChaseSystem;

import helios.engine.ecs.System;
import helios.math;

import helios.core.units.Unit;

import helios.engine.ecs.GameObject;
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.modules.ai.components.ChaseComponent;
import helios.engine.modules.physics.motion.components.SteeringComponent;

import helios.engine.modules.spatial.transform.components.TranslationStateComponent;

export namespace helios::engine::modules::ai::systems {

    /**
     * @brief System that steers entities towards their chase targets.
     *
     * Processes all entities with ChaseComponent, SteeringComponent, and
     * TranslationStateComponent. Updates steering intent to face the target
     * entity based on a cooldown timer.
     *
     * The system:
     * 1. Finds the target entity by GUID
     * 2. Waits for the cooldown timer to expire
     * 3. Calculates direction from entity to target
     * 4. Sets steering intent to face that direction
     */
    class ChaseSystem : public helios::engine::ecs::System {

    public:

        /**
         * @brief Updates steering for all chasing entities.
         *
         * @param updateContext Context providing delta time and game world access.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, sc, cc, tsc] : gameWorld_->find<
                helios::engine::modules::physics::motion::components::SteeringComponent,
                helios::engine::modules::ai::components::ChaseComponent,
                helios::engine::modules::spatial::transform::components::TranslationStateComponent
            >().each()) {

                const auto guid = cc->target();

                const auto* go = gameWorld_->find(guid);

                if (!go) {
                    continue;
                }

                const auto* ttr = go->get<helios::engine::modules::spatial::transform::components::TranslationStateComponent>();
                if (!ttr) {
                    continue;
                }

                cc->updateCooldownTimerBy(updateContext.deltaTime());

                if (cc->cooldownTimer() < cc->cooldown()) {
                    continue;
                }

                cc->setCooldownTimer(0.0f);

                const auto translation = tsc->translation();
                const auto direction = (ttr->translation() - translation).normalize();

                if (std::abs(direction.length() - 1.0f)  > helios::math::EPSILON_LENGTH) {
                    continue;
                }

                sc->setSteeringIntent(direction, 1.0f);

            }
        }

    };

}
