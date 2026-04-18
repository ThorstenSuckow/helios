/**
 * @file MaxScore2UiTextUpdateSystem.ixx
 * @brief System for binding high score values to UI text components.
 */
module;


#include <format>



export module helios.engine.modules.ui.binding.systems.MaxScore2UiTextUpdateSystem;

import helios.engine.mechanics.scoring.components;

import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;


import helios.engine.modules.ui.layout.components.NumberFormatterComponent;

import helios.engine.modules.ui.widgets.components.UiTextComponent;

import helios.ecs.components.Active;

using namespace helios::engine::mechanics::scoring;

import helios.engine.common.tags.SystemRole;

export namespace helios::engine::modules::ui::binding::systems {

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
    template<typename THandle>
    class MaxScore2UiTextUpdateSystem {

    public:


        using EngineRoleTag = helios::engine::common::tags::SystemRole;
        /**
         * @brief Updates text components with current high score values.
         *
         * Skips entities whose MaxScoreObserverComponent has no pending update.
         * Otherwise formats the max score via NumberFormatterComponent and
         * writes the result to UiTextComponent.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, soc, nfc, txt, active] : updateContext.view<
                THandle,
                helios::engine::mechanics::scoring::components::MaxScoreObserverComponent<THandle>,
                helios::engine::modules::ui::layout::components::NumberFormatterComponent<THandle>,
                helios::engine::modules::ui::widgets::components::UiTextComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {


                if (!soc->hasUpdate()) {
                    continue;
                }

                txt->setText(nfc->format<double>(soc->maxScore()));
            }
        }

    };

}
