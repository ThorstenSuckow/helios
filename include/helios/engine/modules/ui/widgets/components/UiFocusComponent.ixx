/**
 * @file UiFocusComponent.ixx
 * @brief Component for tracking menu focus state.
 */
module;

#include <optional>

export module helios.engine.modules.ui.widgets.components.UiFocusComponent;

import helios.engine.modules.ui.widgets.types.MenuId;

import helios.runtime.world.GameObject;

export namespace helios::engine::modules::ui::widgets::components {

    /**
     * @brief Tagging struct for the UI-component that currently has the focus.
     *
     * Used to manage which UI element has focus for input handling.
     */
    template<typename THandle>
    struct UiFocusComponent {};

} // namespace helios::engine::modules::ui::widgets::components
