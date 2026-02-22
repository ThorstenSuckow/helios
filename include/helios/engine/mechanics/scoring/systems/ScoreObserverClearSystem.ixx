/**
 * @file ScoreObserverClearSystem.ixx
 * @brief System that clears update flags on ScoreObserverComponents.
 */
module;




export module helios.engine.mechanics.scoring.systems.ScoreObserverClearSystem;

import helios.engine.mechanics.scoring.ScorePoolManager;

import helios.engine.mechanics.scoring.components;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.messaging.command.CommandBuffer;
import helios.engine.ecs.System;


import helios.engine.modules.physics.collision.events;

import helios.engine.mechanics.lifecycle.components.Active;



export namespace helios::engine::mechanics::scoring::systems {

    /**
     * @brief System that resets the hasUpdate flag on all ScoreObserverComponents.
     *
     * Scheduled after the UI binding systems have consumed the update.
     * This ensures that ScoreObserverComponent::hasUpdate() returns true
     * for exactly one frame after a score change.
     *
     * @see ScoreObserverSystem
     * @see ScoreObserverComponent
     */
    class ScoreObserverClearSystem : public helios::engine::ecs::System {

    public:


        /**
         * @brief Clears update flags on all ScoreObserverComponents.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, soc, active] : gameWorld_->view<
                helios::engine::mechanics::scoring::components::ScoreObserverComponent,
                helios::engine::mechanics::lifecycle::components::Active
            >().whereEnabled()) {

                soc->clearUpdate();

            }

        }

    };

}
