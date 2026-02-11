# helios::engine::modules::ui::transform::components

UI transform state components.

This namespace contains components for storing UI element positioning parameters.

## Components

| Component | Description |
|-----------|-------------|
| `UiTransformComponent` | Stores anchor, pivot, offsets, and viewport reference for UI positioning |

## UiTransformComponent Properties

| Property | Type | Description |
|----------|------|-------------|
| `anchor` | `Anchor` | Where the element attaches to the viewport |
| `pivot` | `Anchor` | The element's own reference point for positioning |
| `offsets` | `vec4f` | Offset from anchor (top, right, bottom, left) |
| `viewportId` | `ViewportId` | The viewport this element is positioned relative to |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::ui::transform::components
@brief UI transform state components.
@details Contains components that store positioning parameters for UI elements, including anchor points, pivot points, offsets, and viewport references.
</p></details>
