/**
 * @file ScoreObserverClearSystem.ixx
 * @brief System that clears update flags on ScoreObserverComponents.
 */
module;


#include <helios/engine/ecs/query/GameObjectView.h>

export module helios.engine.mechanics.scoring.systems.ScoreObserverClearSystem;

import helios.engine.mechanics.scoring.ScorePoolManager;

import helios.engine.mechanics.scoring.components;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.messaging.command.CommandBuffer;
import helios.engine.ecs.System;


import helios.engine.modules.physics.collision.events;



export namespace helios::engine::mechanics::scoring::systems {

    /**
     * @brief System that clears the hasUpdate flag on ScoreObserverComponents.
     *
     * Should run at the end of the frame (post-phase) after all systems
     * that need the hasUpdate flag have processed it.
     */
    class ScoreObserverClearSystem : public helios::engine::ecs::System {

    public:


        /**
         * @brief Clears update flags on all ScoreObserverComponents.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, soc] : gameWorld_->find<
                helios::engine::mechanics::scoring::components::ScoreObserverComponent
            >().each()) {

                soc->clearUpdate();

            }

        }

    };

}
