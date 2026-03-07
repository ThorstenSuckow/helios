# Component System

helios implements a **composition-based component architecture** that separates data from behavior, enabling flexible and reusable game logic without deep inheritance hierarchies.

## Overview

The component system follows the "composition over inheritance" principle:

- **Components** store data for entities (plain data classes, no base class required)
- **Systems** operate on groups of entities with specific component configurations
- **GameWorld** manages entity lifecycle via EntityRegistry and EntityManager
- **GameLoop** orchestrates Systems across Phases and Passes

This design allows you to build complex entities by mixing and matching components rather than creating specialized subclasses.

## Key Classes

### Components

Components are **plain data classes** - they store state but should not contain complex update logic. Unlike traditional ECS frameworks, helios components do **not** inherit from a base class.

```cpp
class HealthComponent {
    float health_ = 100.0f;
    float maxHealth_ = 100.0f;
    bool isEnabled_ = true;

public:
    // Required: Copy and Move constructors
    HealthComponent(const HealthComponent&) = default;
    HealthComponent(HealthComponent&&) noexcept = default;

    // Optional: Enable/Disable for view filtering
    [[nodiscard]] bool isEnabled() const noexcept { return isEnabled_; }
    void enable() noexcept { isEnabled_ = true; }
    void disable() noexcept { isEnabled_ = false; }

    // Business logic
    void takeDamage(float amount) { health_ -= amount; }
    [[nodiscard]] float health() const { return health_; }
};
```

> **Note:** Components must have a `noexcept` move constructor for efficient storage in SparseSets. See [Component Structure](ecs/component-structure.md) for full requirements.

#### Component Enabled State

Components can implement `isEnabled()` / `enable()` / `disable()` for view filtering:

```cpp
[[nodiscard]] bool isEnabled() const noexcept;
void enable() noexcept;
void disable() noexcept;
```

| State | Meaning |
|-------|---------|
| `isEnabled() == true` | Component is active and included in filtered views |
| `isEnabled() == false` | Component is skipped by `.whereEnabled()` but remains attached |

This allows fine-grained control over individual components. For example, temporarily disabling a `CollisionComponent` makes an entity pass through walls.

**Important:** Disabling a component does **not** deactivate the entire entity. Use `GameObject::setActive(false)` to exclude an entity from all processing.

### `GameObject`

A lightweight wrapper (~16 bytes) for entity manipulation. Pass by value, not by reference.

```cpp
// Create entity via GameWorld
auto entity = gameWorld.addGameObject();

// Add components
entity.add<SceneNodeComponent>(sceneNode);
entity.add<Move2DComponent>(speed);
entity.add<HealthComponent>(100.0f);

// Retrieve components by type (O(1) lookup)
auto* health = entity.get<HealthComponent>();

// Check component presence
if (entity.has<CollisionComponent>()) {
    // ...
}
```

Each entity is identified by a versioned `EntityHandle` for safe reference tracking.

#### Component Storage

Components are stored in type-specific `SparseSet<T>` containers managed by `EntityManager`. This provides **O(1) access**:

| Operation | Complexity | Description |
|-----------|------------|-------------|
| `get<T>()` | O(1) | Direct SparseSet lookup via type ID |
| `has<T>()` | O(1) | SparseSet contains check |
| `add<T>()` | O(1) amortized | SparseSet emplace |

### `System`

Global logic processors that operate on the entire GameWorld. Systems are registered with the **GameLoop** and executed within Phases and Passes.

```cpp
import helios.engine.runtime.world.UpdateContext;

class PhysicsSystem {
public:
    void update(UpdateContext& ctx) noexcept {
        // Iterate all active entities with required components
        for (auto [entity, move, transform, active] : ctx.view<
            Move2DComponent,
            TranslationStateComponent,
            Active
        >().whereEnabled()) {
            // Apply physics simulation...
            transform->translateBy(move->velocity() * ctx.deltaTime());
        }
    }
};
```

Systems are organized into Phases (Pre, Main, Post) and Passes within each Phase. See [Game Loop Architecture](gameloop-architecture.md) for details.

### `GameWorld`

The root container managing entities, Managers, and Pools. Located in `helios::engine::runtime::world`.

```cpp
import helios.engine.runtime.world.GameWorld;

helios::engine::runtime::world::GameWorld world;

// Create entities
auto player = world.addGameObject();
player.add<Move2DComponent>(speed);

// Register managers for deferred processing
world.registerManager<SpawnManager>();

// Query entities by component using views
for (auto [entity, move, collision, active] : world.view<
    Move2DComponent,
    CollisionComponent,
    Active
>().whereEnabled()) {
    // Process matching entities
}
```

### `GameLoop`

The orchestrator for system execution, located in `helios::engine::runtime::gameloop`. Systems are added to Phases and Passes:

