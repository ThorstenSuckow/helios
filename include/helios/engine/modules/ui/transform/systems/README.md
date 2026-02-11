# helios::engine::modules::ui::transform::systems

UI positioning systems.

This namespace contains systems that compute screen positions for UI elements based on their transform components and current viewport dimensions.

## Systems

| System | Description |
|--------|-------------|
| `UiTransformSystem` | Computes screen positions based on anchor, pivot, offsets, and viewport bounds |

## Processing

`UiTransformSystem` queries entities with:
- `UiTransformComponent` - positioning parameters
- `TranslationStateComponent` - output position
- `ComposeTransformComponent` - transform composition
- `ModelAabbComponent` - element bounds for pivot calculations

The system updates `TranslationStateComponent` based on the current viewport snapshot dimensions.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::ui::transform::systems
@brief UI positioning systems.
@details Contains systems that process UI transform components to compute final screen positions. The UiTransformSystem reads viewport snapshots and applies anchor/pivot/margin calculations.
</p></details>
