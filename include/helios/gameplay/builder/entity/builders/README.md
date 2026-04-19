# helios::gameplay::builder::entity::builders

Domain-specific builders for configuring different aspects of Entities.

## Overview

Each builder encapsulates a specific domain of entity configuration. Builders provide methods that return fine-grained config objects, which in turn add and configure the appropriate components on the target Entity.

## Builders

| Builder | Domain | Configs Provided |
|---------|--------|------------------|
| `MotionBuilder` | Movement | `Move2DConfig`, `SteeringConfig` |
| `RenderingBuilder` | Visuals | `MeshRenderableConfig`, `SceneNodeConfig` |
| `SceneBuilder` | Scene Graph | `SceneNodeConfig` |
| `CollisionBuilder` | Physics | `CollisionConfig`, `LevelBoundsCollisionConfig` |
| `TransformBuilder` | Spatial | `TransformConfig` |
| `UiTransformBuilder` | UI Layout | `UiTransformConfig` |
| `EffectsBuilder` | VFX | `GfxEffectsConfig` |
| `SpawnBuilder` | Spawning | `SpawnConfig` |
| `AiBuilder` | AI | `ChaseConfig` |
| `CombatBuilder` | Combat | `WeaponConfig` |
| `LifecycleBuilder` | Lifecycle | `LifecycleConfig` |
| `HealthBuilder` | Health | `HealthConfig` |
| `ScoringBuilder` | Scoring | `ScoreValueConfig`, `ScorePoolConfig` |
| `ObserverBuilder` | Data Binding | `ObserverConfig` |

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
│  │ Entity entity_                                 ││
│  ├─────────────────────────────────────────────────────────┤│
│  │ move2D()     → Move2DConfig{entity_}               ││
│  │ steering()   → SteeringConfig{entity_}             ││
│  └─────────────────────────────────────────────────────────┘│
└─────────────────────────────────────────────────────────────┘
```

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.gameplay.builder.entity.builders.configs` | Config classes |
| `helios` | Component implementations |
| `helios.gameplay` | Gameplay components |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::gameplay::builder::entity::builders
@brief Domain-specific builders for configuring different aspects of Entities.
@details Each builder encapsulates a specific domain of entity configuration (motion, rendering, collision, etc.). Builders provide methods that return fine-grained config objects, which add and configure the appropriate components on the target Entity.
</p></details>
