# System & TypedSystem

**System** is the abstract base class for game logic processors. **TypedSystem\<T\>** is an internal wrapper that adapts plain system classes to the System interface via composition. Concrete systems are plain classes — they do not inherit from System.

## Overview

Systems follow the ECS pattern of separating behavior from data:

- **Components** hold data
- **Systems** implement behavior

```cpp
class PhysicsSystem {
public:
    void update(UpdateContext& ctx) noexcept {
        for (auto [entity, transform, velocity, active] : ctx.view<
            TransformComponent,
            VelocityComponent,
            Active
        >().whereEnabled()) {
            transform->position += velocity->direction * ctx.deltaTime();
        }
    }
};
```

Systems are registered via `SystemRegistry::add<T>()` which wraps them in a `TypedSystem<T>` internally.

## Architecture

```
┌────────────────────────────────────────────────────────────────┐
│                       SystemRegistry                           │
│  ┌──────────────────┐  ┌──────────────────┐                    │
│  │ TypedSystem<Phys>│  │ TypedSystem<Coll>│  ...               │
│  │  ┌────────────┐  │  │  ┌────────────┐  │                    │
│  │  │PhysicsSys  │  │  │  │CollisionSys│  │  (plain classes)   │
│  │  └────────────┘  │  │  └────────────┘  │                    │
│  └──────────────────┘  └──────────────────┘                    │
│           │                     │                              │
│           └─────────┬───────────┘                              │
│                     ▼                                          │
│              GameWorld Data                                     │
│         (Entities, Components)                                  │
└────────────────────────────────────────────────────────────────┘
```

## Class Hierarchy

```
System                           (abstract, internal infrastructure)
  └── TypedSystem<T>             (composition wrapper, owns T by value)
        └── T: PhysicsSystem     (plain class, provides update())
        └── T: CollisionSystem   (plain class, provides update() + init())
```

User code defines plain classes. `TypedSystem<T>` and `System` are infrastructure-only.

## API Reference

### System (Abstract Base — Internal)

```cpp
namespace helios::engine::runtime::world {

    class System {
    public:
        virtual ~System() = default;
        virtual void initSystem(GameWorld& gameWorld) noexcept = 0;
        virtual void updateSystem(UpdateContext& updateContext) noexcept = 0;
    };
}
```

`System` defines the virtual interface used by the game loop. User code does **not** derive from it.

### TypedSystem (Composition Wrapper — Internal)

```cpp
namespace helios::engine::runtime::world {

    template<typename T>
    concept HasInit = requires(T& t, GameWorld& gw) {
        { t.init(gw) } -> std::same_as<void>;
    };

    template<typename T>
    concept HasUpdate = requires(T& t, UpdateContext& ctx) {
        { t.update(ctx) } -> std::same_as<void>;
    };

    template<typename T>
    class TypedSystem : public System {
        T system_;                              // owned by value

        void initSystem(GameWorld&) noexcept final;
        void updateSystem(UpdateContext&) noexcept final;

    public:
        T& underlying() noexcept;
        const T& underlying() const noexcept;
    };
}
```

`TypedSystem<T>` bridges `System` and the concrete type T:

- **`initSystem()`** conditionally calls `T::init(GameWorld&)` if `HasInit<T>` is satisfied.
- **`updateSystem()`** delegates to `T::update(UpdateContext&)` (enforced via `static_assert`).

Both overrides are `final`.

### SystemRegistry

```cpp
namespace helios::engine::runtime::world {

    class SystemRegistry {
    public:
        template<typename T, typename... Args>
        T& add(Args&&... args);

        template<typename T>
        [[nodiscard]] T* getSystem() const;

        template<typename T>
        [[nodiscard]] bool hasSystem() const;

        [[nodiscard]] std::span<System* const> systems() noexcept;
    };
}
```

`SystemRegistry` creates and owns `TypedSystem<T>` wrappers. `add<T>()` returns a reference to the underlying T instance.

### Required: update()

Every concrete system must provide:

```cpp
void update(UpdateContext& ctx) noexcept {
    // Per-frame logic
}
```

### Optional: init()

Systems that need one-time initialization can optionally define:

```cpp
void init(GameWorld& world) {
    // Called once when the system is initialized
}
```

Detection uses the `HasInit` concept — no runtime cost if omitted.

## Creating Systems

### Basic System

```cpp
class GravitySystem {
public:
    void update(UpdateContext& ctx) noexcept {
        for (auto [entity, velocity, gravity, active] : ctx.view<
            VelocityComponent,
            GravityComponent,
            Active
        >().whereEnabled()) {
            velocity->velocity.y -= gravity->strength * ctx.deltaTime();
        }
    }
};
```

### System with State

```cpp
class SpawnTimerSystem {
    float spawnTimer_ = 0.0f;
    float spawnInterval_ = 2.0f;

public:
    void update(UpdateContext& ctx) noexcept {
        spawnTimer_ += ctx.deltaTime();

        if (spawnTimer_ >= spawnInterval_) {
            spawnTimer_ = 0.0f;
            ctx.queueCommand<SpawnCommand>(position, enemyType);
        }
    }
};
```

### System with Initialization

```cpp
class CollisionSystem {
    std::unique_ptr<SpatialGrid> grid_;

public:
    void init(GameWorld& gameWorld) {
        grid_ = std::make_unique<SpatialGrid>(worldBounds_, cellSize_);
    }

    void update(UpdateContext& ctx) noexcept {
        grid_->clear();
        // Populate grid and detect collisions
    }
};
```

## Registration

Systems are added to game loop passes via `addSystem<T>()`:

```cpp
gameLoop.addPhase("gameplay")
    .addPass<AlwaysRunPass>()
        .addSystem<GravitySystem>()
        .addSystem<CollisionSystem>();
```

## Best Practices

### 1. Keep Systems Focused

Each system should have a single responsibility:

```cpp
// Good: Focused systems
class MovementSystem { void update(UpdateContext&) noexcept; };
class CollisionSystem { void update(UpdateContext&) noexcept; };
class DamageSystem { void update(UpdateContext&) noexcept; };
```

### 2. Use Views for Queries

```cpp
for (auto [e, t, v, a] : ctx.view<Transform, Velocity, Active>().whereEnabled()) { }
```

### 3. Defer Structural Changes

Use commands for entity creation/destruction during update:

```cpp
void update(UpdateContext& ctx) noexcept {
    for (auto [entity, health, active] : ctx.view<
        HealthComponent, Active
    >().whereEnabled()) {
        if (health->isDead()) {
            ctx.queueCommand<DespawnCommand>(entity.entityHandle());
        }
    }
}
```

### 4. noexcept Guarantee

System updates must not throw exceptions.

## See Also

- [View](view.md) - Component-based entity queries
- [Game Loop Architecture](../gameloop-architecture.md) - How systems execute
- [Command System](../command-system.md) - Deferred operations
- [Component System](../component-system.md) - ECS overview
