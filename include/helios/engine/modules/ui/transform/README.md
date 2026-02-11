# helios::engine::modules::ui::transform

UI transform components and positioning systems.

This namespace provides components and systems for viewport-relative positioning of UI elements. Unlike spatial transforms used for 3D scene objects, UI transforms work in screen space coordinates relative to viewports.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `components/` | UI transform state components |
| `systems/` | Systems for computing UI element positions |

## Key Classes

| Class | Purpose |
|-------|---------|
| `UiTransformComponent` | Stores anchor, pivot, offsets, and viewport reference |
| `UiTransformSystem` | Computes screen positions based on viewport bounds |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::ui::transform
@brief UI transform components and positioning systems.
@details This namespace handles viewport-relative positioning for UI elements. Components store layout parameters (anchor, pivot, offsets), while systems compute final screen positions based on current viewport dimensions.
</p></details>
