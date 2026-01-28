# helios::engine::builder::gameObject::builders

Domain-specific builders for configuring different aspects of GameObjects.

## Overview

Each builder encapsulates a specific domain of entity configuration. Builders provide methods that return fine-grained config objects, which in turn add and configure the appropriate components on the target GameObject.

## Builders

| Builder | Domain | Configs Provided |
|---------|--------|------------------|
| `MotionBuilder` | Movement | `Move2DConfig`, `SteeringConfig` |
| `RenderingBuilder` | Visuals | `RenderableConfig`, `SceneNodeConfig` |
| `SceneBuilder` | Scene Graph | `SceneNodeConfig` |
| `CollisionBuilder` | Physics | `CollisionConfig`, `LevelBoundsCollisionConfig` |
| `TransformBuilder` | Spatial | `TransformConfig` |
| `EffectsBuilder` | VFX | `GfxEffectsConfig` |
| `SpawnBuilder` | Spawning | `SpawnConfig` |
| `AiBuilder` | AI | `ChaseConfig` |
| `ShootingBuilder` | Combat | `WeaponConfig` |
| `LifecycleBuilder` | Lifecycle | `LifecycleConfig` |

## Usage Pattern

Each builder follows the same pattern:

```cpp
prototype.withMotion([](MotionBuilder& builder) {
    builder.move2D()        // Returns Move2DConfig
           .speed(5.0f)
           .acceleration(10.0f);
    
    builder.steering()      // Returns SteeringConfig
           .instantSteering(true);
});
```

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                     Builder (e.g. MotionBuilder)            │
│  ┌─────────────────────────────────────────────────────────┐│
│  │ GameObject* gameObject_                                 ││
│  ├─────────────────────────────────────────────────────────┤│
│  │ move2D()     → Move2DConfig{gameObject_}               ││
│  │ steering()   → SteeringConfig{gameObject_}             ││
│  └─────────────────────────────────────────────────────────┘│
└─────────────────────────────────────────────────────────────┘
```

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.engine.builder.gameObject.builders.configs` | Config classes |
| `helios.engine.modules` | Component implementations |
| `helios.engine.mechanics` | Gameplay components |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::builder::gameObject::builders
@brief Domain-specific builders for configuring different aspects of GameObjects.
@details Each builder encapsulates a specific domain of entity configuration (motion, rendering, collision, etc.). Builders provide methods that return fine-grained config objects, which add and configure the appropriate components on the target GameObject.
</p></details>
