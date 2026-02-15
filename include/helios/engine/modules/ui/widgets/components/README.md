# helios::engine::modules::ui::widgets::components

UI widget state components.

This namespace contains components that store the state, content, and styling of UI widgets.

## Components

| Component | Description |
|-----------|-------------|
| `MenuComponent` | Manages a collection of menu items and tracks selection |
| `UiTextComponent` | Text display with template-based formatting |
| `UiStateComponent` | Tracks interaction states (selected, hovered, focused, pressed) |
| `UiStyleComponent` | Defines colors and scales for normal, selected, and disabled states |
| `UiFocusComponent` | Tracks the currently focused UI entity |
| `UiActionComponent` | Associates an action ID with a UI element |

## MenuComponent

Manages a list of menu item entity handles and tracks the currently selected index.

## UiTextComponent

Wraps a `TextRenderable` and provides template-based value display. Supports dirty tracking to minimize updates.

## UiStateComponent

Tracks common UI interaction states: selected, hovered, focused, enabled, and pressed.

## UiStyleComponent

Defines visual properties for different states, including colors (normal, selected, disabled) and scale factors.

## UiFocusComponent

Stores an optional reference to the currently focused entity for input handling.

## UiActionComponent

Associates an `ActionId` with a UI element that can be triggered when the element is activated.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::ui::widgets::components
@brief UI widget state components.
@details Contains components for UI widgets including menus, text, interaction state, styling, focus tracking, and action binding.
</p></details>
