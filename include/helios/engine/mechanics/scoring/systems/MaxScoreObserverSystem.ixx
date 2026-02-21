/**
 * @file MaxScoreObserverSystem.ixx
 * @brief System that updates MaxScoreObserverComponents from ScorePool data.
 */
module;


#include <format>


export module helios.engine.mechanics.scoring.systems.MaxScoreObserverSystem;

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

import helios.engine.mechanics.lifecycle.components.Active;


import helios.util.log;


export namespace helios::engine::mechanics::scoring::systems {


    /**
     * @brief System that propagates high score snapshots to MaxScoreObserverComponents.
     *
     * Each frame, queries all active entities with a MaxScoreObserverComponent
     * and updates them with the current MaxScorePoolSnapshot from the
     * ScorePoolManager. The component's revision-based change detection
     * ensures that downstream consumers (e.g. UI bindings) are only notified
     * when the high score actually changes.
     *
     * @see MaxScoreObserverClearSystem
     * @see MaxScoreObserverComponent
     * @see ScorePoolManager
     */
    class MaxScoreObserverSystem : public helios::engine::ecs::System {

    public:

        /**
         * @brief Updates all active MaxScoreObserverComponents with current high score data.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, soc, active] : gameWorld_->view<
                helios::engine::mechanics::scoring::components::MaxScoreObserverComponent,
                helios::engine::mechanics::lifecycle::components::Active
            >().whereEnabled()) {

                auto* scorePoolManager = gameWorld_->getManager<helios::engine::mechanics::scoring::ScorePoolManager>();

                if (!scorePoolManager) {
                    continue;
                }

                const auto scorePoolId = soc->scorePoolId();
                auto* scorePool = scorePoolManager->scorePool(scorePoolId);

                if (!scorePool) {
                    continue;
                }

                soc->update(scorePool->maxScoreSnapshot());

            }

        }

    };

}
