# helios::gameplay::builder

Fluent builder pattern implementation for constructing game entities in the helios engine.

## Overview

This module provides a declarative, composable API for creating `Entity` instances with their associated components. The builder pattern enables clean, readable entity construction while abstracting the complexity of component dependencies and initialization order.

## Key Classes

| Class | Purpose |
|-------|---------|
| `EntityFactory` | Entry point for creating and cloning Entities |
| `EntityPrototype` | Fluent builder for configuring entity components |

## Submodules

### entity/

The main factory and prototype classes for Entity construction.

### entity/builders/

Domain-specific builders for different aspects of entity configuration:

| Builder | Purpose |
|---------|---------|
| `MotionBuilder` | Movement and steering configuration |
| `RenderingBuilder` | Visual representation setup |
| `SceneBuilder` | Scene graph integration |
| `CollisionBuilder` | Collision detection configuration |
| `TransformBuilder` | Spatial transform setup |
| `UiTransformBuilder` | UI/viewport-relative transform setup |
| `EffectsBuilder` | Visual effects (spin, etc.) |
| `SpawnBuilder` | Spawn system integration |
| `AiBuilder` | AI behavior configuration |
| `CombatBuilder` | Weapon and projectile setup |
| `LifecycleBuilder` | Entity lifecycle (active/inactive) |
| `HealthBuilder` | Health and damage configuration |
| `ScoringBuilder` | Score tracking configuration |
| `ObserverBuilder` | Data binding and observation |

### entity/builders/configs/

Fine-grained configuration classes used by builders:

| Config | Purpose |
|--------|---------|
| `Move2DConfig` | 2D movement parameters |
| `SteeringConfig` | Rotation and steering behavior |
| `MeshRenderableConfig` | Mesh, material, and shader setup |
| `TextRenderableConfig` | Text rendering setup |
| `SceneNodeConfig` | Scene graph parenting and inheritance |
| `TransformConfig` | Scale and translation |
| `UiTransformConfig` | UI/viewport-relative positioning |
| `CollisionConfig` | Collision layers and responses |
| `LevelBoundsCollisionConfig` | Arena boundary behavior |
| `GfxEffectsConfig` | Visual effects parameters |
| `SpawnConfig` | Spawn profile integration |
| `ChaseConfig` | AI chase behavior |
| `WeaponConfig` | Weapon fire rate and projectiles |
| `CombatConfig` | Combat system configuration |
| `LifecycleConfig` | Entity lifecycle settings |
| `HealthConfig` | Health and max health values |
| `ScoreValueConfig` | Score value on defeat |
| `ScorePoolConfig` | Score pool binding |
| `ObserverConfig` | Data observation/binding |

## Usage

### Creating a New Entity

```cpp
auto player = EntityFactory::entity()
    .withMotion([](auto& motion) {
        motion.move2D()
              .speed(5.0f)
              .acceleration(10.0f);
        motion.steering()
              .instantSteering(false);
    })
    .withRendering([&](auto& rendering) {
        rendering.renderable()
                 .shape(triangleShape)
                 .shader(shader)
                 .primitiveType(PrimitiveType::Triangles)
                 .color({1.0f, 0.0f, 0.0f, 1.0f})
                 .attachTo(rootNode);
    })
    .withCollision([](auto& collision) {
        collision.collision()
                 .useBoundingBox()
                 .layerId(PLAYER_LAYER);
    })
    .make(true); // active = true
```

### Cloning an Existing Entity

```cpp
auto enemyTemplate = createEnemyTemplate();
auto enemy = EntityFactory::from(enemyTemplate.get())
    .withTransform([&](auto& transform) {
        transform.transform()
                 .translate(spawnPosition);
    })
    .make(true);
```

## Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                     EntityFactory                           │
│  ┌───────────────────────────────────────────────────────────┐  │
│  │                  EntityPrototype                      │  │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐        │  │
│  │  │MotionBuilder│  │RenderingBld │  │CollisionBld │  ...   │  │
│  │  └──────┬──────┘  └──────┬──────┘  └──────┬──────┘        │  │
│  │         │                │                │               │  │
│  │         ▼                ▼                ▼               │  │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐        │  │
│  │  │ Move2DConfig│  │RenderableCfg│  │CollisionCfg │  ...   │  │
│  │  │SteeringCfg  │  │SceneNodeCfg │  │LevelBndsCfg │        │  │
│  │  └──────┬──────┘  └──────┬──────┘  └──────┬──────┘        │  │
│  │         │                │                │               │  │
│  │         ▼                ▼                ▼               │  │
│  │  ┌─────────────────────────────────────────────────────┐  │  │
│  │  │                    Entity                       │  │  │
│  │  │  [Move2DComponent] [RenderableComponent] [...]      │  │  │
│  │  └─────────────────────────────────────────────────────┘  │  │
│  └───────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────┘
```

## Design Principles

The builder module follows these principles:

1. **Fluent Interface**: Method chaining for readable configuration.
2. **Callback-Based Configuration**: Builders are configured via lambdas, allowing scoped access.
3. **Automatic Dependencies**: Configs automatically add required dependent components.
4. **Separation of Concerns**: Each builder focuses on a single domain (motion, rendering, etc.).
5. **Cloning Support**: `EntityFactory::from()` creates new entities based on existing templates.

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.ecs` | Entity, Component base classes |
| `helios` | Component implementations used by configs |
| `helios.gameplay` | Gameplay-specific components |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::gameplay::builder
@brief Fluent builder pattern for constructing Entities.
@details This module provides a declarative, composable API for creating Entity instances with their associated components. The builder pattern enables clean, readable entity construction while abstracting component dependencies and initialization order. Key classes include EntityFactory (entry point), EntityPrototype (fluent configuration), domain-specific builders (MotionBuilder, RenderingBuilder, CollisionBuilder, etc.), and fine-grained config classes (Move2DConfig, MeshRenderableConfig, etc.).
</p></details>
