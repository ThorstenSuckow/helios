# Component System

helios implements a **composition-based component architecture** that separates data from behavior, enabling flexible and reusable game logic without deep inheritance hierarchies.

## Overview

The component system follows the "composition over inheritance" principle:

- **Components** store data for GameObjects (pure data containers)
- **Systems** operate on groups of GameObjects with specific component configurations
- **GameWorld** manages the lifecycle of GameObjects, Managers, and Pools
- **GameLoop** orchestrates Systems across Phases and Passes

This design allows you to build complex entities by mixing and matching components rather than creating specialized subclasses.

## Key Classes

### `Component`

The base class for all components attached to a GameObject. Components are **data containers** — they store state but should not contain complex update logic.

```cpp
import helios.engine.ecs.Component;

class HealthComponent : public helios::engine::ecs::Component {
    int health_ = 100;
    
public:
    void takeDamage(int amount) { health_ -= amount; }
    int health() const { return health_; }
};
```

> **Note:** The namespace `helios::engine::ecs` contains the core entity-component classes (`GameObject`, `Component`, `System`), while `helios::engine::runtime::world` manages the game world and lifecycle (`GameWorld`, `UpdateContext`, `Manager`).

Components receive a back-reference to their owning GameObject via `onAttach()`, allowing them to interact with sibling components or the entity as a whole.

#### Component Enabled State

Each component has an `isEnabled()` flag that controls whether it participates in system processing:

```cpp
bool isEnabled() const noexcept;
bool isDisabled() const noexcept;
void enable() noexcept;
void disable() noexcept;
```

| State | Meaning |
|-------|---------|
| `isEnabled() == true` | Component is active and processed by systems |
| `isEnabled() == false` | Component is skipped by systems but remains attached |

This allows fine-grained control over individual components without removing them from the GameObject. For example, temporarily disabling a `CollisionComponent` makes an entity pass through walls.

