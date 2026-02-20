# GameObject Builder

helios provides a **fluent builder pattern** for constructing GameObjects with their associated components. The builder API enables clean, readable entity construction while abstracting component dependencies and initialization order.

## Overview

The traditional approach of manually adding components can become verbose:

```cpp
// Manual approach - verbose and repetitive
auto entity = gameWorld.addGameObject();
entity.add<Move2DComponent>(5.0f, 10.0f);
entity.add<ComposeTransformComponent>();
entity.add<DirectionComponent>();
entity.add<TranslationStateComponent>();
// ... more configuration
```

The builder pattern provides a cleaner alternative:

```cpp
// Builder approach - fluent and composable
auto player = GameObjectFactory::gameObject(gameWorld)
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

The entry point for entity construction. Requires a GameWorld reference.

```cpp
import helios.engine.builder;

// Create a new entity via the GameWorld
auto prototype = GameObjectFactory::gameObject(gameWorld);
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
    .withCombat([](CombatBuilder& b) { /* ... */ })
    .withHealth([](HealthBuilder& b) { /* ... */ })
    .withScoring([](ScoringBuilder& b) { /* ... */ })
    .withLifecycle([](LifecycleBuilder& b) { /* ... */ })
    .make(active);  // Returns GameObject (by value)
```

### Domain Builders

Each builder encapsulates configuration for a specific domain:

| Builder | Domain | Configs Provided |
|---------|--------|------------------|
| `MotionBuilder` | Movement | `Move2DConfig`, `SteeringConfig` |
| `RenderingBuilder` | Visuals | `MeshRenderableConfig`, `TextRenderableConfig`, `SceneNodeConfig` |
| `SceneBuilder` | Scene Graph | `SceneNodeConfig` |
| `CollisionBuilder` | Physics | `CollisionConfig`, `LevelBoundsCollisionConfig` |
| `TransformBuilder` | Spatial | `TransformConfig` |
| `UiTransformBuilder` | UI Layout | `UiTransformConfig` |
| `EffectsBuilder` | VFX | `GfxEffectsConfig` |
| `SpawnBuilder` | Spawning | `SpawnConfig` |
| `AiBuilder` | AI | `ChaseConfig` |
| `CombatBuilder` | Combat | `WeaponConfig`, `CombatConfig` |
| `HealthBuilder` | Health | `HealthConfig` |
| `ScoringBuilder` | Scoring | `ScoreValueConfig`, `ScorePoolConfig` |
| `LifecycleBuilder` | Lifecycle | `LifecycleConfig` |
| `ObserverBuilder` | Data Binding | `ObserverConfig` |

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
auto bullet = GameObjectFactory::gameObject(gameWorld)
    .withRendering([&](auto& r) {
        r.meshRenderable()
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
auto player = GameObjectFactory::gameObject(gameWorld)
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

### Why Callbacks Instead of done()-Chaining?

The `GameObjectFactory` uses **callbacks** (`withMotion([](auto&) { ... })`) rather than the done()-chaining pattern used by `SpawnSystemFactory`. The rationale:

- **Flat domains**: Motion, collision, rendering are independent and equal - there is no parent-child hierarchy to express.
- **Scoped grouping**: The callback visually groups related configs (e.g., `collision()` and `levelBoundsCollision()` inside the same `withCollision` block).
- **External captures**: Callbacks naturally capture external state (`[&shader, &root]`), which would require parameter threading in done()-chaining.

The `SpawnSystemFactory` uses done()-chaining because its configuration has a natural **tree structure** (pool → profile → rule). See [Spawn System](spawn-system.md#spawnsystemfactory-builder) for details.

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.engine.ecs` | GameObject, Component, CloneableComponent |
| `helios.engine.modules` | Component implementations |
| `helios.engine.mechanics` | Gameplay-specific components |
| `helios.engine.runtime.world` | GameWorld entity management |

## Related Concepts

- [Component System](component-system.md) - Understanding Components and Systems
- [Spawn System](spawn-system.md) - Spawn scheduling, profiles, and the SpawnSystemFactory builder
- [Object Pooling](object-pooling.md) - Pool lifecycle for spawned entities
- [Conventions](conventions.md) - Units and coordinate systems
