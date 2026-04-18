/**
 * @file ScoreObserverClearSystem.ixx
 * @brief System that clears update flags on ScoreObserverComponents.
 */
module;




export module helios.engine.mechanics.scoring.systems.ScoreObserverClearSystem;

import helios.engine.mechanics.scoring.ScorePoolManager;

import helios.engine.mechanics.scoring.components;

import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;



import helios.engine.modules.physics.collision.events;

import helios.ecs.components.Active;



import helios.engine.common.tags.SystemRole;

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
    template<typename THandle>
    class ScoreObserverClearSystem {


    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;

        /**
         * @brief Clears update flags on all ScoreObserverComponents.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, soc, active] : updateContext.view<
                THandle,
                helios::engine::mechanics::scoring::components::ScoreObserverComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {

                soc->clearUpdate();

            }

        }

    };

}