```cpp
import helios.engine.runtime.gameloop.GameLoop;

helios::engine::runtime::gameloop::GameLoop gameLoop;

// Pre Phase: Input processing
gameLoop.phase(PhaseType::Pre)
    .addPass()
    .addSystem<InputSystem>(gameWorld);

// Main Phase: Gameplay logic
gameLoop.phase(PhaseType::Main)
    .addPass()
    .addSystem<Move2DSystem>(gameWorld)
    .addSystem<CollisionSystem>(gameWorld);

// Post Phase: Synchronization
gameLoop.phase(PhaseType::Post)
    .addPass()
    .addSystem<SceneSyncSystem>(gameWorld, scene);

// Initialize and run
gameLoop.init(gameWorld);
gameLoop.update(gameWorld, deltaTime, inputSnapshot);
```

## Built-in Components

helios provides several ready-to-use components organized by domain:

### Spatial/Transform

| Component | Purpose |
|-----------|---------|
| `ComposeTransformComponent` | Stores local transform with dirty tracking |
| `ScaleStateComponent` | Unit-based sizing using helios units (meters) |
| `TranslationStateComponent` | Translation delta for frame-based movement |

### Physics/Motion

| Component | Purpose |
|-----------|---------|
| `Move2DComponent` | 2D movement parameters (max speed, acceleration, dampening) |
| `HeadingComponent` | Rotation state and target angle |
| `DirectionComponent` | Normalized movement direction |
| `SpinComponent` | Continuous rotation (spin speed) |

### Physics/Collision

| Component | Purpose |
|-----------|---------|
| `AabbColliderComponent` | Axis-aligned bounding box for collision |
| `CollisionComponent` | Collision configuration (layer, mask, response type) |

### Gameplay

| Component | Purpose |
|-----------|---------|
| `LevelBoundsBehaviorComponent` | Arena boundary interaction (bounce, clamp, wrap, despawn) |
| `ShootComponent` | Projectile firing with cooldown timer |
| `Aim2DComponent` | Aiming direction for twin-stick controls |

### Spawn/Pool

| Component | Purpose |
|-----------|---------|
| `SpawnedByProfileComponent` | Tracks which spawn profile created this entity |

### Scene

| Component | Purpose |
|-----------|---------|
| `SceneNodeComponent` | Links a GameObject to a scene graph node |

### Input

| Component | Purpose |
|-----------|---------|
| `TwinStickInputComponent` | Dual analog stick input mapping |

## Built-in Systems

Systems are organized by their typical Phase placement:

### Pre Phase Systems

| System | Purpose |
|--------|---------|
| Input processing systems | Read input, generate Commands |

### Main Phase Systems

| System | Purpose |
|--------|---------|
| `Move2DSystem` | Applies velocity/acceleration to ComposeTransformComponent |
| `HeadingSystem` | Smoothly rotates entities toward target angle |
| `SpinSystem` | Applies continuous rotation |
| `BoundsUpdateSystem` | Updates AABB colliders from transforms |
| `GridCollisionDetectionSystem` | Spatial partitioning collision detection |
| `LevelBoundsBehaviorSystem` | Handles boundary collision behaviors |
| `ComposeTransformSystem` | Composes transform from translation/rotation/scale |
| `ScaleSystem` | Applies scale changes |
| `GameObjectSpawnSystem` | Evaluates spawn rules, creates spawn commands |
| `ProjectileSpawnSystem` | Handles projectile spawning from shoot commands |

### Post Phase Systems

| System | Purpose |
|--------|---------|
| `SceneSyncSystem` | Syncs transforms from gameplay to scene graph |
| `TransformClearSystem` | Clears dirty flags after frame |

## Creating Custom Components

Components are plain data classes — they do not inherit from a base class. Components should be placed in your project's modules namespace:

```cpp
export module myproject.components.Inventory;

export class InventoryComponent {
    std::vector<Item> items_;
    bool isEnabled_ = true;

public:
    // Required: Copy and Move constructors
    InventoryComponent(const InventoryComponent&) = default;
    InventoryComponent(InventoryComponent&&) noexcept = default;

    // Optional: Enable/Disable for view filtering
    [[nodiscard]] bool isEnabled() const noexcept { return isEnabled_; }
    void enable() noexcept { isEnabled_ = true; }
    void disable() noexcept { isEnabled_ = false; }

    void addItem(Item item) { items_.push_back(std::move(item)); }
    [[nodiscard]] const std::vector<Item>& items() const { return items_; }
};
```

> **Note:** Components must provide both a copy constructor (used by the blueprint/pool system) and a `noexcept` move constructor for efficient storage in SparseSets. See [Component Structure](ecs/component-structure.md) for full requirements.

## Creating Custom Systems

