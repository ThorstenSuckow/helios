/**
 * @file MenuNavigationSystem.ixx
 * @brief System for handling menu navigation input.
 */
module;

#include <iostream>
#include <algorithm>
#include <cassert>

export module helios.ui.widgets.systems.MenuNavigationSystem;
import helios.ecs;

import helios.ui.widgets.components.MenuComponent;

import helios.ecs.components.Active;

import helios.ui.widgets.types;
import helios.ui.widgets.components.UiFocusComponent;
import helios.ui.widgets.components.UiStateComponent;
import helios.ui.widgets.components.UiActionComponent;

import helios.ui.widgets.commands.UiActionCommand;

import helios.runtime.world;
import helios.runtime.messaging.command.NullCommandBuffer;
import helios.runtime.messaging.command.concepts.IsCommandBufferLike;

import helios.ecs.components.Active;

import helios.state.Bindings;


import helios.input.types.Gamepad;
import helios.input.gamepad.GamepadState;

import helios.ecs.components.HierarchyComponent;

import helios.rendering.model.components.ModelAabbComponent;

using namespace helios::input::types;
using namespace helios::ecs::components;
using namespace helios::ui::widgets::types;
using namespace helios::ecs::types;
using namespace helios::runtime::world;
using namespace helios::runtime::messaging::command;
using namespace helios::runtime::concepts;
using namespace helios::ui::widgets::components;
using namespace helios::ecs::components;
using namespace helios::input::gamepad;

import helios.runtime.world.tags.SystemRole;

export namespace helios::ui::widgets::systems {

    /**
     * @brief Handles gamepad input for menu navigation.
     *
     * @details Processes Up/Down input to change selection and A button
     * to activate the selected menu item. Updates UiStateComponent
     * selection state and issues UiActionCommand on activation.
     *
     * ## Input Handling
     *
     * - **Up/Down**: Navigate through menu items
     * - **A Button**: Activate selected item (triggers UiActionCommand)
     *
     * @see MenuComponent
     * @see MenuDisplaySystem
     * @see UiActionCommand
     */
    template<typename THandle, typename TCommandBuffer = NullCommandBuffer>
    requires IsCommandBufferLike<TCommandBuffer>
    class MenuNavigationSystem {


        /**
         * @brief Updates menu selection state.
         *
         * @details Moves focus to the new index, updates UiStateComponent
         * selection flags, and clears the dirty flag.
         *
         * @param updateContext The current frame's update context.
         * @param mc The menu component to update.
         * @param index The new selected index.
         * @param gamepadState The current gamepad state.
         */
        void updateMenu(
            helios::runtime::world::UpdateContext& updateContext,
            MenuComponent<THandle>* mc, const size_t index) {

            auto menuItems = mc->menuItems();

            const auto prevIndex = mc->previousSelectedIndex();

            if (prevIndex == index && !mc->isDirty()) {
                return;
            }

            UiStateComponent* usc = nullptr;

            if (index != prevIndex) {
                usc = updateContext.find(menuItems[prevIndex])->get<UiStateComponent>();
                if (usc) {
                    usc->setSelected(false);
                }
            }
            mc->setSelectedIndex(index);

            // update ui state
            usc = updateContext.find(menuItems[index])->get<UiStateComponent>();
            if (usc) {
                usc->setSelected(true);
            }

            mc->clearDirty();
        }


    public:

        using EngineRoleTag = helios::runtime::tags::SystemRole;


        /**
         * @brief Processes menu navigation input.
         *
         * @details Finds the currently focused menu item, handles Up/Down
         * navigation, and triggers UiActionCommand when the A button is
         * pressed on an item with a UiActionComponent.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            MenuComponent<THandle>* focusedMenu = nullptr;

            for (auto [entity, fc, hc, active] :  updateContext.view<
                THandle,
                UiFocusComponent<THandle>, HierarchyComponent<THandle>, Active<THandle>
            >().whereEnabled()) {
                assert(hc->parent() && updateContext.find(*hc->parent()) && updateContext.find(*hc->parent())->get<MenuComponent<THandle>>() && "Item expected to have parent menu component.");
                focusedMenu = updateContext.find(*hc->parent())->get<MenuComponent<THandle>>();
                break;
            }

            if (!focusedMenu) {
                return;
            }

            const auto size = focusedMenu->menuItems().size();

            const auto& gamepadState = updateContext.inputSnapshot().gamepadState();

            if (gamepadState.isButtonPressed(GamepadInput::Up)) {

                if (focusedMenu->selectedIndex() >= 1) {
                    updateMenu(updateContext, focusedMenu, focusedMenu->selectedIndex() - 1);
                } else {
                    updateMenu(updateContext, focusedMenu, 0);
                }

            } else if (gamepadState.isButtonPressed(GamepadInput::Down)) {

                if (focusedMenu->selectedIndex() + 1 < size) {
                    updateMenu(updateContext, focusedMenu, focusedMenu->selectedIndex() + 1);
                } else {
                    updateMenu(updateContext, focusedMenu, size - 1);
                }

            } else {
                updateMenu(updateContext, focusedMenu, focusedMenu->selectedIndex());
            }

            if (gamepadState.isButtonPressed(GamepadInput::A)) {
                auto* uac = updateContext.find(focusedMenu->menuItems()[focusedMenu->selectedIndex()])->get<
                    helios::ui::widgets::components::UiActionComponent<THandle>
                >();

                if (uac) {
                    updateContext.queueCommand<TCommandBuffer, helios::ui::widgets::commands::UiActionCommand>(
                        focusedMenu->menuItems()[focusedMenu->selectedIndex()], uac->actionId()
                    );
                }
            }
        }



    };

}
