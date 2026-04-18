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


import helios.engine.modules.physics.collision.events;

import helios.ecs.components.Active;


import helios.util.log;


import helios.engine.common.tags.SystemRole;

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
    template<typename THandle>
    class MaxScoreObserverSystem {

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
        explicit MaxScoreObserverSystem(ScorePoolManager& scorePoolManager)
        : scorePoolManager_(scorePoolManager) {}

        /**
         * @brief Updates all active MaxScoreObserverComponents with current high score data.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, soc, active] : updateContext.view<
                THandle,
                helios::engine::mechanics::scoring::components::MaxScoreObserverComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {


                const auto scorePoolId = soc->scorePoolId();
                auto* scorePool = scorePoolManager_.scorePool(scorePoolId);

                if (!scorePool) {
                    continue;
                }

                if (soc->scorePoolRevision() != scorePool->revision()) {
                    soc->setMaxScorePoolSnapshot(scorePool->maxScoreSnapshot());
                }


            }

        }

    };

}
