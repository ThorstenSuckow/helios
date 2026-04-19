# EntityRegistry

The **EntityRegistry** is the single source of truth for entity lifecycle management in the helios ECS. It handles entity creation, destruction, and validation through versioned handles with domain-specific strong IDs.

> **Migration note:** `EntityRegistry` has been moved from `helios.engine.ecs` to `helios.core.ecs` and generalised with template parameters `TStrongId`, `TLookupStrategy`, `TAllowRemoval` and `TCapacity`.

## Overview

The registry is responsible for:

- **Handle Allocation** — Generates unique `EntityHandle<TStrongId>` instances
- **Version Tracking** — Maintains version numbers for stale handle detection
- **Strong ID Management** — Tracks domain-specific strong IDs with collision detection
- **Index Recycling** — Reuses destroyed entity indices via a free list
- **Validation** — Determines if a handle refers to a living entity

```cpp
EntityRegistry<MyStrongId> registry;

// Create entity
auto handle = registry.create();  // {entityId: 0, versionId: 1, strongId: ...}

// Validate
if (registry.isValid(handle)) {
    // Entity is alive
}

// Destroy
registry.destroy(handle);

// Handle is now stale
assert(!registry.isValid(handle));
```

## Template Parameters

```cpp
template<
    typename TStrongId,
    typename TLookupStrategy = HashedLookupStrategy,
    bool TAllowRemoval = true,
    size_t TCapacity = DEFAULT_ENTITY_MANAGER_CAPACITY
>
requires IsStrongIdLike<TStrongId> && IsStrongIdCollisionResolverLike<TLookupStrategy>
class EntityRegistry;
```

| Parameter | Default | Description |
|-----------|---------|-------------|
| `TStrongId` | *(required)* | Domain-specific strong ID type satisfying `IsStrongIdLike`. |
| `TLookupStrategy` | `HashedLookupStrategy` | Strategy for strong ID collision detection. |
| `TAllowRemoval` | `true` | If `false`, `destroy()` triggers a `static_assert` — useful for append-only registries. |
| `TCapacity` | `DEFAULT_ENTITY_MANAGER_CAPACITY` | Default initial capacity for pre-allocation. |

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│       EntityRegistry<TStrongId, TLookupStrategy, ...>       │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  versions_:  [1, 2, 1, 1, 3, ...]                          │
│  strongIds_: [5, 0, 3, 7, 0, ...]   (0 = cleared)          │
│               │  │  │  │  │                                 │
│               │  │  │  │  └─ Entity 4: destroyed (v3)       │
│               │  │  │  └──── Entity 3: alive (v1, sid=7)    │
│               │  │  └─────── Entity 2: alive (v1, sid=3)    │
│               │  └────────── Entity 1: destroyed (v2)       │
│               └───────────── Entity 0: alive (v1, sid=5)    │
│                                                             │
│  freeList_: [1, 4]  ← Recycled indices                     │
│                                                             │
│  lookupStrategy_: HashedLookupStrategy                      │
│    → tracks which strong IDs are in use                     │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

## API Reference

### Construction

```cpp
// Default capacity
EntityRegistry<MyStrongId> registry;

// With pre-allocated capacity
EntityRegistry<MyStrongId> registry(1000);

// Custom lookup strategy
EntityRegistry<MyStrongId, LinearLookupStrategy> registry;

// Append-only (no removal)
EntityRegistry<MyStrongId, HashedLookupStrategy, false> registry;
```

### Entity Creation

```cpp
EntityHandle<TStrongId> create(TStrongId strongId = TStrongId{});
```

Creates a new entity and returns its handle:
- If the free list has recycled indices, one is reused
- Otherwise, a new index is allocated
- If `strongId` is invalid (default), one is auto-generated
- Asserts if the strong ID already exists (collision)

```cpp
auto e1 = registry.create();             // auto-assigned strong ID
auto e2 = registry.create(MyStrongId{42}); // explicit strong ID
```

### Entity Destruction

```cpp
bool destroy(EntityHandle<TStrongId> handle);
```

Destroys the entity:
- Increments the version (invalidates existing handles)
- Unregisters the strong ID from the lookup strategy
- Adds the index to the free list
- Returns `false` if handle was already invalid

If `TAllowRemoval` is `false`, triggers a `static_assert`.

### Validation

```cpp
bool isValid(EntityHandle<TStrongId> handle) const noexcept;
```

Checks if a handle refers to a living entity. All three must match:
1. `handle.versionId` matches the stored version
2. `handle.strongId.value()` matches the stored strong ID
3. `handle.strongId.isValid()` returns `true`

### Version Query

```cpp
VersionId version(EntityId entityId) const;
```

Returns the current version for an entity index.

### Strong ID Query

```cpp
TStrongId strongId(EntityId entityId) const;
```

Returns the strong ID for an entity index, or an invalid default if out of bounds.

## Lookup Strategies

| Strategy | Lookup | Insertion | Removal | Use Case |
|----------|--------|-----------|---------|----------|
| `HashedLookupStrategy` | O(1) avg | O(1) avg | O(1) avg | Default — large registries |
| `LinearLookupStrategy` | O(n) | O(n) | O(1) | Small registries where hash overhead is undesirable |

## Performance

| Operation | Complexity |
|-----------|------------|
| `create()` | O(1) |
| `destroy()` | O(1) |
| `isValid()` | O(1) |
| `version()` | O(1) |
| `strongId()` | O(1) |

## Thread Safety

`EntityRegistry` is **not thread-safe**. All operations must be performed from a single thread or externally synchronized.

## See Also

- [EntityHandle](entity-handle.md) - Versioned entity reference
- [EntityManager](entity-manager.md) - Combines registry with component storage
- [Entity](entity.md) - High-level entity wrapper
- [TypedHandleWorld](typed-handle-world.md) - Multi-domain world