**Important:** Disabling a component does **not** disable the entire GameObject. Use `GameObject::setActive(false)` to exclude an entity from all processing. See [Conventions: GameObject Active State](conventions.md#gameobject-active-state).

### `GameObject`

A container for components that represents an entity in the game world.

```cpp
import helios.engine.ecs.GameObject;

auto entity = std::make_unique<helios::engine::ecs::GameObject>();

// Add components to define behavior
entity->add<SceneNodeComponent>(sceneNode);
entity->add<Move2DComponent>();
entity->add<HealthComponent>();

// Retrieve components by type (O(1) lookup)
auto* health = entity->get<HealthComponent>();
```

Each GameObject has a unique `Guid` for identification and can be queried efficiently via `GameWorld::find<Components...>()`.

### `System`

Global logic processors that operate on the entire GameWorld. Systems are registered with the **GameLoop** and executed within Phases and Passes.

```cpp
import helios.engine.ecs.System;

class PhysicsSystem : public helios::engine::ecs::System {
public:
    void update(UpdateContext& ctx) noexcept override {
        // Iterate all active GameObjects with Move2DComponent
        for (auto [obj, move] : ctx.gameWorld().find<Move2DComponent>().each()) {
            auto* node = obj->get<SceneNodeComponent>();
            // Apply physics simulation...
        }
    }
};
```

Systems are organized into Phases (Pre, Main, Post) and Passes within each Phase. See [Game Loop Architecture](gameloop-architecture.md) for details.

### `GameWorld`

The root container managing GameObjects, Managers, and Pools. Located in `helios::engine::runtime::world`.

```cpp
import helios.engine.runtime.world.GameWorld;

helios::engine::runtime::world::GameWorld world;

// Add entities
auto* player = world.addGameObject(std::move(playerEntity));

// Add managers for deferred processing
world.addManager<ProjectilePoolManager>(poolId);

// Query entities by component
for (auto* obj : world.find<Move2DComponent, SceneNodeComponent>()) {
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
| `TransformComponent` | Stores local transform with dirty tracking |
| `ScaleComponent` | Unit-based sizing using helios units (meters) |
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
| `Move2DSystem` | Applies velocity/acceleration to TransformComponent |
| `HeadingSystem` | Smoothly rotates entities toward target angle |
| `SpinSystem` | Applies continuous rotation |
| `BoundsUpdateSystem` | Updates AABB colliders from transforms |
| `GridCollisionDetectionSystem` | Spatial partitioning collision detection |
| `LevelBoundsBehaviorSystem` | Handles boundary collision behaviors |
| `ComposeTransformSystem` | Composes transform from translation/rotation/scale |
| `ScaleSystem` | Applies scale changes |

### Post Phase Systems

| System | Purpose |
|--------|---------|
| `SceneSyncSystem` | Syncs transforms from gameplay to scene graph |
| `TransformClearSystem` | Clears dirty flags after frame |
| `ScaleClearSystem` | Clears scale dirty flags |

## Creating Custom Components

Define a class inheriting from `Component`. Components should be placed in your project's modules namespace:

```cpp
export module myproject.components.Inventory;

import helios.engine.ecs.Component;

export class InventoryComponent : public helios::engine::ecs::Component {
    std::vector<Item> items_;
    
public:
    void addItem(Item item) { items_.push_back(std::move(item)); }
    const std::vector<Item>& items() const { return items_; }
};
```

For components that need cloning (e.g., for object pools), implement `CloneableComponent`:

```cpp
import helios.engine.ecs.CloneableComponent;

class HealthComponent : public helios::engine::ecs::CloneableComponent {
    int maxHealth_ = 100;
    int currentHealth_ = 100;
    
public:
    std::unique_ptr<Component> clone() const override {
        auto copy = std::make_unique<HealthComponent>();
        copy->maxHealth_ = maxHealth_;
        copy->currentHealth_ = maxHealth_;  // Reset to max on clone
        return copy;
    }
};
```

## Creating Custom Systems

Define a class inheriting from `System`. Systems are registered with the GameLoop and operate on the GameWorld:

```cpp
export module myproject.systems.Spawner;

import helios.engine.ecs.System;
import helios.engine.runtime.world.GameWorld;

export class SpawnerSystem : public helios::engine::ecs::System {
    float timer_ = 0.0f;
    
public:
    explicit SpawnerSystem(helios::engine::runtime::world::GameWorld& world) 
        : System(world) {}
    
    void update(helios::engine::runtime::world::UpdateContext& ctx) noexcept override {
        timer_ += ctx.deltaTime();
        if (timer_ > 5.0f) {
            timer_ = 0.0f;
            // Queue spawn via CommandBuffer
            ctx.commandBuffer().add<SpawnCommand>(position, enemyType);
        }
    }
};
```

## Querying GameObjects

GameWorld provides efficient component-based queries:

```cpp
// Find all GameObjects with specific components
for (auto* obj : gameWorld.find<Move2DComponent, SceneNodeComponent>()) {
    // obj has both components
}

// Using structured bindings for direct component access
for (auto [obj, move, node] : gameWorld.find<Move2DComponent, SceneNodeComponent>().each()) {
    // move and node are references to the components
}

// Filter by active state
for (auto* obj : gameWorld.find<HealthComponent>(GameObjectFilter::Active)) {
    // Only active GameObjects
}
```

### GameObjectFilter

The `GameObjectFilter` enum controls which GameObjects are included in query results:

```cpp
import helios.engine.ecs.query.GameObjectFilter;

using helios::engine::ecs::query::GameObjectFilter;
```

| Filter | Meaning |
|--------|---------|
| `GameObjectFilter::None` | No filtering (default) |
| `GameObjectFilter::Active` | Only `obj->isActive() == true` |
| `GameObjectFilter::Inactive` | Only `obj->isActive() == false` |
| `GameObjectFilter::ComponentEnabled` | Only objects with enabled queried components |
| `GameObjectFilter::ComponentDisabled` | Only objects with disabled queried components |
| `GameObjectFilter::All` | All GameObjects regardless of state |

Filters can be combined using bitwise OR:

```cpp
// Find inactive objects with disabled components
auto filter = GameObjectFilter::Inactive | GameObjectFilter::ComponentDisabled;
for (auto* obj : gameWorld.find<CollisionComponent>(filter)) {
    // Process matching objects
}
```

### Filtering Examples

**Active GameObjects with enabled components:**

```cpp
void update(UpdateContext& ctx) noexcept override {
    auto filter = GameObjectFilter::Active | GameObjectFilter::ComponentEnabled;
    
    for (auto [obj, move, collision] : gameWorld_->find<Move2DComponent, CollisionComponent>(filter).each()) {
        // Both GameObject is active AND components are enabled
        // No manual checks required
    }
}
```

**Manual filtering (alternative approach):**

```cpp
void update(UpdateContext& ctx) noexcept override {
    for (auto [obj, move, collision] : gameWorld_->find<Move2DComponent, CollisionComponent>().each()) {
        
        // Skip inactive GameObjects
        if (!obj->isActive()) {
            continue;
        }
        
        // Skip disabled components
        if (collision.isDisabled()) {
            continue;  // Movement still applies, but no collision
        }
        
        // Process with collision enabled...
    }
}
```

**Finding disabled components (e.g., for re-enabling):**

```cpp
// Find objects where invulnerability expired
for (auto [obj, invuln] : gameWorld_->find<InvulnerabilityComponent>(
        GameObjectFilter::Active | GameObjectFilter::ComponentDisabled).each()) {
    // Re-enable collision after invulnerability ends
    obj->get<CollisionComponent>()->enable();
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
// Instead of: class Player : public GameObject { ... }

// Do this:
auto player = std::make_unique<GameObject>();
player->add<SceneNodeComponent>(node);
player->add<Move2DComponent>();
player->add<HeadingComponent>();
player->add<HealthComponent>();
player->add<TwinStickInputComponent>();
```

**Use Commands for State Mutations:** Instead of mutating state directly in Systems, emit Commands to the CommandBuffer for deterministic execution.

## Related Documentation

- [Game Loop Architecture](gameloop-architecture.md) — Phase/Pass structure, commit points
- [Command System](command-system.md) — Deferred action execution
- [Event System](event-system.md) — Phase/pass event propagation
