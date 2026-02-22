
module;

export module helios.engine.mechanics.timing.systems.GameTimerObserverClearSystem;


import helios.engine.mechanics.timing.components;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.ecs.System;

import helios.engine.mechanics.lifecycle.components.Active;



export namespace helios::engine::mechanics::timing::systems {


    class GameTimerObserverClearSystem : public helios::engine::ecs::System {

    public:


        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, soc, active] : gameWorld_->view<
                helios::engine::mechanics::timing::components::GameTimerObserverComponent,
                helios::engine::mechanics::lifecycle::components::Active
            >().whereEnabled()) {

                soc->clearUpdate();

            }

        }

    };

}
