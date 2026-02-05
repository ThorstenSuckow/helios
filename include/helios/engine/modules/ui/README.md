# helios::engine::modules::ui

User interface components and systems for game entities.

This module provides components and systems for creating and managing UI elements within the helios engine. It supports viewport-relative positioning, text rendering, and data binding between game state and UI components.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `binding/` | Systems for binding game data to UI components |
| `layout/` | Layout primitives such as anchors and alignment |
| `transform/` | UI-specific transform components and positioning systems |
| `widgets/` | UI widget components (text, etc.) and related systems |

## Key Classes

| Class | Purpose |
|-------|---------|
| `Anchor` | Enum defining anchor points for UI positioning |
| `UiTransformComponent` | Viewport-relative positioning with anchor and margin support |
| `UiTransformSystem` | Computes UI element positions based on viewport and anchor |
| `UiTextComponent` | Text display with template-based formatting |
| `UiTextBoundsUpdateSystem` | Updates AABB bounds when text content changes |
| `Score2UiTextUpdateSystem` | Binds score data to UI text components |

## Architecture

The UI module follows the ECS pattern used throughout helios:

1. **Components** store UI-specific state (anchors, margins, text content)
2. **Systems** process components each frame to update positions and content
3. **Bindings** connect game state (e.g., scores) to UI widgets

UI elements are positioned relative to viewports using anchors and margins, allowing for resolution-independent layouts.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::ui
@brief User interface components and systems for game entities.
@details This namespace provides a lightweight UI framework built on the ECS architecture. It supports viewport-relative positioning through anchors and margins, text rendering with template-based formatting, and data binding between game state and UI components.
</p></details>
