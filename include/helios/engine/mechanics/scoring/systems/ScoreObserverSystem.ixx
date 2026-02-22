/**
 * @file ScoreObserverSystem.ixx
 * @brief System that synchronizes ScoreObserverComponent with ScorePools.
 */
module;


#include <format>


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

import helios.engine.mechanics.lifecycle.components.Active;


import helios.util.log;


export namespace helios::engine::mechanics::scoring::systems {

    /**
     * @brief System that propagates score snapshots to ScoreObserverComponents.
     *
     * Each frame, queries all active entities with a ScoreObserverComponent
     * and updates them with the current ScorePoolSnapshot from the
     * ScorePoolManager. The component's revision-based change detection
     * ensures that downstream consumers (e.g. UI bindings) are only notified
     * when the score actually changes.
     *
     * @see ScoreObserverClearSystem
     * @see ScoreObserverComponent
     * @see ScorePoolManager
     */
    class ScoreObserverSystem : public helios::engine::ecs::System {

        /**
         * @brief Reference to the ScorePoolManager that owns the score pools.
         */
        ScorePoolManager& scorePoolManager_;

    public:

        /**
         * @brief Constructs the system with a reference to the ScorePoolManager.
         *
         * @param scorePoolManager The ScorePoolManager providing score pool state.
         */
        explicit ScoreObserverSystem(ScorePoolManager& scorePoolManager)
        : scorePoolManager_(scorePoolManager) {}

        /**
         * @brief Updates all active ScoreObserverComponents with current score data.
         *
         * Compares each observer's cached revision against the pool's current
         * revision. On mismatch, propagates the pool's snapshot to the component.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, soc, active] : gameWorld_->view<
                helios::engine::mechanics::scoring::components::ScoreObserverComponent,
                helios::engine::mechanics::lifecycle::components::Active
            >().whereEnabled()) {

                const auto scorePoolId = soc->scorePoolId();
                auto* scorePool = scorePoolManager_.scorePool(scorePoolId);

                if (!scorePool) {
                    continue;
                }

                if (soc->scorePoolRevision() != scorePool->revision()) {
                    soc->setScorePoolSnapshot(scorePool->snapshot());
                }

            }

        }

    };

}
