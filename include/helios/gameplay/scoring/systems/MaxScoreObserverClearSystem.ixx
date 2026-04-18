/**
 * @file MaxScoreObserverClearSystem.ixx
 * @brief System that clears update flags on MaxScoreObserverComponents.
 */
module;




export module helios.gameplay.scoring.systems.MaxScoreObserverClearSystem;

import helios.gameplay.scoring.components;
import helios.runtime.world.UpdateContext;


import helios.runtime.world.GameWorld;

import helios.engine.modules.physics.collision.events;

import helios.ecs.components.Active;



import helios.engine.common.tags.SystemRole;

export namespace helios::gameplay::scoring::systems {


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
    template<typename THandle>
    class MaxScoreObserverClearSystem {


    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;
        /**
         * @brief Clears update flags on all active MaxScoreObserverComponents.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, soc, active] : updateContext.view<
                THandle,
                helios::gameplay::scoring::components::MaxScoreObserverComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {

                soc->clearUpdate();

            }

        }

    };

}
