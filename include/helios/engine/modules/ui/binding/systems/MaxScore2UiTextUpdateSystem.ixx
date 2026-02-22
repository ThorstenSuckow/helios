/**
 * @file MaxScore2UiTextUpdateSystem.ixx
 * @brief System for binding high score values to UI text components.
 */
module;


#include <format>



export module helios.engine.modules.ui.binding.systems.MaxScore2UiTextUpdateSystem;

import helios.engine.mechanics.scoring.components;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.ecs.System;

import helios.engine.modules.ui.layout.components.NumberFormatterComponent;

import helios.engine.modules.ui.widgets.components.UiTextComponent;

import helios.engine.mechanics.lifecycle.components.Active;


export namespace helios::engine::modules::ui::binding::systems {

    using namespace helios::engine::mechanics::scoring;

    /**
     * @brief System for binding high score values to UI text components.
     *
     * Queries entities with MaxScoreObserverComponent, NumberFormatterComponent,
     * and UiTextComponent. When the max score observer signals an update, the
     * formatted high score is propagated to the text component for display.
     *
     * @see MaxScoreObserverComponent
     * @see NumberFormatterComponent
     */
    class MaxScore2UiTextUpdateSystem : public helios::engine::ecs::System {


    public:


        /**
         * @brief Updates text components with current high score values.
         *
         * Skips entities whose MaxScoreObserverComponent has no pending update.
         * Otherwise formats the max score via NumberFormatterComponent and
         * writes the result to UiTextComponent.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, soc, nfc, txt, active] : gameWorld_->view<
                helios::engine::mechanics::scoring::components::MaxScoreObserverComponent,
                helios::engine::modules::ui::layout::components::NumberFormatterComponent,
                helios::engine::modules::ui::widgets::components::UiTextComponent,
                helios::engine::mechanics::lifecycle::components::Active
            >().whereEnabled()) {


                if (!soc->hasUpdate()) {
                    continue;
                }

                txt->setText(nfc->format(soc->maxScore()));
            }
        }

    };

}
