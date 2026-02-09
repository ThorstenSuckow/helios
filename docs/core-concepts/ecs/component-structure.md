# Component Structure

This document describes the required structure for components in the helios ECS, including the **mandatory Copy/Move constructors** needed for the prefab/cloning system.

## Overview

Components in helios are **plain data classes** (POD-like) without inheritance from a base class. The ECS uses **compile-time trait detection** to discover lifecycle hooks, enabling zero-overhead optional functionality.

## Basic Component Structure

Every component should follow this structure:

```cpp
class MyComponent {
private:
    // Data members
    float value_;
    bool isEnabled_ = true;

public:
    // 1. Default constructor
    MyComponent() = default;

    // 2. Copy constructor (REQUIRED for cloning)
    MyComponent(const MyComponent& other);

    // 3. Copy assignment operator
    MyComponent& operator=(const MyComponent&) = default;

    // 4. Move constructor (REQUIRED, must be noexcept)
    MyComponent(MyComponent&&) noexcept = default;

    // 5. Move assignment operator
    MyComponent& operator=(MyComponent&&) noexcept = default;

    // 6. Enable/Disable (optional, for view filtering)
    [[nodiscard]] bool isEnabled() const noexcept { return isEnabled_; }
    void enable() noexcept { isEnabled_ = true; }
    void disable() noexcept { isEnabled_ = false; }

    // 7. Lifecycle hooks (optional)
    void onAcquire() noexcept;
    void onRelease() noexcept;
};
```

## Why Copy/Move Constructors Are Required

### The Prefab System

helios uses a **prefab-based spawning system** where template GameObjects (prefabs) are cloned to create new instances:

```
┌─────────────────┐      clone()      ┌─────────────────┐
│     Prefab      │ ─────────────────►│   New Entity    │
│  (Template)     │                   │   (Instance)    │
├─────────────────┤                   ├─────────────────┤
│ HealthComponent │ ──── copy ────────│ HealthComponent │
│ MoveComponent   │ ──── copy ────────│ MoveComponent   │
│ RenderComponent │ ──── copy ────────│ RenderComponent │
└─────────────────┘                   └─────────────────┘
```

When `GameWorld::clone()` or `GameObjectPoolManager::acquire()` is called, **all components are copied** from the prefab to the new entity using their copy constructors.

### Copy Constructor Requirements

The copy constructor is invoked by `ComponentReflector` during cloning:

```cpp
// In ComponentReflector::registerType<T>()
.clone = [](void* managerRaw, const void* sourceRaw, const EntityHandle* target) -> void* {
    auto* manager = static_cast<EntityManager*>(managerRaw);
    const auto* source = static_cast<const T*>(sourceRaw);

    // Copy constructor is called here!
    T* cmp = manager->emplace<T>(*target, *source);

    // onClone() hook called after copy
    if constexpr (traits::HasClone<T>) {
        cmp->onClone(*source);
    }

    return cmp;
}
```

**If your component is not copy-constructible, cloning will fail silently** (returns `nullptr`).

### Move Constructor Requirements

The move constructor **must be `noexcept`** due to how `std::vector` works internally.

Components are stored in `SparseSet<T>`, which uses `std::vector<T>` as its dense storage:

```cpp
// In SparseSet<T>
std::vector<T> storage_;
```

When the vector reallocates (grows), it must move existing elements to new memory. `std::vector` uses:
- **Move constructor** if it's `noexcept`
- **Copy constructor** if move is not `noexcept` (for strong exception guarantee)

**Problem:** If your move constructor is not `noexcept`, the copy constructor is called during reallocation, which can cause:
- Unexpected side effects (e.g., creating duplicate SceneNodes)
- Performance degradation (copying instead of moving)
- Subtle bugs that only appear when the vector grows

```cpp
// BAD: Copy constructor called on vector reallocation!
class BadComponent {
    BadComponent(BadComponent&&) { }  // Not noexcept!
};

// GOOD: Move constructor is noexcept
class GoodComponent {
    GoodComponent(GoodComponent&&) noexcept = default;
};
```

## Complete Component Example

