# Component Lifecycle Hooks

The helios ECS provides **lifecycle hooks** that components can implement to respond to state changes during their lifetime. These hooks are automatically invoked by the engine based on compile-time trait detection.

## Overview

Lifecycle hooks enable components to:

- **Reset state** when acquired from or released to an object pool
- **Intercept removal** to prevent destruction or perform cleanup
- **React to enable/disable** for runtime activation control
- **Initialize after cloning** when spawned from prefabs
- **Respond to activation** when GameObjects become active/inactive

## Available Hooks

| Hook | Signature | When Called |
|------|-----------|-------------|
| `onAcquire` | `void onAcquire()` | GameObject acquired from pool |
| `onRelease` | `void onRelease()` | GameObject returned to pool |
| `onRemove` | `bool onRemove()` | Before component removal |
| `enable` | `void enable()` | Component enabled via `setActive(true)` |
| `disable` | `void disable()` | Component disabled via `setActive(false)` |
| `onClone` | `void onClone(const T&)` | After copy construction during cloning |
| `onActivate` | `void onActivate()` | GameObject activated |
| `onDeactivate` | `void onDeactivate()` | GameObject deactivated |

## Trait Detection

The `ComponentReflector` uses C++20 concepts to detect which hooks a component implements at compile time:

```cpp
// helios/engine/ecs/Traits.ixx

template<typename T>
concept HasOnAcquire = requires(T t) {
    {t.onAcquire()} -> std::same_as<void>;
};

template<typename T>
concept HasOnRelease = requires(T t) {
    {t.onRelease()} -> std::same_as<void>;
};

template<typename T>
concept HasOnRemove = requires(T t) {
    {t.onRemove()} -> std::convertible_to<bool>;
};

template<typename T>
concept HasToogleable = requires(T t) {
    {t.disable()} -> std::same_as<void>;
    {t.enable()} -> std::same_as<void>;
};

template<typename T>
concept HasClone = requires(T t, const T& source) {
    {t.onClone(source)} -> std::same_as<void>;
};

template<typename T>
concept HasActivatable = requires(T t) {
    {t.onActivate()} -> std::same_as<void>;
    {t.onDeactivate()} -> std::same_as<void>;
};
```

## Hook Details

### onAcquire / onRelease

Used by the **object pooling system** to manage component state when GameObjects are reused.

```cpp
class HealthComponent {
    float health_;
    float maxHealth_;

public:
    void onAcquire() noexcept {
        // Reset to full health when spawned from pool
        health_ = maxHealth_;
    }

    void onRelease() noexcept {
        // Clean up before returning to pool
        health_ = maxHealth_;
    }
};
```

**Typical use cases:**
- Reset runtime state (health, timers, counters)
- Clear cached references
- Reinitialize physics state

### onRemove

Called **before** a component is removed from an entity. Return `false` to **prevent removal**.

```cpp
class PersistentComponent {
public:
    bool onRemove() {
        if (hasUnsavedData()) {
            saveData();
        }
        return true; // Allow removal
    }
};

class ProtectedComponent {
public:
    bool onRemove() {
        return false; // Prevent removal
    }
};
```

### enable / disable

Called when a GameObject's active state changes via `setActive()`.

```cpp
class SceneNodeComponent {
    SceneNode* sceneNode_;
    bool isEnabled_ = true;

public:
    void enable() noexcept {
        isEnabled_ = true;
        sceneNode_->setActive(true);
    }

    void disable() noexcept {
        isEnabled_ = false;
        sceneNode_->setActive(false);
    }
};
```

**Important:** Both `enable()` and `disable()` must be implemented together (the `HasToogleable` concept requires both).

### onClone

Called **after** the copy constructor during cloning. Use this for initialization that requires the copy to be complete.

```cpp
class SceneNodeComponent {
    SceneNode* sceneNode_;

public:
    SceneNodeComponent(const SceneNodeComponent&) = default;

    void onClone(const SceneNodeComponent& source) {
        // Create a new SceneNode for this clone
        auto* parent = source.sceneNode_->parent();
        auto node = std::make_unique<SceneNode>(source.sceneNode_->shareRenderable());
        sceneNode_ = parent->addNode(std::move(node));
    }
};
```

**Why onClone?**  
The copy constructor should be simple (for `noexcept` move support). Complex cloning logic with side effects belongs in `onClone()`.

### onActivate / onDeactivate

Called when the GameObject's activation state changes. Both methods must be implemented together.

```cpp
class AIComponent {
public:
    void onActivate() {
        startBehaviorTree();
    }

    void onDeactivate() {
        pauseBehaviorTree();
    }
};
```

## Registration

Hooks are automatically detected when a component is registered:

```cpp
// In your module's registry.ixx
inline void registerComponents() {
    using R = helios::engine::ecs::ComponentReflector;
    
    // Hooks are detected via concepts during registration
    R::registerType<HealthComponent>();      // Has onAcquire, onRelease
    R::registerType<SceneNodeComponent>();   // Has enable, disable, onClone
}
```

## Invocation

The engine invokes hooks through the `ComponentOps` function pointers:

```cpp
// GameObject::setActive() calls enable/disable
void setActive(bool active) {
    for (auto typeId : componentTypeIds()) {
        const auto ops = ComponentOpsRegistry::ops(typeId);
        void* raw = entityManager_->raw(entityHandle_, typeId);

        if (active && ops.enable) {
            ops.enable(raw);
        } else if (!active && ops.disable) {
            ops.disable(raw);
        }
    }
}

// GameObject::onAcquire() calls component onAcquire
void onAcquire() {
    for (auto typeId : componentTypeIds()) {
        const auto ops = ComponentOpsRegistry::ops(typeId);
        void* raw = entityManager_->raw(entityHandle_, typeId);

        if (ops.onAcquire) {
            ops.onAcquire(raw);
        }
    }
}
```

## Best Practices

1. **Keep hooks lightweight** - They are called frequently during pooling and state changes

2. **Use `noexcept`** - Lifecycle hooks should not throw exceptions

3. **Pair enable/disable** - Both must be implemented if either is needed

4. **Reset in onAcquire** - Always reset component state when acquired from pool

5. **Avoid side effects in copy constructor** - Use `onClone()` for complex initialization

6. **Don't rely on hook order** - Components may be processed in any order

## See Also

- [Component Registry](component-registry.md) - How to register components
- [GameObject](ecs/gameobject.md) - High-level entity wrapper
- [EntityManager](ecs/entity-manager.md) - Low-level entity management
- [Component System](component-system.md) - Component fundamentals
- [Sparse Set](sparse-set.md) - Underlying storage and removal mechanics

