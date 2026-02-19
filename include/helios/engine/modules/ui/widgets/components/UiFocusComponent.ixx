/**
 * @file UiFocusComponent.ixx
 * @brief Component for tracking menu focus state.
 */
module;

#include <optional>

export module helios.engine.modules.ui.widgets.components.UiFocusComponent;

import helios.engine.core.data.MenuId;

import helios.engine.ecs.GameObject;

export namespace helios::engine::modules::ui::widgets::components {

    /**
     * @brief Tagging struct for the UI-component that currently has the focus.
     *
     * Used to manage which UI element has focus for input handling.
     */
    struct UiFocusComponent {};

} // namespace helios::engine::modules::ui::widgets::components
