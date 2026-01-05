
# Component System

helios implements a **composition-based component architecture** that separates data from behavior, enabling flexible and reusable game logic without deep inheritance hierarchies.

## Overview

The component system follows the "composition over inheritance" principle:

- **Components** store data and optional behavior for GameObjects
- **Systems** operate on groups of GameObjects with specific component configurations
- **GameWorld** manages the lifecycle of GameObjects and Systems

This design allows you to build complex entities by mixing and matching components rather than creating specialized subclasses.

## Key Classes

### `Component`

The base class for all components attached to a GameObject.

```cpp
import helios.engine.game.Component;

class HealthComponent : public helios::engine::game::Component {
    int health_ = 100;
public:
    void takeDamage(int amount) { health_ -= amount; }
    int health() const { return health_; }
};
```

Components receive a back-reference to their owning GameObject via `onAttach()`, allowing them to interact with sibling components or the entity as a whole.

### `GameObject`

A container for components that represents an entity in the game world.

```cpp
import helios.engine.game.GameObject;

auto entity = std::make_unique<helios::engine::game::GameObject>();

// Add components to define behavior
entity->add<SceneNodeComponent>(sceneNode);
entity->add<Move2DComponent>();
entity->add<HealthComponent>();

// Retrieve components by type
auto* health = entity->get<HealthComponent>();
```

Each GameObject has a unique `Guid` for identification and supports per-frame updates for components implementing the `Updatable` interface.

### `System`

Global logic processors that operate on the entire GameWorld.

```cpp
import helios.engine.game.System;

class PhysicsSystem : public helios::engine::game::System {
public:
    void update(UpdateContext& ctx) noexcept override {
        // Iterate all GameObjects with Move2DComponent
        for (auto* obj : gameWorld_->find<Move2DComponent>()) {
            auto* move = obj->get<Move2DComponent>();
            auto* node = obj->get<SceneNodeComponent>();
            // Apply physics simulation...
        }
    }
};
```

Systems are registered with the GameWorld and executed each frame after individual component updates.

### `GameWorld`

The root container managing GameObjects, Systems, and the active Level.

```cpp
import helios.engine.game.GameWorld;

helios::engine::game::GameWorld world;

// Add entities
auto* player = world.addGameObject(std::move(playerEntity));

// Add systems
world.add<Move2DSystem>();
world.add<SceneSyncSystem>(scene.get());
world.add<BoundsUpdateSystem>();

// Update each frame
world.update(ctx);
```

## Built-in Components

helios provides several ready-to-use components:

| Component | Purpose |
|-----------|---------|
| `SceneNodeComponent` | Links a GameObject to a scene graph node |
| `Move2DComponent` | 2D physics state (velocity, acceleration, rotation) |
| `TransformComponent` | Stores local and world transform matrices |
| `ScaleComponent` | Unit-based sizing using helios units (meters) |
| `TwinStickInputComponent` | Dual analog stick input handling |
| `ShootComponent` | Projectile firing with cooldown timer |
| `AabbColliderComponent` | Axis-aligned bounding box for collision |
| `LevelBoundsBehaviorComponent` | Arena boundary interaction (bounce, clamp, wrap) |

## Built-in Systems

| System | Purpose |
|--------|---------|
| `Move2DSystem` | Physics simulation (velocity, rotation) |
| `SceneSyncSystem` | Syncs transforms from gameplay to scene graph |
| `ScaleSystem` | Applies ScaleComponent sizing |
| `BoundsUpdateSystem` | Updates AABB colliders from transforms |
| `LevelBoundsBehaviorSystem` | Handles boundary collision behaviors |
| `ProjectilePoolSystem` | Object pool for projectile management |
| `TransformClearSystem` | Clears dirty flags after frame |

## Creating Custom Components

Define a class inheriting from `Component`:

```cpp
export module myproject.components.Inventory;

import helios.engine.game.Component;

export class InventoryComponent : public helios::engine::game::Component {
    std::vector<Item> items_;
    
public:
    void addItem(Item item) { items_.push_back(std::move(item)); }
    const std::vector<Item>& items() const { return items_; }
};
```

## Creating Custom Systems

Define a class inheriting from `System`:

```cpp
export module myproject.systems.Spawner;

import helios.engine.game.System;

export class SpawnerSystem : public helios::engine::game::System {
    float timer_ = 0.0f;
    
public:
    void update(UpdateContext& ctx) noexcept override {
        timer_ += ctx.deltaTime();
        if (timer_ > 5.0f) {
            timer_ = 0.0f;
            // Spawn new entity...
        }
    }
};
```

## Update Order

Each frame, the GameWorld executes updates in this order:

1. **GameObject Updates:** All components implementing `Updatable` are updated
2. **System Updates:** All registered Systems are updated in registration order
3. **Post-Frame Cleanup:** Clear systems reset dirty flags

This ordering ensures that component state is computed before systems process global logic.

## Best Practices

**Keep Components Focused:** Each component should represent a single aspect of an entity (health, physics, rendering link). Avoid "god components" that do everything.

**Use Systems for Cross-Cutting Concerns:** Physics simulation, collision detection, and pooling are ideal for Systems because they need access to multiple entities.

**Prefer Composition:** Instead of creating specialized GameObject subclasses, configure entities by attaching different component combinations.

```cpp
// Instead of: class Player : public GameObject { ... }

// Do this:
auto player = std::make_unique<GameObject>();
player->add<SceneNodeComponent>(node);
player->add<Move2DComponent>();
player->add<HealthComponent>();
player->add<PlayerInputComponent>();
```

