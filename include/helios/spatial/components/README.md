# helios::spatial::components

Transform state and derived spatial components.

This namespace contains components that store individual aspects of an
entity's spatial state, together with lightweight aliases for derived matrices,
bounds, and 2D size/scale data.

## Components

| Component | Description |
|-----------|-------------|
| `BoundsComponent` | Stores 4D bounds data. |
| `ComposeTransformComponent` | Local/world transform matrices with dirty tracking. |
| `DimensionComponent` | Physical dimensions with unit metadata. |
| `LocalPositionStateComponent` | Local-space position vector. |
| `LocalToWorldBoundsComponent` | Computed world-space bounds. |
| `LocalToWorldMatrixComponent` | Computed local-to-world transform matrix. |
| `LocalTransformComponent` | Local transform state with dirty tracking. |
| `RotationStateComponent` | Composite heading/spin rotation state. |
| `Scale2DComponent` | 2D scale values. |
| `ScaleStateComponent` | Entity dimensions (width, height, depth) with unit. |
| `Size2DComponent` | 2D size values. |
| `TranslationStateComponent` | Current position/translation vector. |
| `WorldTransformStateComponent` | Stored world transform matrix. |

## Usage

Transform components are composed by the transform systems:

```cpp
auto& transform = gameObject.add<ComposeTransformComponent>();

auto& scale = gameObject.add<ScaleStateComponent>(1.0f, 1.0f, 1.0f, Unit::Meter);

auto& translation = gameObject.add<TranslationStateComponent>();
translation.setTranslation({0.0f, 0.0f, -5.0f});
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::spatial::components
@brief Transform state and derived spatial components.
@details This namespace contains components that store individual aspects of an
entity's spatial state including position, scale, local transforms, world
transforms, and derived bounds data.
</p></details>

