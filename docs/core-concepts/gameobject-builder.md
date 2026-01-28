# GameObject Builder

helios provides a **fluent builder pattern** for constructing GameObjects with their associated components. The builder API enables clean, readable entity construction while abstracting component dependencies and initialization order.

## Overview

The traditional approach of manually adding components can become verbose and error-prone:

```cpp
// Manual approach - verbose and repetitive
auto entity = std::make_unique<GameObject>();
entity->add<Move2DComponent>();
entity->get<Move2DComponent>()->setMovementSpeed(5.0f);
entity->get<Move2DComponent>()->setMovementAcceleration(10.0f);
entity->add<ComposeTransformComponent>();
entity->add<DirectionComponent>();
entity->add<TranslationStateComponent>();
// ... more configuration
```

The builder pattern provides a cleaner alternative:

```cpp
// Builder approach - fluent and composable
auto player = GameObjectFactory::gameObject()
    .withMotion([](auto& motion) {
        motion.move2D()
              .speed(5.0f)
              .acceleration(10.0f);
    })
    .make(true);
```

## Architecture

The builder system is organized into three layers:

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

### GameObjectFactory

The entry point for entity construction. Provides static methods for creating new entities or cloning existing ones.

```cpp
import helios.engine.builder;

// Create a new empty entity
auto prototype = GameObjectFactory::gameObject();

// Clone from existing template
auto prototype = GameObjectFactory::from(templateEntity);
```

### GameObjectPrototype

The fluent builder returned by the factory. Holds domain-specific builders and provides callback-based configuration methods.

```cpp
prototype
    .withMotion([](MotionBuilder& b) { /* ... */ })
    .withRendering([](RenderingBuilder& b) { /* ... */ })
    .withCollision([](CollisionBuilder& b) { /* ... */ })
    .withTransform([](TransformBuilder& b) { /* ... */ })
    .withEffects([](EffectsBuilder& b) { /* ... */ })
    .withSpawn([](SpawnBuilder& b) { /* ... */ })
    .withAi([](AiBuilder& b) { /* ... */ })
    .withShooting([](ShootingBuilder& b) { /* ... */ })
    .make(active);  // Returns std::unique_ptr<GameObject>
```

### Domain Builders

Each builder encapsulates configuration for a specific domain:

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

### Config Classes

Fine-grained configuration objects that add and configure specific components:

```cpp
motion.move2D()           // Returns Move2DConfig
      .speed(5.0f)        // Fluent method
      .acceleration(10.0f);
```

Configs automatically handle component dependencies using `getOrAdd()`, preventing duplicates when multiple configs share the same requirements.

## Creating Entities

### Basic Entity

```cpp
auto bullet = GameObjectFactory::gameObject()
    .withRendering([&](auto& r) {
        r.renderable()
         .shape(circleShape)
         .shader(shader)
         .primitiveType(PrimitiveType::Triangles)
         .color({1.0f, 1.0f, 0.0f, 1.0f})
         .attachTo(rootNode);
    })
    .withMotion([](auto& m) {
        m.move2D()
         .speed(20.0f)
         .instantAcceleration(true);
    })
    .make(true);
```

### Complex Entity

```cpp
auto player = GameObjectFactory::gameObject()
    .withMotion([](auto& motion) {
        motion.move2D()
              .speed(5.0f)
              .acceleration(10.0f);
        motion.steering()
              .instantSteering(false)
              .steeringSetsDirection(true);
    })
    .withRendering([&](auto& rendering) {
        rendering.renderable()
                 .shape(triangleShape)
                 .shader(shader)
                 .primitiveType(PrimitiveType::Triangles)
                 .color({0.0f, 1.0f, 0.0f, 1.0f})
                 .attachTo(rootNode);
    })
    .withCollision([](auto& collision) {
        collision.collision()
                 .useBoundingBox()
                 .layerId(PLAYER_LAYER)
                 .reportCollisions(true)
                 .solidCollisionMask(ENEMY_LAYER | WALL_LAYER);
        collision.levelBounds()
                 .onCollision(CollisionBehavior::Clamp)
                 .respondWith(CollisionResponse::Slide);
    })
    .withShooting([](auto& shooting) {
        shooting.weapon()
                .fireRate(0.1f);
    })
    .make(true);
```

## Cloning Entities

The factory supports cloning for spawning multiple instances from a template:

```cpp
// Create template once
auto enemyTemplate = GameObjectFactory::gameObject()
    .withRendering([&](auto& r) { /* ... */ })
    .withMotion([](auto& m) { /* ... */ })
    .withCollision([](auto& c) { /* ... */ })
    .make(false);  // Template stays inactive

// Clone for each spawn
for (int i = 0; i < 10; ++i) {
    auto enemy = GameObjectFactory::from(enemyTemplate.get())
        .withTransform([&](auto& t) {
            t.transform().translate(spawnPositions[i]);
        })
        .make(true);
    
    world.addGameObject(std::move(enemy));
}
```

### Cloning Behavior

When cloning with `GameObjectFactory::from()`:

1. Only components implementing `CloneableComponent` are copied
2. Each cloneable component's `clone()` method is invoked
3. Additional configuration can be applied before `make()`
4. Component dependencies are resolved via `getOrAdd()` to prevent duplicates

## Automatic Dependencies

Configs automatically add required dependent components. This prevents errors and ensures proper initialization order.

| Config | Automatically Adds |
|--------|-------------------|
| `Move2DConfig` | `Move2DComponent`, `ComposeTransformComponent`, `DirectionComponent`, `TranslationStateComponent` |
| `SteeringConfig` | `SteeringComponent`, `ComposeTransformComponent`, `RotationStateComponent`, `DirectionComponent` |
| `RenderableConfig` | `RenderableComponent` (mesh, material created internally) |
| `SceneNodeConfig` | `SceneNodeComponent` (creates and parents SceneNode) |
| `CollisionConfig` | `CollisionStateComponent`, `AabbColliderComponent`, `CollisionComponent` |

**Example:** If both `Move2DConfig` and `SteeringConfig` are used, only one `ComposeTransformComponent` and `DirectionComponent` are created because both use `getOrAdd()`.

## Design Principles

The builder module follows these principles:

1. **Fluent Interface**: Method chaining for readable configuration
2. **Callback-Based Configuration**: Builders accessed via lambdas for scoped configuration
3. **Automatic Dependencies**: Configs handle required components automatically
4. **Separation of Concerns**: Each builder focuses on a single domain
5. **Cloning Support**: Template-based entity spawning with customization

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.engine.ecs` | GameObject, Component, CloneableComponent |
| `helios.engine.modules` | Component implementations |
| `helios.engine.mechanics` | Gameplay-specific components |
| `helios.engine.runtime.world` | GameWorld entity management |

## Related Concepts

- [Component System](component-system.md) — Understanding Components and Systems
- [Spawn System](spawn-system.md) — Using builders with object pooling
- [Conventions](conventions.md) — Units and coordinate systems
