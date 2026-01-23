# helios::engine::builder::gameObject::builders::configs

Fine-grained configuration classes for component setup.

## Overview

Config classes provide a fluent interface for adding and configuring specific components on a GameObject. Each config is responsible for a narrow domain and automatically handles component dependencies.

## Config Classes

### Motion

| Config | Components Added | Purpose |
|--------|------------------|---------|
| `Move2DConfig` | `Move2DComponent`, `ComposeTransformComponent`, `DirectionComponent`, `TranslationStateComponent` | 2D movement with acceleration |
| `SteeringConfig` | `SteeringComponent`, `ComposeTransformComponent`, `RotationStateComponent`, `DirectionComponent` | Rotation and steering |

### Rendering

| Config | Components Added | Purpose |
|--------|------------------|---------|
| `RenderableConfig` | `RenderableComponent` | Mesh, material, shader setup |
| `SceneNodeConfig` | `SceneNodeComponent` | Scene graph integration |

### Spatial

| Config | Components Added | Purpose |
|--------|------------------|---------|
| `TransformConfig` | `ComposeTransformComponent`, `ScaleStateComponent`, `TranslationStateComponent` | Transform setup |

### Physics

| Config | Components Added | Purpose |
|--------|------------------|---------|
| `CollisionConfig` | `CollisionStateComponent`, `AabbColliderComponent`, `CollisionComponent` | Collision detection |
| `LevelBoundsCollisionConfig` | `LevelBoundsBehaviorComponent` | Arena boundary responses |

### Effects

| Config | Components Added | Purpose |
|--------|------------------|---------|
| `GfxEffectsConfig` | `SpinComponent`, `RotationStateComponent` | Visual effects |

### Gameplay

| Config | Components Added | Purpose |
|--------|------------------|---------|
| `SpawnConfig` | `SpawnedByProfileComponent` | Spawn profile tracking |
| `ChaseConfig` | `ChaseComponent` | AI chase behavior |
| `WeaponConfig` | `Aim2DComponent`, `ShootComponent` | Weapon configuration |

## Usage Pattern

All configs follow the fluent builder pattern:

```cpp
Move2DConfig{gameObject}
    .speed(5.0f)
    .acceleration(10.0f)
    .instantAcceleration(false);
```

## Automatic Dependencies

Configs automatically add required dependent components using `getOrAdd()`. This prevents duplicates when multiple configs share dependencies.

Example: Both `Move2DConfig` and `SteeringConfig` require `ComposeTransformComponent`. If both are used, only one instance is created.

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.engine.modules.physics` | Motion and collision components |
| `helios.engine.modules.spatial` | Transform components |
| `helios.engine.modules.rendering` | Rendering components |
| `helios.engine.mechanics` | Gameplay components |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::builder::gameObject::builders::configs
@brief Fine-grained configuration classes for component setup.
@details Config classes provide a fluent interface for adding and configuring specific components on a GameObject. Each config is responsible for a narrow domain and automatically handles component dependencies using getOrAdd() to prevent duplicates.
</p></details>