Define a plain class with an `update(UpdateContext&)` method. Systems are registered with the GameLoop via `addSystem<T>()`:

```cpp
export module myproject.systems.Spawner;

import helios.engine.runtime.world.UpdateContext;

export class SpawnerSystem {
    float timer_ = 0.0f;

public:
    void update(helios::engine::runtime::world::UpdateContext& ctx) noexcept {
        timer_ += ctx.deltaTime();
        if (timer_ > 5.0f) {
            timer_ = 0.0f;
            // Queue spawn command
            ctx.queueCommand<SpawnCommand>(position, enemyType);
        }
    }
};
```

## Querying Entities

Systems query entities via `UpdateContext::view<>()`, which returns a `View` over all entities possessing the requested components. The `Active` tag component and `.whereEnabled()` filter replace the former `GameObjectFilter` API.

```cpp
import helios.engine.runtime.world.UpdateContext;
import helios.engine.mechanics.lifecycle.components.Active;

// In a system's update():
void update(UpdateContext& ctx) noexcept {

    // Iterate all active entities with specific components
    for (auto [entity, move, node, active] : ctx.view<
        Move2DComponent,
        SceneNodeComponent,
        Active
    >().whereEnabled()) {
        // entity is the EntityHandle
        // move, node are pointers to the components
    }
}
```

### Active Filtering

Include the `Active` tag component in the view to restrict results to active entities. Call `.whereEnabled()` to additionally skip entities whose components report `isEnabled() == false`:

```cpp
// Active entities with enabled components
for (auto [entity, health, active] : ctx.view<
    HealthComponent,
    Active
>().whereEnabled()) {
    // Only entities that are active AND whose components are enabled
}
```

### Unfiltered Iteration

Omitting the `Active` tag and `.whereEnabled()` iterates **all** entities with the requested components, regardless of activation state:

```cpp
// All entities (active or inactive)
for (auto [entity, collision] : ctx.view<CollisionComponent>()) {
    // Includes inactive entities and disabled components
}
```

### Excluding Components

Use `.exclude<T>()` to skip entities that have a specific component:

```cpp
for (auto [entity, move, active] : ctx.view<
    Move2DComponent,
    Active
>().exclude<FrozenTag>().whereEnabled()) {
    // Entities with Move2DComponent but without FrozenTag
}
```

### Initialization-Time Queries

Outside of a system's `update()` (e.g. during setup), queries go through `GameWorld::view<>()` directly:

```cpp
for (auto [entity, transform, velocity, active] : gameWorld.view<
    TransformComponent,
    VelocityComponent,
    Active
>().whereEnabled()) {
    transform->position += velocity->direction * deltaTime;
}
```

## Update Order

The GameLoop executes updates in three phases:

```
┌─────────────────────────────────────────────────────────────────┐
│                          FRAME                                  │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  PRE PHASE ──────────────────────────────────────────────────   │
│    Pass 1: Input systems                                        │
│    [Phase Commit: Commands flush, Managers process]             │
│                                                                 │
│  MAIN PHASE ─────────────────────────────────────────────────   │
│    Pass 1: Movement, Physics                                    │
│    Pass 2: Collision Detection                                  │
│    Pass 3: Gameplay Systems                                     │
│    [Phase Commit: Commands flush, Managers process]             │
│                                                                 │
│  POST PHASE ─────────────────────────────────────────────────   │
│    Pass 1: Scene Sync                                           │
│    Pass 2: Cleanup (clear dirty flags)                          │
│    [Phase Commit: Commands flush, Managers process]             │
│                                                                 │
│                        RENDER                                   │
└─────────────────────────────────────────────────────────────────┘
```

See [Game Loop Architecture](gameloop-architecture.md) for detailed phase/pass event handling.

## Best Practices

**Keep Components as Data Containers:** Components should store state, not complex logic. Move update logic to Systems.

**Use Systems for Cross-Cutting Concerns:** Physics simulation, collision detection, and scene synchronization belong in Systems.

**Prefer Composition:** Configure entities by attaching different component combinations rather than creating specialized subclasses.

```cpp
// Instead of deep inheritance hierarchies:

// Do this:
auto player = gameWorld.addGameObject();
player.add<SceneNodeComponent>(node);
player.add<Move2DComponent>();
player.add<HeadingComponent>();
player.add<HealthComponent>();
player.add<TwinStickInputComponent>();
```

**Use Commands for State Mutations:** Instead of mutating state directly in Systems, emit Commands to the CommandBuffer for deterministic execution.

## Related Documentation

- [Game Loop Architecture](gameloop-architecture.md) - Phase/Pass structure, commit points
- [Command System](command-system.md) - Deferred action execution
- [Event System](event-system.md) - Phase/pass event propagation
- [Spawn System](spawn-system.md) - Entity lifecycle and object pooling
