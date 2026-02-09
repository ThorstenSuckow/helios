# Updatable

The **Updatable** interface defines the contract for objects that require per-frame updates. It is the base interface for `System` and can be implemented by any class that needs to participate in the game loop.

## Overview

```cpp
class Updatable {
public:
    virtual ~Updatable() = default;
    virtual void update(UpdateContext& updateContext) noexcept = 0;
};
```

The interface is intentionally minimal:
- Single pure virtual method `update()`
- `noexcept` guarantee for exception safety in the game loop
- Virtual destructor for proper cleanup

## Usage

### Implementing Updatable

```cpp
class PhysicsSystem : public System {  // System inherits from Updatable
public:
    void update(UpdateContext& ctx) noexcept override {
        // Called every frame
        float dt = ctx.deltaTime;
        // Process physics...
    }
};
```

### UpdateContext

The `update()` method receives an `UpdateContext` with frame information:

```cpp
struct UpdateContext {
    float deltaTime;           // Time since last frame (seconds)
    CommandBuffer* commands;   // For deferred operations
    GameWorld* gameWorld;      // Reference to the world
};
```

## Design Rationale

### noexcept Requirement

The `update()` method is marked `noexcept` because:

1. **Game loop stability** - Exceptions during update can crash the game
2. **Performance** - `noexcept` enables compiler optimizations
3. **Predictability** - Forces error handling at the source

```cpp
void update(UpdateContext& ctx) noexcept override {
    // Must handle all errors internally
    if (auto result = riskyOperation(); !result) {
        LOG_ERROR("Operation failed");
        return;  // Don't throw
    }
}
```

### Interface Segregation

`Updatable` is separate from `System` to allow:

- Non-system objects to be updatable
- Systems that don't need updates (rare)
- Composition over inheritance patterns

## Relationship with System

`System` extends `Updatable` and adds:

- `GameWorld*` member for world access
- `init()` callback for initialization

```cpp
class System : public Updatable {
protected:
    GameWorld* gameWorld_ = nullptr;
    
public:
    virtual void init(GameWorld& gameWorld) noexcept;
    void update(UpdateContext& ctx) noexcept override = 0;
};
```

## See Also

- [System](system.md) - Game logic processor using Updatable
- [Game Loop Architecture](../gameloop-architecture.md) - How updates are dispatched

