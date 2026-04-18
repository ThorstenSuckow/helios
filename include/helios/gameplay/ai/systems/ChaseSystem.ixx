/**
 * @file ChaseSystem.ixx
 * @brief System for updating entity steering to chase targets.
 */
module;

#include <algorithm>

export module helios.gameplay.ai.systems.ChaseSystem;


import helios.math;

import helios.core.units.Unit;

import helios.runtime.world.GameObject;
import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;

import helios.gameplay.ai.components.ChaseComponent;
import helios.engine.modules.physics.motion.components.SteeringComponent;

import helios.gameplay.lifecycle.components.DeadTagComponent;

import helios.engine.modules.spatial.transform.components.TranslationStateComponent;

import helios.ecs.components.Active;

import helios.gameplay.lifecycle.components;

import helios.engine.common.tags.SystemRole;

using namespace helios::gameplay::lifecycle::components;
using namespace helios::ecs::components;
export namespace helios::gameplay::ai::systems {

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
    template<typename THandle>
    class ChaseSystem {


    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;
        /**
         * @brief Updates steering for all chasing entities.
         *
         * @param updateContext Context providing delta time and game world access.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, sc, cc, tsc, active] : updateContext.view<
                THandle,
                helios::engine::modules::physics::motion::components::SteeringComponent<THandle>,
                helios::gameplay::ai::components::ChaseComponent<THandle>,
                helios::engine::modules::spatial::transform::components::TranslationStateComponent<THandle>,
                helios::ecs::components::Active<THandle>
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
