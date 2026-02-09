# EntityManager

The **EntityManager** is the central hub for entity lifecycle management and component storage in the helios ECS. It coordinates between the `EntityRegistry` (handle allocation) and type-specific `SparseSet` containers (component data).

## Overview

`EntityManager` provides a unified interface for:

- **Entity Creation/Destruction** - Delegates handle management to `EntityRegistry`
- **Component Storage** - Maintains type-indexed `SparseSet` containers
- **Component Operations** - Emplace, retrieve, remove, and query components
- **Entity Cloning** - Deep-copy all components from one entity to another

```cpp
EntityRegistry registry;
EntityManager manager(registry);

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

## Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                        EntityManager                            │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌──────────────────┐     ┌──────────────────────────────────┐  │
│  │  EntityRegistry  │     │  components_ (vector)            │  │
│  │  ┌────────────┐  │     │  ┌─────────────────────────────┐ │  │
│  │  │VersionId[] │  │     │  │ [0] SparseSet<Transform>    │ │  │
│  │  │ FreeList   │  │     │  │ [1] SparseSet<Health>       │ │  │
│  │  └────────────┘  │     │  │ [2] SparseSet<Renderable>   │ │  │
│  └──────────────────┘     │  │ [3] nullptr                 │ │  │
│          │                │  │ [4] SparseSet<Collision>    │ │  │
│          ▼                │  │ ...                         │ │  │
│    Handle Validation      │  └─────────────────────────────┘ │  │
│    Create / Destroy       └──────────────────────────────────┘  │
│                                        │                        │
│                                        ▼                        │
│                               Component Storage                 │
│                               (indexed by TypeId)               │
└─────────────────────────────────────────────────────────────────┘
```

## API Reference

### Entity Operations

| Method | Description |
|--------|-------------|
| `create()` | Creates a new entity, returns `EntityHandle` |
| `destroy(handle)` | Destroys entity and invalidates handle |
| `isValid(handle)` | Checks if handle points to living entity |
| `handle(entityId)` | Reconstructs handle from entity ID |

#### Creating Entities

```cpp
auto entity = manager.create();
// entity.entityId = allocated ID
// entity.version = current version for that slot
```

#### Destroying Entities

```cpp
bool destroyed = manager.destroy(entity);
// Returns false if entity was already invalid
```

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

#### Emplacing Components

```cpp
// Construct with arguments
auto* transform = manager.emplace<TransformComponent>(entity, position, rotation);

// Returns nullptr if component already exists
auto* duplicate = manager.emplace<TransformComponent>(entity);  // nullptr!

// Use emplaceOrGet for upsert behavior
auto* existing = manager.emplaceOrGet<TransformComponent>(entity);
```

#### Retrieving Components

```cpp
// Type-safe retrieval
auto* health = manager.get<HealthComponent>(entity);
if (health) {
    health->takeDamage(10.0f);
}

// Raw pointer access (for reflection/serialization)
void* raw = manager.raw(entity, typeId);
```

#### Removing Components

```cpp
bool removed = manager.remove<HealthComponent>(entity);
// Returns false if:
// - Entity invalid
// - Component not attached
// - onRemove() returned false
```

### Component Queries

```cpp
// Check single component
if (manager.has<HealthComponent>(entity)) { ... }

// Iterate all component types on entity
for (auto typeId : manager.componentTypeIds(entity)) {
    void* component = manager.raw(entity, typeId);
    // Process component...
}
```

### Enable/Disable Components

```cpp
// Enable specific component
manager.enable(entity, typeId);

// Disable specific component
manager.disable(entity, typeId);

// With explicit flag
manager.enable(entity, typeId, false);  // Same as disable
```

### Entity Cloning

Clone all components from one entity to another:

```cpp
auto source = manager.create();
manager.emplace<TransformComponent>(source, position);
manager.emplace<HealthComponent>(source, 100.0f);

auto target = manager.create();
manager.clone(source, target);
// target now has copies of all source's components
```

Cloning uses the `ComponentOps::clone` function pointer, which:
1. Invokes the copy constructor
2. Calls `onClone()` if the component implements it

### SparseSet Access

For advanced use cases, direct access to the underlying storage:

```cpp
// Get typed SparseSet
auto* transforms = manager.getSparseSet<TransformComponent>();
if (transforms) {
    // Iterate all transforms efficiently
    for (const auto& t : *transforms) {
        // Process...
    }
}
```

## Component Storage

Components are stored in type-specific `SparseSet` containers:

```cpp
std::vector<std::unique_ptr<SparseSetBase>> components_;
```

- **Index** = `ComponentTypeId::id<T>().value()`
- **Value** = `SparseSet<T>` containing all instances of that component type
- **Lazy allocation** - Sets created on first use

### Memory Layout

```
components_[0] → SparseSet<TransformComponent>
                 ├── sparse_: [_, 0, _, 1, _, 2]  (entityId → denseIndex)
                 └── storage_: [T₁, T₄, T₆]       (dense, cache-friendly)

components_[1] → SparseSet<HealthComponent>
                 ├── sparse_: [0, _, 1, _]
                 └── storage_: [H₁, H₃]

components_[2] → nullptr  (no entities have this type)
```

## Lifecycle Hook Integration

EntityManager integrates with the lifecycle system:

```cpp
// On component removal
template<typename T>
bool remove(const EntityHandle& handle) {
    // ...
    const auto& ops = ComponentOpsRegistry::ops(typeId);
    
    // Allow component to intercept removal
    if (ops.onRemove && !ops.onRemove(rawCmp)) {
        return false;  // Removal blocked
    }
    
    return sparseSet->remove(handle.entityId);
}
```

## Capacity and Performance

EntityManager is constructed with an initial capacity hint:

```cpp
explicit EntityManager(EntityRegistry& registry, 
                       size_t capacity = ENTITY_MANAGER_DEFAULT_CAPACITY);
```

This capacity is passed to each `SparseSet` to pre-allocate storage, reducing reallocations during gameplay.

## Thread Safety

`EntityManager` is **not thread-safe**. All operations must be performed from a single thread or externally synchronized.

## Best Practices

1. **Use GameObject for game code** - EntityManager is lower-level, prefer `GameObject` facade

2. **Batch operations** - Create/destroy entities in batches when possible

3. **Check validity** - Always verify handles before operations

4. **Prefer type-safe methods** - Use `get<T>()` over `raw()` when type is known

5. **Pre-register component types** - Register all types during bootstrap to avoid runtime allocations

## Example: System Implementation

```cpp
void HealthSystem::update(EntityManager& em, float deltaTime) {
    auto* healthSet = em.getSparseSet<HealthComponent>();
    auto* regenSet = em.getSparseSet<HealthRegenComponent>();
    
    if (!healthSet || !regenSet) return;
    
    for (size_t i = 0; i < healthSet->size(); ++i) {
        auto entityId = healthSet->entityAt(i);
        
        if (regenSet->contains(entityId)) {
            auto* health = healthSet->get(entityId);
            auto* regen = regenSet->get(entityId);
            
            health->heal(regen->rate() * deltaTime);
        }
    }
}
```

## See Also

- [GameObject](gameobject.md) - High-level entity wrapper
- [EntityRegistry](entity-registry.md) - Handle allocation and validation
- [EntityHandle](entity-handle.md) - Versioned entity reference
- [View](view.md) - Component-based entity queries
- [SparseSet](../sparse-set.md) - Underlying storage
- [Component Lifecycle](../component-lifecycle.md) - Lifecycle hooks
- [Component Registry](../component-registry.md) - Type registration