```cpp
/**
 * @brief Component for tracking entity health.
 */
class HealthComponent {
private:
    float maxHealth_{100.0f};
    float health_{0.0f};
    bool isEnabled_ = true;

public:
    // Default constructor
    HealthComponent() = default;

    // Parameterized constructor
    explicit HealthComponent(float maxHealth) : maxHealth_(maxHealth) {}

    // Copy constructor - copies configuration, not runtime state
    HealthComponent(const HealthComponent& other)
        : maxHealth_(other.maxHealth_)
        , isEnabled_(other.isEnabled_) {
        // Note: health_ is NOT copied - it's reset in onAcquire()
    }

    // Copy assignment
    HealthComponent& operator=(const HealthComponent&) = default;

    // Move constructor - MUST be noexcept
    HealthComponent(HealthComponent&&) noexcept = default;

    // Move assignment
    HealthComponent& operator=(HealthComponent&&) noexcept = default;

    // Enable/Disable for view filtering
    [[nodiscard]] bool isEnabled() const noexcept { return isEnabled_; }
    void enable() noexcept { isEnabled_ = true; }
    void disable() noexcept { isEnabled_ = false; }

    // Pool lifecycle hooks
    void onAcquire() noexcept {
        health_ = maxHealth_;  // Reset on spawn
    }

    void onRelease() noexcept {
        health_ = maxHealth_;  // Reset on despawn
    }

    // Getters/Setters
    [[nodiscard]] float health() const noexcept { return health_; }
    [[nodiscard]] float maxHealth() const noexcept { return maxHealth_; }
    [[nodiscard]] bool isAlive() const noexcept { return health_ > 0.0f; }

    void takeDamage(float damage) noexcept {
        health_ = std::max(0.0f, health_ - damage);
    }
};
```

## Copy Constructor Patterns

### Pattern 1: Copy Configuration Only

For components with runtime state that should be reset on clone:

```cpp
class TimerComponent {
    float duration_;      // Configuration - copy this
    float elapsed_{0.0f}; // Runtime state - don't copy

public:
    TimerComponent(const TimerComponent& other)
        : duration_(other.duration_) {
        // elapsed_ starts at 0 for clones
    }
};
```

### Pattern 2: Full Copy with onClone Hook

For components that need post-copy initialization:

```cpp
class SceneNodeComponent {
    SceneNode* sceneNode_;

public:
    // Copy constructor - just copy the pointer (temporary)
    SceneNodeComponent(const SceneNodeComponent& other) = default;

    // onClone - create a NEW SceneNode for this entity
    void onClone(const SceneNodeComponent& source) {
        auto* parent = source.sceneNode_->parent();
        auto node = std::make_unique<SceneNode>(
            source.sceneNode_->shareRenderable()
        );
        sceneNode_ = parent->addNode(std::move(node));
    }
};
```

### Pattern 3: Non-Copyable with Clone Fallback

For components that manage unique resources:

```cpp
class UniqueResourceComponent {
    std::unique_ptr<Resource> resource_;

public:
    // Explicitly deleted - cannot be cloned
    UniqueResourceComponent(const UniqueResourceComponent&) = delete;

    // Move is fine
    UniqueResourceComponent(UniqueResourceComponent&&) noexcept = default;
};
```

> **Note:** Non-copyable components cannot participate in prefab cloning. The `ComponentReflector` will skip them during clone operations.

## Registration

Components must be registered with the `ComponentReflector` before use:

```cpp
// In module registry (e.g., helios/engine/mechanics/health/registry.ixx)
inline void registerComponents() {
    using R = helios::engine::ecs::ComponentReflector;
    R::registerType<HealthComponent>();
}
```

Registration generates `ComponentOps` function pointers for all detected lifecycle hooks.

## Checklist

When creating a new component, verify:

- [ ] Default constructor exists
- [ ] Copy constructor is implemented (or explicitly `= default`)
- [ ] Move constructor is `noexcept` (or `= default`)
- [ ] `isEnabled()` / `enable()` / `disable()` are implemented (if view filtering is needed)
- [ ] Lifecycle hooks (`onAcquire`, `onRelease`, etc.) are implemented as needed
- [ ] Component is registered with `ComponentReflector`

## See Also

- [Component Lifecycle](../component-lifecycle.md) - Lifecycle hooks in detail
- [Traits](traits.md) - Compile-time hook detection
- [ComponentOps](component-ops.md) - Function pointer storage
- [Spawn System](../spawn-system.md) - Prefab and pooling details

