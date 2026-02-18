/**
 * @file MenuNavigationSystem.ixx
 * @brief System for handling menu navigation input.
 */
module;

#include <iostream>
#include <algorithm>
#include <cassert>

export module helios.engine.modules.ui.widgets.systems.MenuNavigationSystem;
import helios.engine.ecs;

import helios.engine.modules.ui.widgets.components.MenuComponent;

import helios.engine.mechanics.lifecycle.components.Active;

import helios.engine.modules.ui.widgets.components.UiFocusComponent;
import helios.engine.modules.ui.widgets.components.UiStateComponent;
import helios.engine.modules.ui.widgets.components.UiActionComponent;

import helios.engine.modules.ui.commands.UiActionCommand;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.mechanics.lifecycle.components.Active;

import helios.engine.runtime.messaging.command.CommandBuffer;

import helios.input.types.Gamepad;
import helios.input.gamepad.GamepadState;

import helios.engine.ecs.components.HierarchyComponent;

import helios.engine.modules.rendering.model.components.ModelAabbComponent;

export namespace helios::engine::modules::ui::widgets::systems {

    using namespace helios::input::types;
    using namespace helios::engine::ecs::components;
    using namespace helios::engine::core::data;
    using namespace helios::engine::ecs;
    using namespace helios::engine::runtime::world;
    using namespace helios::engine::modules::ui::widgets::components;
    using namespace helios::engine::mechanics::lifecycle::components;
    using namespace helios::input::gamepad;

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
    class MenuNavigationSystem : public helios::engine::ecs::System {

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
        void updateMenu(helios::engine::runtime::world::UpdateContext& updateContext, MenuComponent* mc, const size_t index, const GamepadState& gamepadState) {

            auto menuItems = mc->menuItems();

            const auto prevIndex = mc->selectedIndex();

            mc->setSelectedIndex(index);
            if (!mc->isDirty()) {
                return;
            }
            mc->clearDirty();

            auto prev = gameWorld_->find(menuItems[prevIndex]);
            prev->remove<UiFocusComponent>();

            // update ui state
            auto* usc = gameWorld_->find(menuItems[index])->get<UiStateComponent>();
            if (usc) {
                usc->setSelected(true);
                prev->add<UiFocusComponent>();
            }

            if (index != prevIndex) {
                usc = gameWorld_->find(menuItems[prevIndex])->get<UiStateComponent>();
                if (usc) {
                    usc->setSelected(false);
                }
            }


        }


    public:

        /**
         * @brief Processes menu navigation input.
         *
         * @details Finds the currently focused menu item, handles Up/Down
         * navigation, and triggers UiActionCommand when the A button is
         * pressed on an item with a UiActionComponent.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            MenuComponent* focusedMenu = nullptr;

            for (auto [entity, fc, hc, active] :  gameWorld_->view<
                UiFocusComponent, HierarchyComponent, Active
            >().whereEnabled()) {
                assert(hc->parent() && gameWorld_->find(*hc->parent()) && gameWorld_->find(*hc->parent())->get<MenuComponent>() && "Item expected to have parent menu component.");
                focusedMenu = gameWorld_->find(*hc->parent())->get<MenuComponent>();
                break;
            }

            if (!focusedMenu) {
                return;
            }

            const auto size = focusedMenu->menuItems().size();

            const auto& gamepadState = updateContext.inputSnapshot().gamepadState();

            if (gamepadState.isButtonPressed(GamepadInput::Up)) {

                if (focusedMenu->selectedIndex() >= 1) {
                    updateMenu(updateContext, focusedMenu, focusedMenu->selectedIndex() - 1, gamepadState);
                } else {
                    updateMenu(updateContext, focusedMenu, 0, gamepadState);
                }

            } else if (gamepadState.isButtonPressed(GamepadInput::Down)) {

                if (focusedMenu->selectedIndex() + 1 < size) {
                    updateMenu(updateContext, focusedMenu, focusedMenu->selectedIndex() + 1, gamepadState);
                } else {
                    updateMenu(updateContext, focusedMenu, size - 1, gamepadState);
                }

            } else {
                updateMenu(updateContext, focusedMenu, focusedMenu->selectedIndex(), gamepadState);
            }

            if (gamepadState.isButtonPressed(GamepadInput::A)) {
                auto* uac = gameWorld_->find(focusedMenu->menuItems()[focusedMenu->selectedIndex()])->get<
                    helios::engine::modules::ui::widgets::components::UiActionComponent
                >();

                if (uac) {
                    updateContext.commandBuffer().add<helios::engine::modules::ui::commands::UiActionCommand>(
                        focusedMenu->menuItems()[focusedMenu->selectedIndex()], uac->actionId()
                    );
                }
            }


        }



    };

}
