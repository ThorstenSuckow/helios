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
 * @brief Tracks the currently focused UI entity.
 *
 * Used to manage which UI element has focus for input handling.
 */
class UiFocusComponent {


    std::optional<helios::engine::ecs::GameObject> focusedEntity_ = std::nullopt;

public:

    /**
     * @brief Sets the focused entity.
     *
     * @param entity The entity to focus, or nullopt to clear focus.
     */
    void setFocusedEntity(const std::optional<helios::engine::ecs::GameObject>& entity) noexcept {
        focusedEntity_ = entity;
    }

    /**
     * @brief Returns the currently focused entity.
     *
     * @return The focused entity, or nullopt if none.
     */
    [[nodiscard]] std::optional<helios::engine::ecs::GameObject> focusedEntity() const noexcept {
        return focusedEntity_;
    }

    /**
     * @brief Clears the current focus.
     */
    void clearFocus() noexcept {
        focusedEntity_ = std::nullopt;
    }

};

} // namespace helios::engine::modules::ui::widgets::components
