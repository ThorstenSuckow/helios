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


export module helios.ui.widgets.systems.MenuDisplaySystem;

import helios.ecs;
import helios.runtime.world;
import helios.ui.widgets.types.MenuId;

import helios.ui.widgets.components.UiFocusComponent;

import helios.ui.widgets.components.MenuComponent;

import helios.state.CombinedStateToIdMapPair;

import helios.gameplay.gamestate.types;
import helios.gameplay.matchstate.types;

import helios.rendering.model.components.ModelAabbComponent;
import helios.math;

import helios.engine.common.tags.SystemRole;

using namespace helios::ui::widgets::types;
using namespace helios::state;
using namespace helios::gameplay::gamestate::types;
using namespace helios::gameplay::matchstate::types;
using namespace helios::ecs::types;
using namespace helios::ui::widgets::components;
using namespace helios::runtime::world;
using namespace helios::ecs;
export namespace helios::ui::widgets::systems {

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
    template<typename THandle, typename StateLft, typename StateRgt>
    class MenuDisplaySystem {


        /**
         * @brief Previously active menu IDs for change detection.
         */
        std::vector<MenuId> prevMenuIds_;

        /**
         * @brief Policy mapping states to menu IDs.
         */
        CombinedStateToIdMapPair<StateLft, StateRgt, MenuId> stateToMenuMap_;

        /**
         * @brief Cache for inactive focused items to be cleaned up.
         */
        std::vector<THandle> inactiveItems_;

        /**
         * @brief Shows or hides a menu by ID.
         *
         * @param menuId The menu to show/hide.
         * @param show True to show, false to hide.
         * @param components View of all MenuComponent entities.
         */
        void showMenu(const MenuId menuId, const bool show, View<THandle, MenuComponent<THandle>>& components) {
            for (auto [entity, mc] : components) {
                if (mc->menuId() == menuId) {
                    entity.setActive(show);
                }
            }
        }

    protected:
        /**
         * @brief Focuses the first item of a menu.
         *
         * @param menuId The menu to focus.
         * @param components View of all MenuComponent entities.
         */
        void focusMenu(
            helios::runtime::world::UpdateContext& updateContext,
            const MenuId menuId, View<THandle, MenuComponent<THandle>>& components) {
            for (auto [entity, mc] : components) {
                if (mc->menuId() == menuId) {
                    mc->selectDefaultIndex();

                    if (mc->menuItems().size() > mc->selectedIndex()) {
                        auto menuItem = updateContext.find(mc->menuItems()[mc->selectedIndex()]);
                        menuItem->getOrAdd<UiFocusComponent>();
                    }
                    break;
                }
            }
        }


    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;


        /**
         * @brief Constructs the system with a state-to-menu policy.
         *
         * @param stateToMenuMap Policy mapping game/match states to menu IDs.
         */
        explicit MenuDisplaySystem(CombinedStateToIdMapPair<StateLft, StateRgt, MenuId> stateToMenuMap)
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
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {


            auto& session = updateContext.session();

            auto gameState = session.state<StateLft>();
            auto matchState = session.state<StateRgt>();

            auto menuIds = stateToMenuMap_.ids(gameState, matchState);

            if (menuIds.empty() && prevMenuIds_.empty()) {
                return;
            }

            View<THandle, MenuComponent<THandle>> components = updateContext.view<THandle, MenuComponent<THandle>>().whereEnabled();


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

            for (auto [entity, fc] :  updateContext.view<THandle, UiFocusComponent<THandle>>().whereEnabled()) {

                /**
                 * @todo Check if item is child element of menu?
                 */
                if (entity.isActive()) {
                    hasFocus = true;
                    break;
                }

                /**
                 * @todo guaranteee that the UiFocusComponent is exclusive,
                 * break here if first comp with focus was found.
                 */
                inactiveItems_.push_back(entity.handle());
            }

            for (auto& handle : inactiveItems_) {
                if (auto entity = updateContext.find(handle)) {
                    entity->remove<UiFocusComponent>();
                }
            }

            // focus an item from the first menu
            if (!hasFocus) {
                focusMenu(updateContext, menuIds[0], components);
            }

        }

    };

}
