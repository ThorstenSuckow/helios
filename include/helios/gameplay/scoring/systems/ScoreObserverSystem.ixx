/**
 * @file ScoreObserverSystem.ixx
 * @brief System that synchronizes ScoreObserverComponent with ScorePools.
 */
module;


#include <format>


export module helios.gameplay.scoring.systems.ScoreObserverSystem;

import helios.gameplay.scoring.ScorePoolManager;
import helios.gameplay.scoring.commands;
import helios.gameplay.scoring.components;
import helios.gameplay.scoring.types;

import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;


import helios.physics.collision.events;

import helios.ecs.components.Active;


import helios.util.log;


import helios.engine.common.tags.SystemRole;

export namespace helios::gameplay::scoring::systems {

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
    template<typename THandle>
    class ScoreObserverSystem {

        /**
         * @brief Reference to the ScorePoolManager that owns the score pools.
         */
        ScorePoolManager& scorePoolManager_;


    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;
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
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, soc, active] : updateContext.view<
                THandle,
                helios::gameplay::scoring::components::ScoreObserverComponent<THandle>,
                helios::ecs::components::Active<THandle>
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
