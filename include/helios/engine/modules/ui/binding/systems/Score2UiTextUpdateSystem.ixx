/**
 * @file Score2UiTextUpdateSystem.ixx
 * @brief System for binding score values to UI text components.
 */
module;


#include <format>



export module helios.engine.modules.ui.binding.systems.Score2UiTextUpdateSystem;

import helios.gameplay.scoring.components;

import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;


import helios.engine.modules.ui.widgets.components.UiTextComponent;

import helios.engine.modules.ui.layout.components.NumberFormatterComponent;

import helios.ecs.components.Active;


import helios.engine.common.tags.SystemRole;

export namespace helios::engine::modules::ui::binding::systems {

    /**
     * @brief System for binding score values to UI text components.
     *
     * Queries entities with ScoreObserverComponent, NumberFormatterComponent,
     * and UiTextComponent. When the score observer signals an update, the
     * formatted total score is propagated to the text component for display.
     *
     * @see ScoreObserverComponent
     * @see NumberFormatterComponent
     */
    template<typename THandle>
    class Score2UiTextUpdateSystem {

    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;

        /**
         * @brief Updates text components with current score values.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, soc, nfc, txt, active] : updateContext.view<
                THandle,
                helios::gameplay::scoring::components::ScoreObserverComponent<THandle>,
                helios::engine::modules::ui::layout::components::NumberFormatterComponent<THandle>,
                helios::engine::modules::ui::widgets::components::UiTextComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {

                if (!soc->hasUpdate()) {
                    continue;
                }

                txt->setText(nfc->format<double>(soc->totalScore()));
            }
        }

    };

}
