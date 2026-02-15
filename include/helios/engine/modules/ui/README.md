# helios::engine::modules::ui

User interface components and systems for game entities.

This module provides components and systems for creating and managing UI elements within the helios engine. It supports viewport-relative positioning, text rendering, menus, and data binding between game state and UI components.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `binding/` | Systems for binding game data to UI components |
| `commands/` | UI action commands and dispatchers |
| `layout/` | Layout primitives such as anchors and alignment |
| `transform/` | UI-specific transform components and positioning systems |
| `widgets/` | UI widget components (menus, text, etc.) and related systems |

## Key Classes

| Class | Purpose |
|-------|---------|
| `Anchor` | Enum defining anchor points for UI positioning |
| `UiTransformComponent` | Viewport-relative positioning with anchor and margin support |
| `UiTransformSystem` | Computes UI element positions based on viewport and anchor |
| `MenuComponent` | Manages menu items and selection state |
| `UiTextComponent` | Text display with template-based formatting |
| `UiStateComponent` | Tracks interaction states (selected, hovered, focused) |
| `UiStyleComponent` | Defines colors and scales for different states |
| `UiFocusComponent` | Tracks the currently focused UI entity |
| `UiActionComponent` | Associates actions with UI elements |
| `UiActionCommand` | Command for triggering UI actions |
| `UiActionCommandManager` | Processes UI action commands with policy-based dispatch |
| `MenuNavigationSystem` | Handles gamepad input for menu navigation |
| `UiStyleUpdateSystem` | Applies styles based on interaction state |
| `UiTextBoundsUpdateSystem` | Updates AABB bounds when text content changes |
| `Score2UiTextUpdateSystem` | Binds score data to UI text components |

## Architecture

The UI module follows the ECS pattern used throughout helios:

1. **Components** store UI-specific state (anchors, offsets, text content, interaction state)
2. **Systems** process components each frame to update positions, styling, and content
3. **Commands** handle UI actions (button presses, menu selections)
4. **Bindings** connect game state (e.g., scores) to UI widgets

UI elements are positioned relative to viewports using anchors and offsets, allowing for resolution-independent layouts.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::ui
@brief User interface components and systems for game entities.
@details This namespace provides a lightweight UI framework built on the ECS architecture. It supports viewport-relative positioning through anchors and offsets, menus with navigation and actions, text rendering with template-based formatting, and data binding between game state and UI components.
</p></details>
