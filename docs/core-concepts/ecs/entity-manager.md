# EntityManager

The **EntityManager** is the central hub for entity lifecycle management and component storage in the helios ECS. It coordinates between the `EntityRegistry` (handle allocation) and type-specific `SparseSet` containers (component data).

> **Migration note:** `EntityManager` has been moved from `helios.engine.ecs` to `helios.core.ecs` and generalised with template parameters `THandle`, `TEntityRegistry` and `TCapacity`. It no longer holds a reference to a concrete `EntityRegistry` — instead it owns an `EntityRegistry_type` instance internally.

## Overview

`EntityManager` provides a unified interface for:

- **Entity Creation/Destruction** — Delegates handle management to `EntityRegistry`
- **Component Storage** — Maintains type-indexed `SparseSet` containers
- **Component Operations** — Emplace, retrieve, remove, and query components
- **Entity Cloning** — Deep-copy all components from one entity to another

```cpp
// Template parameters make the manager domain-specific
using GameHandle = EntityHandle<GameStrongId>;
using GameReg    = EntityRegistry<GameStrongId>;
using GameEM     = EntityManager<GameHandle, GameReg, 4096>;

GameEM manager;

// Create entity
auto entity = manager.create();

// Add components
auto* transform = manager.emplace<TransformComponent>(entity, position);
auto* health = manager.emplace<HealthComponent>(entity, 100.0f);

// Query and retrieve
if (manager.has<HealthComponent>(entity)) {
    auto* hp = manager.get<HealthComponent>(entity);
}

// Remove component
manager.remove<HealthComponent>(entity);

// Destroy entity (removes all components)
manager.destroy(entity);
```

## Template Parameters

```cpp
template<
    typename THandle,
    typename TEntityRegistry,
    size_t TCapacity
>
class EntityManager;
```

| Parameter | Description |
|-----------|-------------|
| `THandle` | The concrete `EntityHandle<TStrongId>` specialisation. Scopes `ComponentTypeId` and `ComponentOpsRegistry` to this domain. |
| `TEntityRegistry` | The concrete `EntityRegistry` specialisation used for handle allocation and validation. |
| `TCapacity` | Default initial capacity hint for pre-allocating the registry and sparse sets. |

### Type Aliases

| Alias | Resolves To |
|-------|-------------|
| `EntityRegistry_type` | `TEntityRegistry` |
| `Handle_type` | `THandle` |
| `ComponentTypeId_type` | `ComponentTypeId<Handle_type>` |
| `ComponentOpsRegistry_type` | `ComponentOpsRegistry<Handle_type>` |

## Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│              EntityManager<THandle, TEntityRegistry, N>         │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌──────────────────┐     ┌──────────────────────────────────┐  │
│  │  EntityRegistry  │     │  components_ (vector)            │  │
│  │  (TEntityRegistry│     │  ┌─────────────────────────────┐ │  │
│  │   — owned)       │     │  │ [0] SparseSet<Transform>    │ │  │
│  │  ┌────────────┐  │     │  │ [1] SparseSet<Health>       │ │  │
│  │  │VersionId[] │  │     │  │ [2] SparseSet<Renderable>   │ │  │
│  │  │ StrongId[] │  │     │  │ [3] nullptr                 │ │  │
│  │  │ FreeList   │  │     │  │ [4] SparseSet<Collision>    │ │  │
│  │  └────────────┘  │     │  │ ...                         │ │  │
│  └──────────────────┘     │  └─────────────────────────────┘ │  │
│          │                └──────────────────────────────────┘  │
│          ▼                                │                     │
│    Handle Validation                      ▼                     │
│    Create / Destroy              Component Storage              │
│                            (indexed by ComponentTypeId<THandle>)│
└─────────────────────────────────────────────────────────────────┘
```

## API Reference

### Entity Operations

| Method | Description |
|--------|-------------|
| `create()` | Creates a new entity, returns `Handle_type` |
| `destroy(handle)` | Destroys entity and invalidates handle |
| `isValid(handle)` | Checks if handle points to living entity |
| `isValid(entityId)` | Checks if entity ID refers to living entity |
| `handle(entityId)` | Reconstructs handle from entity ID |

### Component Operations

| Method | Description |
|--------|-------------|
| `emplace<T>(handle, args...)` | Constructs component in-place |
| `emplaceOrGet<T>(handle, args...)` | Returns existing or creates new |
| `get<T>(handle)` | Returns pointer or nullptr |
| `has<T>(handle)` | Checks component existence |
| `has(handle, typeId)` | Runtime type ID version |
| `remove<T>(handle)` | Removes component from entity |
| `raw(handle, typeId)` | Returns void* to component data |
| `enable(handle, typeId)` | Enables a component |
| `disable(handle, typeId)` | Disables a component |
| `clone(source, target)` | Clones all components between entities |
| `componentTypeIds(handle)` | Generator over attached component type IDs |
| `getSparseSet<T>()` | Direct access to underlying storage |

### Construction

```cpp
// Default capacity from TCapacity
GameEM manager;

// Custom capacity
GameEM manager(2048);

// With existing registry (deprecated)
GameReg registry;
GameEM manager(registry, 2048);
```

## Component Storage

Components are stored in type-specific `SparseSet` containers:

```cpp
std::vector<std::unique_ptr<SparseSetBase>> components_;
```

- **Index** = `ComponentTypeId<THandle>::id<T>().value()`
- **Value** = `SparseSet<T>` containing all instances of that component type
- **Lazy allocation** — Sets created on first use

### Domain Scoping

Because `ComponentTypeId` is templated on `THandle`, each domain has its
own independent type-ID counter. A `TransformComponent` registered in the
game domain and the UI domain will have separate IDs.

## Lifecycle Hook Integration

EntityManager integrates with the lifecycle system through
`ComponentOpsRegistry<THandle>`:

```cpp
// On component removal
template<typename T>
bool remove(const Handle_type& handle) {
    // ...
    const auto& ops = ComponentOpsRegistry_type::ops(typeId);
    
    // Allow component to intercept removal
    if (ops.onRemove && !ops.onRemove(rawCmp)) {
        return false;  // Removal blocked
    }
    
    return sparseSet->remove(handle.entityId);
}
```

## Thread Safety

`EntityManager` is **not thread-safe**. All operations must be performed from a single thread or externally synchronized.

## Best Practices

1. **Use Entity for game code** — EntityManager is lower-level, prefer `Entity` facade

2. **Batch operations** — Create/destroy entities in batches when possible

3. **Check validity** — Always verify handles before operations

4. **Prefer type-safe methods** — Use `get<T>()` over `raw()` when type is known

5. **Pre-register component types** — Register all types during bootstrap to avoid runtime allocations

## See Also

- [Entity](entity.md) - High-level entity wrapper
- [EntityRegistry](entity-registry.md) - Handle allocation and validation
- [EntityHandle](entity-handle.md) - Versioned entity reference
- [TypedHandleWorld](typed-handle-world.md) - Multi-domain world
- [View](view.md) - Component-based entity queries
- [SparseSet](../sparse-set.md) - Underlying storage
- [Component Lifecycle](../component-lifecycle.md) - Lifecycle hooks
- [Component Registry](../component-registry.md) - Type registration

