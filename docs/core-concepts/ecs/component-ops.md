# ComponentOps

**ComponentOps** is a struct containing function pointers for component lifecycle callbacks. It enables type-erased invocation of lifecycle hooks at runtime while maintaining compile-time type safety during registration.

## Overview

```cpp
struct ComponentOps {
    using OnAcquireFn = void(*)(void*);
    using OnReleaseFn = void(*)(void*);
    using OnRemoveFn = bool(*)(void*);
    using EnableFn = void(*)(void*);
    using DisableFn = void(*)(void*);
    using CloneFn = void*(*)(void* manager, const void* source, const EntityHandle* target);
    using OnActivateFn = void(*)(void*);
    using OnDeactivateFn = void(*)(void*);

    OnAcquireFn onAcquire = nullptr;
    OnReleaseFn onRelease = nullptr;
    OnRemoveFn onRemove = nullptr;
    EnableFn enable = nullptr;
    DisableFn disable = nullptr;
    CloneFn clone = nullptr;
    OnActivateFn onActivate = nullptr;
    OnDeactivateFn onDeactivate = nullptr;
};
```

## Function Pointers

| Pointer | Signature | Purpose |
|---------|-----------|---------|
| `onAcquire` | `void(void*)` | Called when entity acquired from pool |
| `onRelease` | `void(void*)` | Called when entity released to pool |
| `onRemove` | `bool(void*)` | Called before removal, return false to block |
| `enable` | `void(void*)` | Enable component |
| `disable` | `void(void*)` | Disable component |
| `clone` | `void*(void*, const void*, const EntityHandle*)` | Clone component to target entity |
| `onActivate` | `void(void*)` | Called when GameObject activated |
| `onDeactivate` | `void(void*)` | Called when GameObject deactivated |

## Type Erasure Pattern

ComponentOps enables calling methods on components without knowing their concrete type:

```cpp
// At registration time (type known):
ComponentOps ops{
    .onAcquire = [](void* ptr) {
        static_cast<HealthComponent*>(ptr)->onAcquire();
    }
};

// At runtime (type erased):
void* component = getComponent(entityId, typeId);
const auto& ops = ComponentOpsRegistry::ops(typeId);
if (ops.onAcquire) {
    ops.onAcquire(component);  // No type info needed
}
```

## Clone Function

The `clone` function pointer has a special signature for component copying:

```cpp
using CloneFn = void*(*)(
    void* manager,           // EntityManager for emplace
    const void* source,      // Source component data
    const EntityHandle* target  // Target entity handle
);
```

Implementation in ComponentReflector:

```cpp
.clone = [](void* managerRaw, const void* sourceRaw, const EntityHandle* target) -> void* {
    auto* manager = static_cast<EntityManager*>(managerRaw);
    const auto* source = static_cast<const T*>(sourceRaw);
    
    // Copy construct into target entity
    T* cmp = manager->emplace<T>(*target, *source);
    
    // Call onClone if implemented
    if constexpr (traits::HasClone<T>) {
        cmp->onClone(*source);
    }
    
    return cmp;
}
```

## Null Checks

Function pointers are `nullptr` if the component doesn't implement the corresponding hook:

```cpp
const auto& ops = ComponentOpsRegistry::ops(typeId);

// Safe invocation pattern
if (ops.onAcquire) {
    ops.onAcquire(rawComponent);
}

// Or use helper that checks internally
ops.onRemove ? ops.onRemove(raw) : true;
```

---

# ComponentOpsRegistry

The **ComponentOpsRegistry** is a global registry that maps `ComponentTypeId` to `ComponentOps`. It provides O(1) lookup for lifecycle function pointers.

## Overview

```cpp
class ComponentOpsRegistry {
    inline static std::vector<ComponentOps> operations_;
    inline static constexpr ComponentOps emptyOps_{};

public:
    static void setOps(ComponentTypeId typeId, const ComponentOps& ops);
    static const ComponentOps& ops(ComponentTypeId typeId);
};
```

## API

### setOps()

Registers ComponentOps for a type:

```cpp
static void setOps(ComponentTypeId typeId, const ComponentOps& ops);
```

Called by `ComponentReflector::registerType<T>()`:

```cpp
ComponentOps ops{ /* ... */ };
ComponentOpsRegistry::setOps(ComponentTypeId::id<T>(), ops);
```

### ops()

Retrieves ComponentOps for a type:

```cpp
static const ComponentOps& ops(ComponentTypeId typeId);
```

Returns a reference to empty ops if the type is not registered:

```cpp
const auto& ops = ComponentOpsRegistry::ops(typeId);
// ops.onAcquire may be nullptr if not registered
```

## Internal Storage

Uses a type-indexed vector for O(1) access:

```cpp
operations_[typeId.value()] = ops;
```

The vector auto-resizes when new types are registered:

```cpp
if (typeId.value() >= operations_.size()) {
    operations_.resize(typeId.value() + 1);
}
```

## Usage Pattern

```cpp
// During bootstrap (type registration)
helios::engine::bootstrap::registerAllComponents();

// During gameplay (type-erased invocation)
for (auto typeId : entity.componentTypeIds()) {
    const auto& ops = ComponentOpsRegistry::ops(typeId);
    void* raw = entityManager.raw(handle, typeId);
    
    if (ops.onRelease) {
        ops.onRelease(raw);
    }
}
```

## Thread Safety

`ComponentOpsRegistry` is **not thread-safe**. All registration must complete before any concurrent access. In practice, call `registerAllComponents()` once during startup before any systems run.

## See Also

- [Traits](traits.md) - Compile-time hook detection
- [Component Registry](../component-registry.md) - High-level registration API
- [Component Lifecycle](../component-lifecycle.md) - Lifecycle hook details

