# System

**System** is a type-erased wrapper for game logic processors. Concrete systems are plain classes that provide an `update(UpdateContext&)` method — they do not inherit from `System`. The wrapper uses the Concept/Model pattern to erase the concrete type at compile time.

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

Systems are registered via `SystemRegistry::add<T>()` which wraps them in a `System` internally.

## Architecture

```
┌────────────────────────────────────────────────────────────────┐
│                       SystemRegistry                           │
│  ┌──────────────────┐  ┌──────────────────┐                    │
│  │ System           │  │ System           │  ...               │
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

## Type Erasure Design

```
System (value type, owns erased Concept via unique_ptr)
  └── Concept              (internal virtual interface)
        └── Model<T>       (typed wrapper, owns T by value)
              └── T        (plain class, provides update())
```

User code defines plain classes. `System` is infrastructure-only and erases the concrete type.

## API Reference

### System (Type-Erased Wrapper)

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

    class System {
        class Concept { /* virtual update(), init(), underlying() */ };

        template<typename T>
        class Model final : public Concept {
            T system_;   // owned by value
        };

        std::unique_ptr<Concept> pimpl_;

    public:
        template<typename T>
        requires HasUpdate<T>
        explicit System(T system);

        void update(UpdateContext&) noexcept;
        void init(GameWorld&) noexcept;

        [[nodiscard]] void* underlying() noexcept;
        [[nodiscard]] const void* underlying() const noexcept;
    };
}
```

`System` erases the concrete type via the Concept/Model pattern:

- **`update()`** delegates to `T::update(UpdateContext&)` (required via `HasUpdate<T>`).
- **`init()`** conditionally calls `T::init(GameWorld&)` if `HasInit<T>` is satisfied.
- **`underlying()`** returns a type-erased pointer to the owned system instance.

`System` is move-only (non-copyable).

### SystemRegistry

`SystemRegistry` is a type alias for `ConceptModelRegistry<System, SystemTypeId>`:

```cpp
namespace helios::engine::runtime::world {

    using SystemRegistry = ConceptModelRegistry<System, SystemTypeId>;

    // Inherited API from ConceptModelRegistry:
    // template<typename T, typename... Args> T& add(Args&&...);
    // template<typename T> [[nodiscard]] T* item() const;
    // template<typename T> [[nodiscard]] bool has() const;
    // [[nodiscard]] std::span<System* const> items() const noexcept;
}
```

`SystemRegistry` creates and owns `System` wrappers. `add<T>()` returns a reference to the underlying T instance. `items()` returns a span in insertion order for deterministic iteration.

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
