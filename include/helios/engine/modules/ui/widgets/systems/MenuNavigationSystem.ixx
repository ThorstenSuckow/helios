/**
 * @file MenuNavigationSystem.ixx
 * @brief System for handling menu navigation input.
 */
module;

#include <iostream>
#include <algorithm>

export module helios.engine.modules.ui.widgets.systems.MenuNavigationSystem;
import helios.engine.ecs.System;

import helios.engine.modules.ui.widgets.components.MenuComponent;


import helios.engine.modules.ui.widgets.components.UiStateComponent;
import helios.engine.modules.ui.widgets.components.UiActionComponent;

import helios.engine.modules.ui.commands.UiActionCommand;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.mechanics.lifecycle.components.Active;

import helios.engine.runtime.messaging.command.CommandBuffer;

import helios.input.types.Gamepad;

import helios.engine.modules.rendering.model.components.ModelAabbComponent;

export namespace helios::engine::modules::ui::widgets::systems {

    using namespace helios::input::types;

    /**
     * @brief Handles gamepad input for menu navigation.
     *
     * Processes Up/Down input to change selection and A button to activate
     * the selected menu item. Updates UiStateComponent selection state
     * and issues UiActionCommand on activation.
     */
    class MenuNavigationSystem : public helios::engine::ecs::System {


    public:

        /**
         * @brief Processes menu navigation input.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {


            auto focusedEntity = updateContext.gameWorld().session().focusedEntity();

            if (!focusedEntity || !focusedEntity->isActive()) {
                return;
            }

            auto* mc = focusedEntity->get<helios::engine::modules::ui::widgets::components::MenuComponent>();

            if (!mc || mc->menuItems().empty()) {
                return;
            }

            auto menuItems = mc->menuItems();

            auto size = menuItems.size();

            auto gamepadState = updateContext.inputSnapshot().gamepadState();

            if (gamepadState.isButtonPressed(GamepadInput::Up)) {
                if (mc->selectedIndex() >= 1) {
                    mc->setSelectedIndex(mc->selectedIndex() - 1);
                } else {
                    mc->setSelectedIndex(0);
                }
            }

            if (gamepadState.isButtonPressed(GamepadInput::Down)) {
                if (mc->selectedIndex() + 1 < size) {
                    mc->setSelectedIndex(mc->selectedIndex() + 1);
                } else {
                    mc->setSelectedIndex(size - 1);
                }
            }

            auto* usc = gameWorld_->find(menuItems[mc->selectedIndex()])->get<helios::engine::modules::ui::widgets::components::UiStateComponent>();
            if (usc) {
                usc->setSelected(true);
            }

            if (mc->previousSelectedIndex() != mc->selectedIndex()) {
                usc = gameWorld_->find(menuItems[mc->previousSelectedIndex()])->get<helios::engine::modules::ui::widgets::components::UiStateComponent>();
                if (usc) {
                    usc->setSelected(false);
                }

                mc->setPreviousSelectedIndex(mc->selectedIndex());
            }

            
            if (gamepadState.isButtonPressed(GamepadInput::A)) {
                
                auto* uac = gameWorld_->find(menuItems[mc->selectedIndex()])->get<
                    helios::engine::modules::ui::widgets::components::UiActionComponent
                >();

                if (uac) {
                    updateContext.commandBuffer().add<helios::engine::modules::ui::commands::UiActionCommand>(
                        menuItems[mc->selectedIndex()], uac->actionId()
                    );
                }

            }
            
        }

    };

}
