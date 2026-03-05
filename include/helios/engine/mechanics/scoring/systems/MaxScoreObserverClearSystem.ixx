/**
 * @file MaxScoreObserverClearSystem.ixx
 * @brief System that clears update flags on MaxScoreObserverComponents.
 */
module;




export module helios.engine.mechanics.scoring.systems.MaxScoreObserverClearSystem;

import helios.engine.mechanics.scoring.components;
import helios.engine.runtime.world.UpdateContext;


import helios.engine.runtime.world.GameWorld;

import helios.engine.modules.physics.collision.events;

import helios.engine.mechanics.lifecycle.components.Active;



import helios.engine.common.tags.SystemTag;

export namespace helios::engine::mechanics::scoring::systems {


    /**
     * @brief System that resets the hasUpdate flag on all MaxScoreObserverComponents.
     *
     * Scheduled after the UI binding systems have consumed the update.
     * This ensures that MaxScoreObserverComponent::hasUpdate() returns true
     * for exactly one frame after a high score change.
     *
     * @see MaxScoreObserverSystem
     * @see MaxScoreObserverComponent
     */
    class MaxScoreObserverClearSystem {


    public:

        using EngineRoleTag = helios::engine::common::tags::SystemTag;
        /**
         * @brief Clears update flags on all active MaxScoreObserverComponents.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, soc, active] : updateContext.view<
                helios::engine::mechanics::scoring::components::MaxScoreObserverComponent,
                helios::engine::mechanics::lifecycle::components::Active
            >().whereEnabled()) {

                soc->clearUpdate();

            }

        }

    };

}
