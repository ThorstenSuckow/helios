/**
 * @file MenuDisplaySystem.ixx
 * @brief System for controlling menu visibility based on UI focus.
 */
module;

#include <iostream>
#include <algorithm>

export module helios.engine.modules.ui.widgets.systems.MenuDisplaySystem;
import helios.engine.ecs.System;

import helios.engine.modules.ui.widgets.components.MenuComponent;


import helios.engine.modules.ui.widgets.components.UiStateComponent;
import helios.engine.modules.ui.widgets.components.UiActionComponent;

import helios.engine.modules.ui.commands.UiActionCommand;

import helios.engine.ecs.GameObject;
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.mechanics.lifecycle.components.Active;

import helios.engine.ecs.EntityHandle;

import helios.engine.runtime.messaging.command.CommandBuffer;

import helios.input.types.Gamepad;

import helios.engine.modules.rendering.model.components.ModelAabbComponent;
import helios.math;
export namespace helios::engine::modules::ui::widgets::systems {

    using namespace helios::input::types;

    /**
     * @brief Controls menu visibility based on session UI focus state.
     *
     * @details This system monitors the session's focused entity and shows/hides
     * menus accordingly. When the focused entity changes:
     * - The previously focused menu (if any) is hidden
     * - The newly focused menu is shown along with its menu items
     *
     * The system tracks the previous focused entity to detect focus changes
     * and avoid redundant visibility updates.
     *
     * @see MenuComponent
     * @see UiFocusComponent
     * @see Session::focusedEntity()
     */
    class MenuDisplaySystem : public helios::engine::ecs::System {

        /**
         * @brief Handle to the previously focused entity for change detection.
         */
        helios::engine::ecs::EntityHandle prevEntityHandle_;
        
        /**
         * @brief Shows or hides a menu and its items.
         *
         * @param go The menu entity.
         * @param show True to show, false to hide.
         * @param gameWorld Reference to the game world for item lookup.
         */
        void showMenu(
            helios::engine::ecs::GameObject go,
            bool show,
            helios::engine::runtime::world::GameWorld& gameWorld
        ) {
            auto* mc = go.get<helios::engine::modules::ui::widgets::components::MenuComponent>();

            if (!mc) {
                return;
            }

            go.setActive(show);
            for (auto& item : mc->menuItems()) {
                gameWorld.find(item)->setActive(show);
            }

        } 

        
        
    public:

        /**
         * @brief Updates menu visibility based on focus changes.
         *
         * @details Compares the current focused entity with the previous one.
         * If focus changed, hides the previous menu and shows the new one.
         * Skips processing if focus remains unchanged.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {


            auto focusedEntity = updateContext.gameWorld().session().focusedEntity();

            if (focusedEntity && focusedEntity->entityHandle() == prevEntityHandle_) {
                return;
            }

            if (prevEntityHandle_.isValid()) {
                auto go = updateContext.gameWorld().find(prevEntityHandle_);
                if (go) {
                    showMenu(go.value(), false,updateContext.gameWorld());
                }
                prevEntityHandle_ = helios::engine::ecs::EntityHandle();
            }

            if (!focusedEntity) {
                return;
            }

            prevEntityHandle_ = focusedEntity->entityHandle();


            auto* mc = focusedEntity->get<helios::engine::modules::ui::widgets::components::MenuComponent>();

            if (!mc) {
                return;
            }

            showMenu(focusedEntity.value(), true, updateContext.gameWorld());

        }

    };

}
