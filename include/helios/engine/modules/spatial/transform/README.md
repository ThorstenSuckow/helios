# helios::engine::modules::spatial::transform

Transform components and systems for spatial state management.

This module provides components that store individual transform properties and systems that compose them into final world transforms.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `components/` | Per-frame transform deltas and composed transform |
| `systems/` | Transform composition and cleanup systems |

## Key Classes

| Class | Purpose |
|-------|---------|
| `TranslationStateComponent` | Translation delta for the current frame |
| `RotationStateComponent` | Rotation delta for the current frame |
| `ScaleStateComponent` | Scale delta for the current frame |
| `ComposeTransformComponent` | Final composed transform matrix |
| `ComposeTransformSystem` | Builds final transform from component deltas |
| `ScaleSystem` | Applies scale state to composed transform |
| `TransformClearSystem` | Clears dirty flags after scene sync |

## Transform Workflow

Each frame, transform state flows through the pipeline:

1. **Input Systems** write deltas to state components
2. **ComposeTransformSystem** combines deltas into final matrix
3. **SceneSyncSystem** copies matrix to SceneNode
4. **TransformClearSystem** resets dirty flags

## Usage

```cpp
// Create entity with transform components (via builder)
auto entity = GameObjectFactory::gameObject()
    .withTransform([](auto& t) {
        t.transform()
         .translate({0.0f, 0.0f, 0.0f})
         .scale({1.0f, 1.0f, 1.0f});
    })
    .make(true);

// Direct component access for per-frame updates
auto* translation = entity.get<TranslationStateComponent>();
translation->add({1.0f, 0.0f, 0.0f});  // Move right

auto* rotation = entity.get<RotationStateComponent>();
rotation->add(helios::math::radians(45.0f), helios::math::Z_AXISf);

// Register systems in correct order
gameLoop.phase(PhaseType::Main).addPass()
    .addSystem<ComposeTransformSystem>(&gameWorld)
    .addSystem<ScaleSystem>(&gameWorld);

gameLoop.phase(PhaseType::Post).addPass()
    .addSystem<SceneSyncSystem>(&gameWorld, &scene)
    .addSystem<TransformClearSystem>(&gameWorld);
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::spatial::transform
@brief Transform components and systems for spatial state management.
@details This namespace contains the transform layer of the spatial system. Components store individual transform properties while systems compose them into final world transforms each frame.
</p></details>

