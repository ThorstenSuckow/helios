/**
 * @file GameTimer2UiTextUpdateSystem.ixx
 * @brief System for binding game timer values to UI text components.
 */
module;


#include <format>



export module helios.ui.binding.systems.GameTimer2UiTextUpdateSystem;

import helios.gameplay.timing.TimerManager;
import helios.gameplay.timing.components;

import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;


import helios.ui.widgets;
import helios.ui.layout;

import helios.ecs.components.Active;

using namespace helios::gameplay::timing;

import helios.runtime.world.tags.SystemRole;

export namespace helios::ui::binding::systems {

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
    template<typename THandle>
    class GameTimer2UiTextUpdateSystem {

        /**
         * @brief Reference to the TimerManager that owns the game timers.
         */
        TimerManager& timerManager_;

    public:

        using EngineRoleTag = helios::runtime::tags::SystemRole;
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
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, gtc, dfc, txt, active] : updateContext.view<
                THandle,
                helios::gameplay::timing::components::GameTimerBindingComponent<THandle>,
                helios::ui::layout::components::TimeFormatterComponent<THandle>,
                helios::ui::widgets::components::UiTextComponent<THandle>,
                helios::ecs::components::Active<THandle>
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
