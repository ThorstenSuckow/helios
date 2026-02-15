/**
 * @file FocusMenuListener.ixx
 * @brief Listener that focuses a menu on game state transitions.
 */
module;


#include <optional>
#include <ranges>

export module helios.engine.mechanics.gamestate.listeners.FocusMenuListener;

import helios.engine.runtime.world.UpdateContext;

import helios.engine.mechanics.gamestate.types;
import helios.engine.mechanics.gamestate.GameStateTransitionListener;

import helios.engine.modules.ui.widgets.components.MenuComponent;

import helios.engine.core.data;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;

export namespace helios::engine::mechanics::gamestate::listeners {

    /**
     * @brief Focuses a specific menu when entering the Paused state.
     *
     * On transition to Paused, finds the menu with the configured MenuId
     * and sets it as the focused entity. On exit from Paused, clears
     * the focused entity.
     *
     * @see MenuComponent
     */
    class FocusMenuListener : public GameStateTransitionListener {

        const helios::engine::core::data::MenuId menuId_;

        const size_t selectedIndex_;

    public:

        /**
         * @brief Constructs a listener for the specified menu.
         *
         * @param menuId The ID of the menu to focus.
         * @param selectedIndex The index to select when focusing (default: 0).
         */
        explicit FocusMenuListener(
            const helios::engine::core::data::MenuId menuId, const size_t selectedIndex = 0
        ) : menuId_{menuId}, selectedIndex_ {selectedIndex} {};

        void onGameStateEnter(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const GameState to
        ) noexcept {


        }

        void onGameStateExit(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const GameState from
        ) noexcept {

            if (from == GameState::Paused) {
                updateContext.gameWorld().session().setFocusedEntity(std::nullopt);
            }
        }

        void onGameStateTransition(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const GameStateTransitionContext transitionCtx
        ) noexcept override {

            const auto from = transitionCtx.from();
            const auto to = transitionCtx.to();
            const auto transition = transitionCtx.transitionId();

            if (to == GameState::Paused) {
                for (auto [entity, mc] : updateContext.gameWorld().view<
                    helios::engine::modules::ui::widgets::components::MenuComponent
                >().whereEnabled()) {
                    if (mc->menuId() == menuId_) {
                        mc->setSelectedIndex(selectedIndex_);
                        updateContext.gameWorld().session().setFocusedEntity(entity);
                    }
                    break;
                }

            }
        }




    };

}

