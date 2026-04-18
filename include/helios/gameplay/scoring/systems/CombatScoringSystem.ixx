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


export module helios.gameplay.scoring.systems.CombatScoringSystem;

import helios.gameplay.scoring.commands;
import helios.gameplay.scoring.components;
import helios.gameplay.scoring.types;

import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;
import helios.runtime.messaging.command.NullCommandBuffer;
import helios.runtime.messaging.command.concepts.IsCommandBufferLike;

import helios.state.Bindings;



import helios.ecs.types.TypeDefs;

import helios.gameplay.health.events.HealthDepletedEvent;


import helios.physics.collision.events;


import helios.util.log;


using namespace helios::gameplay::scoring::components;
using namespace helios::gameplay::scoring::types;
using namespace helios::gameplay::scoring::commands;
using namespace helios::gameplay::health::events;
using namespace helios::ecs::types;
using namespace helios::runtime::world;
using namespace helios::runtime::messaging::command;
using namespace helios::runtime::concepts;

#define HELIOS_LOG_SCOPE "helios::gameplay::scoring::systems::CombatScoringSystem"
import helios.runtime.world.tags.SystemRole;

export namespace helios::gameplay::scoring::systems {

    /**
     * @brief System that processes death events and awards scores.
     *
     * Listens for HealthDepletedEvent and checks if the killed entity has a
     * ScoreValueComponent. If so, issues an UpdateScoreCommand to
     * credit the attacker's score pool.
     */
    template<typename THandle, typename TCommandBuffer = NullCommandBuffer>
    requires IsCommandBufferLike<TCommandBuffer>
    class CombatScoringSystem {

        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE);

    public:

        using EngineRoleTag = helios::runtime::tags::SystemRole;


        /**
         * @brief Processes death events and issues score commands.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto& event : updateContext.readPass<HealthDepletedEvent<THandle>>()) {

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

                updateContext.queueCommand<TCommandBuffer, UpdateScoreCommand>(
                    std::move(scoreContext)
                );
            }

        }

    };

}
