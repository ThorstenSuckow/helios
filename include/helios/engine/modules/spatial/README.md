# helios::engine::modules::spatial
Spatial data management for game entities.

This module provides components and systems for managing the spatial state of GameObjects, including transforms, translations, rotations, and scales.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `transform/` | Transform state components and composition systems |

## Key Classes

| Class | Purpose |
|-------|---------|
| `TranslationStateComponent` | Per-frame translation delta |
| `RotationStateComponent` | Per-frame rotation delta |
| `ScaleStateComponent` | Per-frame scale delta |
| `ComposeTransformComponent` | Final composed transform matrix |
| `ComposeTransformSystem` | Composes deltas into final transform |
| `ScaleSystem` | Applies scale changes |
| `TransformClearSystem` | Clears dirty flags after composition |

---

<details>
<summary>Doxygen</summary><p>
@details This namespace contains components and systems for managing entity positions, rotations, and scales. It separates spatial state from physics simulation, allowing for clean composition of final transforms.
@brief Spatial data management for game entities.
@namespace helios::engine::modules::spatial
</p></details>


