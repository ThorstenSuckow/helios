# helios::engine::modules::spatial::transform::components

Transform state components for spatial management.

This namespace contains components that store individual aspects of an entity's spatial state. These are composed by transform systems to produce final world transforms.

## Components

| Component | Description |
|-----------|-------------|
| `ComposeTransformComponent` | Local/world transform matrices with dirty tracking. |
| `ScaleStateComponent` | Entity dimensions (width, height, depth) with unit. |
| `TranslationStateComponent` | Current position/translation vector. |

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
@namespace helios::engine::modules::spatial::transform::components
@brief Transform state components for spatial management.
@details This namespace contains components that store individual aspects of an entity's spatial state including position, scale, and composed transform matrices. These are processed by transform systems to produce final world transforms.
</p></details>

