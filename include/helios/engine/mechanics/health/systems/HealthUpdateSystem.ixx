/**
 * @file HealthUpdateSystem.ixx
 * @brief System that processes entity death and emits death events.
 */
module;

#include <format>


export module helios.engine.mechanics.health.systems.HealthUpdateSystem;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.messaging.command.CommandBuffer;
import helios.engine.ecs.System;

import helios.engine.mechanics.health.events.DeathEvent;

import helios.engine.mechanics.combat.types.AttackContext;
import helios.engine.mechanics.combat.components.LastAttackerComponent;

import helios.engine.mechanics.health.components.HealthComponent;
import helios.engine.runtime.spawn.commands.DespawnCommand;

import helios.engine.modules.physics.collision.events;

import helios.engine.mechanics.spawn.components.SpawnedByProfileComponent;

import helios.util.log;

#define HELIOS_LOG_SCOPE "helios::engine::mechanics::health::systems::HealthUpdateSystem"
export namespace helios::engine::mechanics::health::systems {

    /**
     * @brief System that detects dead entities and issues despawn commands.
     *
     * Queries entities with HealthComponent and SpawnedByProfileComponent.
     * When an entity's health reaches zero, issues a DespawnCommand and
     * publishes a DeathEvent with optional attack context.
     */
    class HealthUpdateSystem : public helios::engine::ecs::System {

        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE);

    public:


        /**
         * @brief Processes dead entities and emits death events.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, hc, sbp] : gameWorld_->find<
                helios::engine::mechanics::health::components::HealthComponent,
                helios::engine::mechanics::spawn::components::SpawnedByProfileComponent
            >().each()) {

                if (hc->isAlive()) {
                    continue;
                }

                logger_.info(std::format("Entity {0} killed.", entity->guid().value()));

                updateContext.commandBuffer().add<helios::engine::runtime::spawn::commands::DespawnCommand>(
                    entity->guid(),
                    sbp->spawnProfileId()
                );

                auto* lac = entity->get<helios::engine::mechanics::combat::components::LastAttackerComponent>();
                if (lac) {
                    auto attackContext = lac->lastAttackContext();
                    updateContext.pushPhase<helios::engine::mechanics::health::events::DeathEvent>(
                        entity->guid(),
                        attackContext
                    );
                } else {
                    updateContext.pushPhase<helios::engine::mechanics::health::events::DeathEvent>(
                        entity->guid()
                    );
                }

            }

        }

    };

}
