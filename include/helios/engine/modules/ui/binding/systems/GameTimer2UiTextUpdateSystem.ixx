/**
 * @file GameTimer2UiTextUpdateSystem.ixx
 * @brief System for binding game timer values to UI text components.
 */
module;


#include <format>



export module helios.engine.modules.ui.binding.systems.GameTimer2UiTextUpdateSystem;

import helios.engine.mechanics.timing.TimerManager;
import helios.engine.mechanics.timing.components;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;


import helios.engine.modules.ui.widgets;
import helios.engine.modules.ui.layout;

import helios.engine.mechanics.lifecycle.components.Active;

using namespace helios::engine::mechanics::timing;

import helios.engine.common.tags.SystemRole;

export namespace helios::engine::modules::ui::binding::systems {

    /**
     * @brief System for binding game timer values to UI text components.
     *
     * Queries entities with GameTimerBindingComponent, TimeFormatterComponent,
     * and UiTextComponent. When the observed timer's revision changes, the
     * formatted time string is propagated to the text component for display.
     *
     * @see TimerManager
     * @see GameTimerBindingComponent
     * @see TimeFormatterComponent
     */
    class GameTimer2UiTextUpdateSystem {

        /**
         * @brief Reference to the TimerManager that owns the game timers.
         */
        TimerManager& timerManager_;

    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;
        /**
         * @brief Constructs the system with a reference to the TimerManager.
         *
         * @param timerManager The TimerManager providing game timer state.
         */
        explicit GameTimer2UiTextUpdateSystem(TimerManager& timerManager)
        : timerManager_(timerManager) {}



        /**
         * @brief Updates text components with current game timer values.
         *
         * For each matching entity, compares the timer revision against the
         * observer's cached revision. On mismatch, formats the elapsed/remaining
         * time via the TimeFormatterComponent and updates the UiTextComponent.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, gtc, dfc, txt, active] : updateContext.view<
                helios::engine::mechanics::timing::components::GameTimerBindingComponent,
                helios::engine::modules::ui::layout::components::TimeFormatterComponent,
                helios::engine::modules::ui::widgets::components::UiTextComponent,
                helios::engine::mechanics::lifecycle::components::Active
            >().whereEnabled()) {

                if (const auto* timer = timerManager_.gameTimer(gtc->gameTimerId());
                    timer->timerRevision() != gtc->timerRevision()) {
                    txt->setText(dfc->format(timer->elapsed(), timer->duration()));
                    gtc->setTimerRevision(timer->timerRevision());
                }

            }
        }

    };

}
