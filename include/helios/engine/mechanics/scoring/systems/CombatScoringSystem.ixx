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
import helios.engine.runtime.messaging.command.CommandBuffer;
import helios.engine.ecs.System;

import helios.engine.mechanics.health.events.DeathEvent;

import helios.engine.mechanics.combat.types.AttackContext;
import helios.engine.mechanics.combat.components.LastAttackerComponent;

import helios.engine.modules.physics.collision.events;


import helios.util.log;

#define HELIOS_LOG_SCOPE "helios::engine::mechanics::scoring::systems::CombatScoringSystem"
export namespace helios::engine::mechanics::scoring::systems {

    /**
     * @brief System that processes death events and awards scores.
     *
     * Listens for DeathEvent and checks if the killed entity has a
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

            for (auto& event : updateContext.readPhase<helios::engine::mechanics::health::events::DeathEvent>()) {

                if (!event.attackContext().has_value()) {
                    continue;
                }

                auto& attackContext = event.attackContext().value();

                const auto* enemy = updateContext.gameWorld().find(event.source());

                const auto* hitman = updateContext.gameWorld().find(attackContext.source);

                if (!enemy || ! hitman) {
                    continue;
                }

                auto* svc = enemy->get<
                    helios::engine::mechanics::scoring::components::ScoreValueComponent<
                        helios::engine::mechanics::scoring::types::KillReward>>();
                if (!svc) {
                    continue;
                }

                auto* scc = hitman->get<helios::engine::mechanics::scoring::components::ScorePoolComponent>();
                if (!scc) {
                    continue;
                }

                auto scoreContext = helios::engine::mechanics::scoring::types::ScoreValueContext{
                    .scoreTypeId = helios::engine::core::data::ScoreTypeId::id<helios::engine::mechanics::scoring::types::KillReward>(),
                    .scorePoolId = scc->scorePoolId(),
                    .value = svc->score().value()
                };

                logger_.info(
                    std::format("Entity {0} killed. Reward: {1}",
                    hitman->guid().value(),
                    svc->score().value())
                );

                updateContext.commandBuffer().add<helios::engine::mechanics::scoring::commands::UpdateScoreCommand>(
                    std::move(scoreContext)
                );
            }

        }

    };

}
