/**
 * @file MenuDisplaySystem.ixx
 * @brief System for controlling menu visibility based on game state.
 */
module;

#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <helios/helios_config.h>


export module helios.engine.modules.ui.widgets.systems.MenuDisplaySystem;

import helios.engine.ecs;
import helios.engine.runtime.world;
import helios.engine.core.data;

import helios.engine.modules.ui.widgets.components.UiFocusComponent;

import helios.engine.modules.ui.widgets.components.MenuComponent;

import helios.engine.state.CombinedStateToIdMapPair;

import helios.engine.mechanics.gamestate.types;
import helios.engine.mechanics.match.types;

import helios.engine.modules.rendering.model.components.ModelAabbComponent;
import helios.math;


export namespace helios::engine::modules::ui::widgets::systems {


    using namespace helios::engine::core::data;
    using namespace helios::engine::state;
    using namespace helios::engine::mechanics::gamestate::types;
    using namespace helios::engine::mechanics::match::types;
    using namespace helios::engine::ecs;
    using namespace helios::engine::modules::ui::widgets::components;

    /**
     * @brief Controls menu visibility based on game and match state.
     *
     * @details Uses a CombinedStateToIdMapPair to determine which menus
     * should be active for the current state combination. Automatically
     * shows/hides menus and manages focus when states change.
     *
     * ## Behavior
     *
     * - Menus not associated with the current state are hidden
     * - Menus associated with the current state are shown
     * - When no item has focus, the first menu's default item is focused
     * - Inactive focused items have their UiFocusComponent removed
     *
     * @see CombinedStateToIdMapPair
     * @see MenuComponent
     * @see MenuNavigationSystem
     */
    class MenuDisplaySystem : public helios::engine::ecs::System {

        /**
         * @brief Previously active menu IDs for change detection.
         */
        std::vector<MenuId> prevMenuIds_;

        /**
         * @brief Policy mapping states to menu IDs.
         */
        CombinedStateToIdMapPair<GameState, MatchState, MenuId> stateToMenuMap_;

        /**
         * @brief Cache for inactive focused items to be cleaned up.
         */
        std::vector<GameObject> inactiveItems_;

        /**
         * @brief Shows or hides a menu by ID.
         *
         * @param menuId The menu to show/hide.
         * @param show True to show, false to hide.
         * @param components View of all MenuComponent entities.
         */
        void showMenu(const MenuId menuId, const bool show, View<MenuComponent>& components) {
            for (auto [entity, mc] : components) {
                if (mc->menuId() == menuId) {
                    entity.setActive(show);
                }
            }
        }

        /**
         * @brief Focuses the first item of a menu.
         *
         * @param menuId The menu to focus.
         * @param components View of all MenuComponent entities.
         */
        void focusMenu(const MenuId menuId, View<MenuComponent>& components) {
            for (auto [entity, mc] : components) {
                if (mc->menuId() == menuId) {
                    mc->selectDefaultIndex();

                    if (mc->menuItems().size() > mc->selectedIndex()) {
                        auto menuItem = gameWorld_->find(mc->menuItems()[mc->selectedIndex()]);
                        menuItem->getOrAdd<UiFocusComponent>();
                    }

                    break;
                }
            }
        }
        
        
    public:

        /**
         * @brief Constructs the system with a state-to-menu policy.
         *
         * @param stateToMenuMap Policy mapping game/match states to menu IDs.
         */
        explicit MenuDisplaySystem(CombinedStateToIdMapPair<GameState, MatchState, MenuId> stateToMenuMap)
        : stateToMenuMap_(std::move(stateToMenuMap)) {

            inactiveItems_.reserve(INACTIVE_FOCUSED_ITEMS_CACHE_CAPACITY);
        }

        /**
         * @brief Updates menu visibility based on current state.
         *
         * @details Queries the current game and match state, then:
         * 1. Hides menus that are no longer active
         * 2. Shows menus for the current state
         * 3. Ensures at least one menu item has focus
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {


            auto& session = updateContext.session();

            auto gameState = session.gameState();
            auto matchState = session.matchState();

            auto menuIds = stateToMenuMap_.ids(gameState, matchState);

            if (menuIds.empty() && prevMenuIds_.empty()) {
                return;
            }

            View<MenuComponent> components = gameWorld_->view<MenuComponent>().whereEnabled();


            for (auto& prevMenuId : prevMenuIds_) {
                if (std::ranges::find(menuIds, prevMenuId) == menuIds.end()) {
                    showMenu(prevMenuId, false, components);
                }
            }

            prevMenuIds_.assign(menuIds.begin(), menuIds.end());
            for (auto& menuId : menuIds) {
                showMenu(menuId, true,components);
            }

            if (menuIds.empty()) {
                return;
            }


            bool hasFocus = false;
            inactiveItems_.clear();

            for (auto [entity, fc] :  gameWorld_->view<UiFocusComponent>().whereEnabled()) {

                /**
                 * @todo Check if item is child element of menu?
                 */
                if (entity.isActive()) {
                    hasFocus = true;
                    break;
                }

                inactiveItems_.push_back(entity);
            }

            for (auto& item : inactiveItems_) {
                item.remove<UiFocusComponent>();
            }

            // focus an item from the first menu
            if (!hasFocus) {
                focusMenu(menuIds[0], components);
            }

        }

    };

}
