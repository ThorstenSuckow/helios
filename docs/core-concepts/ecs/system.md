# System

A **System** is an abstract base class for game logic processors that operate on the GameWorld. Systems implement cross-cutting concerns like physics, collision detection, or AI.

## Overview

Systems follow the ECS pattern of separating behavior from data:

- **Components** hold data
- **Systems** implement behavior

```cpp
class PhysicsSystem : public System {
public:
    void update(UpdateContext& ctx) noexcept override {
        for (auto [entity, transform, velocity] : gameWorld_->view<
            TransformComponent,
            VelocityComponent,
            Active
        >().whereEnabled()) {
            transform->position += velocity->direction * ctx.deltaTime;
        }
    }
};

// Register with GameWorld
gameWorld.addSystem(std::make_unique<PhysicsSystem>());
```

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                        GameWorld                            │
│  ┌─────────────────────────────────────────────────────────┐│
│  │                    Systems                              ││
│  │  ┌──────────┐ ┌──────────┐ ┌──────────┐                 ││
│  │  │ Physics  │ │Collision │ │    AI    │  ...            ││
│  │  │ System   │ │  System  │ │  System  │                 ││
│  │  └────┬─────┘ └────┬─────┘ └────┬─────┘                 ││
│  │       │            │            │                       ││
│  │       └────────────┼────────────┘                       ││
│  │                    ▼                                    ││
│  │              GameWorld Data                             ││
│  │         (Entities, Components)                          ││
│  └─────────────────────────────────────────────────────────┘│
└─────────────────────────────────────────────────────────────┘
```

## API Reference

### Base Class

```cpp
class System : public Updatable {
protected:
    GameWorld* gameWorld_ = nullptr;

public:
    virtual ~System() = default;
    
    virtual void init(GameWorld& gameWorld) noexcept;
    void update(UpdateContext& updateContext) noexcept override = 0;
};
```

### init()

Called once when the system is added to the GameWorld:

```cpp
void init(GameWorld& gameWorld) noexcept override {
    System::init(gameWorld);  // Sets gameWorld_
    
    // Custom initialization
    physicsWorld_ = createPhysicsWorld();
}
```

### update()

Called every frame with timing and context information:

```cpp
void update(UpdateContext& ctx) noexcept override {
    float dt = ctx.deltaTime;
    
    for (auto [entity, physics] : gameWorld_->view<PhysicsComponent>()) {
        // Update physics simulation
    }
}
```

## UpdateContext

The `UpdateContext` provides frame-specific information:

```cpp
struct UpdateContext {
    float deltaTime;           // Time since last frame (seconds)
    CommandBuffer* commands;   // For deferred operations
    GameWorld* gameWorld;      // Reference to the world
    // ... additional context
};
```

## Creating Systems

### Basic System

```cpp
class GravitySystem : public System {
public:
    void update(UpdateContext& ctx) noexcept override {
        for (auto [entity, velocity, gravity, active] : gameWorld_->view<
            VelocityComponent,
            GravityComponent,
            Active
        >().whereEnabled()) {
            velocity->velocity.y -= gravity->strength * ctx.deltaTime;
        }
    }
};
```

### System with State

```cpp
class SpawnTimerSystem : public System {
    float spawnTimer_ = 0.0f;
    float spawnInterval_ = 2.0f;

public:
    void update(UpdateContext& ctx) noexcept override {
        spawnTimer_ += ctx.deltaTime;
        
        if (spawnTimer_ >= spawnInterval_) {
            spawnTimer_ = 0.0f;
            ctx.commands->submit<SpawnEnemyCommand>();
        }
    }
};
```

### System with Initialization

```cpp
class CollisionSystem : public System {
    std::unique_ptr<SpatialGrid> grid_;

public:
    void init(GameWorld& gameWorld) noexcept override {
        System::init(gameWorld);
        
        // Create spatial partitioning grid
        grid_ = std::make_unique<SpatialGrid>(worldBounds_, cellSize_);
    }
    
    void update(UpdateContext& ctx) noexcept override {
        grid_->clear();
        
        // Populate grid and detect collisions
    }
};
```

## Updatable Interface

`System` inherits from `Updatable`, which defines the update contract:

```cpp
class Updatable {
public:
    virtual ~Updatable() = default;
    virtual void update(UpdateContext& updateContext) noexcept = 0;
};
```

This interface allows systems (and other updatable objects) to be processed uniformly by the game loop.

## Best Practices

### 1. Keep Systems Focused

Each system should have a single responsibility:

```cpp
// Good: Focused systems
class MovementSystem : public System { };
class CollisionSystem : public System { };
class DamageSystem : public System { };

// Bad: God system
class EverythingSystem : public System { };  // Avoid
```

### 2. Use Views for Queries

Prefer views over manual iteration:

```cpp
// Good: View-based query
for (auto [e, t, v] : gameWorld_->view<Transform, Velocity>()) { }

// Avoid: Manual iteration
for (auto& entity : gameWorld_->entities()) {
    if (entity.has<Transform>() && entity.has<Velocity>()) { }
}
```

### 3. Defer Structural Changes

Use commands for entity creation/destruction during update:

```cpp
void update(UpdateContext& ctx) noexcept override {
    for (auto [entity, health, active] : gameWorld_->view<
        HealthComponent, Active
    >().whereEnabled()) {
        if (health->isDead()) {
            // Don't destroy directly during iteration!
            ctx.commands->submit<DespawnCommand>(entity.entityHandle());
        }
    }
}
```

### 4. noexcept Guarantee

System updates must not throw exceptions:

```cpp
void update(UpdateContext& ctx) noexcept override {
    // All code paths must be exception-safe
}
```

## See Also

- [Updatable](updatable.md) - Base interface for per-frame updates
- [View](view.md) - Component-based entity queries
- [GameWorld](../gameloop-architecture.md) - System registration and execution
- [Command System](../command-system.md) - Deferred operations
- [Component System](../component-system.md) - ECS overview


