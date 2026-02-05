/**
 * @file ScoreObserverSystem.ixx
 * @brief System that synchronizes ScoreObserverComponent with ScorePools.
 */
module;


#include <format>
#include <helios/engine/ecs/query/GameObjectView.h>

export module helios.engine.mechanics.scoring.systems.ScoreObserverSystem;

import helios.engine.mechanics.scoring.ScorePoolManager;
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

#define HELIOS_LOG_SCOPE "helios::engine::mechanics::scoring::systems::ScoreObserverSystem"
export namespace helios::engine::mechanics::scoring::systems {

    /**
     * @brief System that updates ScoreObserverComponents from their referenced ScorePools.
     *
     * Each frame, queries all entities with ScoreObserverComponent and updates
     * their cached score value from the corresponding ScorePool.
     */
    class ScoreObserverSystem : public helios::engine::ecs::System {

        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE);

    public:


        /**
         * @brief Updates observer components with current pool values.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, soc] : gameWorld_->find<
                helios::engine::mechanics::scoring::components::ScoreObserverComponent
            >().each()) {

                auto* scorePoolManager = gameWorld_->getManager<helios::engine::mechanics::scoring::ScorePoolManager>();

                if (!scorePoolManager) {
                    continue;
                }

                const auto scorePoolId = soc->scorePoolId();
                auto* scorePool = scorePoolManager->scorePool(scorePoolId);

                if (!scorePool) {
                    continue;
                }

                soc->update(scorePool->snapshot());

            }

        }

    };

}
