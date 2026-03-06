/**
 * @file ChaseSystem.ixx
 * @brief System for updating entity steering to chase targets.
 */
module;

#include <algorithm>

export module helios.engine.modules.ai.systems.ChaseSystem;


import helios.math;

import helios.core.units.Unit;

import helios.engine.ecs.GameObject;
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.modules.ai.components.ChaseComponent;
import helios.engine.modules.physics.motion.components.SteeringComponent;

import helios.engine.modules.spatial.transform.components.TranslationStateComponent;

import helios.engine.mechanics.lifecycle.components.Active;

import helios.engine.mechanics.lifecycle.components;

using namespace helios::engine::mechanics::lifecycle::components;

import helios.engine.common.tags.SystemRole;

export namespace helios::engine::modules::ai::systems {

    /**
     * @brief System that steers entities towards their chase targets.
     *
     * Processes all entities with ChaseComponent, SteeringComponent, and
     * TranslationStateComponent. Updates steering intent to face the target
     * entity based on a cooldown timer.
     *
     * The system:
     * 1. Finds the target entity by handle
     * 2. Waits for the cooldown timer to expire
     * 3. Calculates direction from entity to target
     * 4. Sets steering intent to face that direction
     */
    class ChaseSystem {


    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;
        /**
         * @brief Updates steering for all chasing entities.
         *
         * @param updateContext Context providing delta time and game world access.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, sc, cc, tsc, active] : updateContext.view<
                helios::engine::modules::physics::motion::components::SteeringComponent,
                helios::engine::modules::ai::components::ChaseComponent,
                helios::engine::modules::spatial::transform::components::TranslationStateComponent,
                helios::engine::mechanics::lifecycle::components::Active
            >().whereEnabled()) {

                const auto entityHandle = cc->target();

                auto go = updateContext.find(entityHandle);

                if (!go || !go->isActive() || go->has<DeadTagComponent>()) {
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
