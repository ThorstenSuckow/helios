# helios::engine::builder

Fluent builder pattern implementation for constructing game entities in the helios engine.

## Overview

This module provides a declarative, composable API for creating `GameObject` instances with their associated components. The builder pattern enables clean, readable entity construction while abstracting the complexity of component dependencies and initialization order.

## Key Classes

| Class | Purpose |
|-------|---------|
| `GameObjectFactory` | Entry point for creating and cloning GameObjects |
| `GameObjectPrototype` | Fluent builder for configuring entity components |

## Submodules

### gameObject/

The main factory and prototype classes for GameObject construction.

### gameObject/builders/

Domain-specific builders for different aspects of entity configuration:

| Builder | Purpose |
|---------|---------|
| `MotionBuilder` | Movement and steering configuration |
| `RenderingBuilder` | Visual representation setup |
| `SceneBuilder` | Scene graph integration |
| `CollisionBuilder` | Collision detection configuration |
| `TransformBuilder` | Spatial transform setup |
| `EffectsBuilder` | Visual effects (spin, etc.) |
| `SpawnBuilder` | Spawn system integration |
| `AiBuilder` | AI behavior configuration |
| `CombatBuilder` | Weapon and projectile setup |

### gameObject/builders/configs/

Fine-grained configuration classes used by builders:

| Config | Purpose |
|--------|---------|
| `Move2DConfig` | 2D movement parameters |
| `SteeringConfig` | Rotation and steering behavior |
| `MeshRenderableConfig` | Mesh, material, and shader setup |
| `SceneNodeConfig` | Scene graph parenting and inheritance |
| `TransformConfig` | Scale and translation |
| `CollisionConfig` | Collision layers and responses |
| `LevelBoundsCollisionConfig` | Arena boundary behavior |
| `GfxEffectsConfig` | Visual effects parameters |
| `SpawnConfig` | Spawn profile integration |
| `ChaseConfig` | AI chase behavior |
| `WeaponConfig` | Weapon fire rate and projectiles |

## Usage

### Creating a New GameObject

```cpp
auto player = GameObjectFactory::gameObject()
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

### Cloning an Existing GameObject

```cpp
auto enemyTemplate = createEnemyTemplate();
auto enemy = GameObjectFactory::from(enemyTemplate.get())
    .withTransform([&](auto& transform) {
        transform.transform()
                 .translate(spawnPosition);
    })
    .make(true);
```

## Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                     GameObjectFactory                           │
│  ┌───────────────────────────────────────────────────────────┐  │
│  │                  GameObjectPrototype                      │  │
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
│  │  │                    GameObject                       │  │  │
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
5. **Cloning Support**: `GameObjectFactory::from()` creates new entities based on existing templates.

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.engine.ecs` | GameObject, Component base classes |
| `helios.engine.modules` | Component implementations used by configs |
| `helios.engine.mechanics` | Gameplay-specific components |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::builder
@brief Fluent builder pattern for constructing GameObjects.
@details This module provides a declarative, composable API for creating GameObject instances with their associated components. The builder pattern enables clean, readable entity construction while abstracting component dependencies and initialization order. Key classes include GameObjectFactory (entry point), GameObjectPrototype (fluent configuration), domain-specific builders (MotionBuilder, RenderingBuilder, CollisionBuilder, etc.), and fine-grained config classes (Move2DConfig, MeshRenderableConfig, etc.).
</p></details>
