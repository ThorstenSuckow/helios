# helios::engine::modules::ui::widgets

UI widget components and systems.

This namespace provides components for UI widgets such as menus, text displays, and interactive elements, along with systems that manage their state and styling.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `components/` | Widget state and styling components |
| `systems/` | Systems for navigation, styling, and bounds updates |

## Key Classes

| Class | Purpose |
|-------|---------|
| `MenuComponent` | Manages menu items and selection state |
| `UiTextComponent` | Text display with template-based value formatting |
| `UiStateComponent` | Tracks interaction states (selected, hovered, focused) |
| `UiStyleComponent` | Defines colors and scales for different states |
| `UiFocusComponent` | Tracks the currently focused UI entity |
| `UiActionComponent` | Associates actions with UI elements |
| `MenuNavigationSystem` | Handles gamepad input for menu navigation |
| `UiStyleUpdateSystem` | Applies styles based on interaction state |
| `UiTextBoundsUpdateSystem` | Updates AABB when text content changes |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::ui::widgets
@brief UI widget components and systems.
@details This namespace contains widget components that define UI element content, interaction state, and appearance, along with systems that process navigation input, apply styling, and update bounds.
</p></details>
