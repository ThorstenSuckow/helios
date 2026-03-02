/**
 * @file CombatScoringSystem.ixx
 * @brief System that awards scores based on combat events.
 */
module;

#include <algorithm>
#include <cassert>
#include <cmath>
#include <format>
#include <helios/helios_config.h>
#include <unordered_set>
#include <vector>


export module helios.engine.mechanics.scoring.systems.CombatScoringSystem;

import helios.engine.mechanics.scoring.commands;
import helios.engine.mechanics.scoring.components;
import helios.engine.mechanics.scoring.types;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.state.Bindings;
import helios.engine.runtime.messaging.command.EngineCommandBuffer;

import helios.engine.ecs.System;
import helios.engine.core.data;

import helios.engine.mechanics.health.events.HealthDepletedEvent;


import helios.engine.modules.physics.collision.events;


import helios.util.log;


using namespace helios::engine::mechanics::scoring::components;
using namespace helios::engine::mechanics::scoring::types;
using namespace helios::engine::mechanics::scoring::commands;
using namespace helios::engine::mechanics::health::events;
using namespace helios::engine::core::data;

#define HELIOS_LOG_SCOPE "helios::engine::mechanics::scoring::systems::CombatScoringSystem"
export namespace helios::engine::mechanics::scoring::systems {

    /**
     * @brief System that processes death events and awards scores.
     *
     * Listens for HealthDepletedEvent and checks if the killed entity has a
     * ScoreValueComponent. If so, issues an UpdateScoreCommand to
     * credit the attacker's score pool.
     */
    class CombatScoringSystem : public helios::engine::ecs::System {

        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE);

    public:


        /**
         * @brief Processes death events and issues score commands.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto& event : updateContext.readPass<HealthDepletedEvent>()) {

                if (!event.damageContext()) {
                    continue;
                }

                const auto& attackContext = event.damageContext();

                const auto interactionContext = attackContext->interactionContext;

                const auto enemy = updateContext.find(event.source());
                const auto instigator = updateContext.find(interactionContext.instigator);

                if (!enemy || !instigator) {
                    continue;
                }

                auto* svc = enemy->get<ScoreValueComponent<KillReward>>();
                if (!svc) {
                    continue;
                }

                auto* scc = instigator->get<ScorePoolComponent>();
                if (!scc) {
                    continue;
                }

                auto scoreContext = ScoreValueContext{
                    .scoreTypeId = ScoreTypeId::id<KillReward>(),
                    .scorePoolId = scc->scorePoolId(),
                    .value = svc->score().value()
                };

                logger_.info(
                    std::format("Entity {0} killed. Reward: {1}",
                    instigator->entityHandle().entityId,
                    svc->score().value())
                );

                updateContext.commandBuffer().add<UpdateScoreCommand>(
                    std::move(scoreContext)
                );
            }

        }

    };

}
