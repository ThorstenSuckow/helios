/**
 * @file Score2UiTextUpdateSystem.ixx
 * @brief System for binding score values to UI text components.
 */
module;


#include <format>

#include <helios/engine/ecs/query/GameObjectView.h>

export module helios.engine.modules.ui.binding.systems.Score2UiTextUpdateSystem;

import helios.engine.mechanics.scoring.components;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.ecs.System;

import helios.engine.modules.ui.widgets.components.UiTextComponent;


export namespace helios::engine::modules::ui::binding::systems {

    /**
     * @brief System for binding score values to UI text components.
     *
     * Queries entities with both ScoreObserverComponent and UiTextComponent.
     * When the score observer signals an update, the new total score is
     * propagated to the text component for display.
     */
    class Score2UiTextUpdateSystem : public helios::engine::ecs::System {

    public:


        /**
         * @brief Updates text components with current score values.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, soc, txt] : gameWorld_->find<
                helios::engine::mechanics::scoring::components::ScoreObserverComponent,
                helios::engine::modules::ui::widgets::components::UiTextComponent
            >().each()) {

                if (soc->hasUpdate()) {
                    txt->setDouble(soc->totalScore());
                }
            }
        }

    };

}
