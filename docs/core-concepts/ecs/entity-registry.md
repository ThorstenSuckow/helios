# EntityRegistry

The **EntityRegistry** is the single source of truth for entity lifecycle management in the helios ECS. It handles entity creation, destruction, and validation through versioned handles.

## Overview

The registry is responsible for:

- **Handle Allocation** - Generates unique `EntityHandle` instances
- **Version Tracking** - Maintains version numbers for stale handle detection
- **Index Recycling** - Reuses destroyed entity indices via a free list
- **Validation** - Determines if a handle refers to a living entity

```cpp
EntityRegistry registry;

// Create entity
auto handle = registry.create();  // {entityId: 0, versionId: 1}

// Validate
if (registry.isValid(handle)) {
    // Entity is alive
}

// Destroy
registry.destroy(handle);

// Handle is now stale
assert(!registry.isValid(handle));
```

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                     EntityRegistry                          │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  versions_: [1, 2, 1, 1, 3, ...]                            │
│              │  │  │  │  │                                  │
│              │  │  │  │  └─ Entity 4: version 3 (recycled)  │
│              │  │  │  └──── Entity 3: version 1 (alive)     │
│              │  │  └─────── Entity 2: version 1 (alive)     │
│              │  └────────── Entity 1: version 2 (recycled)  │
│              └───────────── Entity 0: version 1 (alive)     │
│                                                             │
│  freeList_: [1]  ← Recycled indices available for reuse     │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

## API Reference

### Construction

```cpp
// Default constructor
EntityRegistry registry;

// With pre-allocated capacity
EntityRegistry registry(1000);  // Reserve space for 1000 entities
```

### Entity Creation

```cpp
EntityHandle create();
```

Creates a new entity and returns its handle:
- If the free list has recycled indices, one is reused
- Otherwise, a new index is allocated
- New entities start with version 1

```cpp
auto e1 = registry.create();  // {0, 1}
auto e2 = registry.create();  // {1, 1}
auto e3 = registry.create();  // {2, 1}
```

### Entity Destruction

```cpp
bool destroy(const EntityHandle handle);
```

Destroys the entity:
- Increments the version (invalidates existing handles)
- Adds the index to the free list
- Returns `false` if handle was already invalid

```cpp
registry.destroy(e2);
// e2's index (1) is now in free list
// Next create() will reuse index 1 with version 2

auto e4 = registry.create();  // {1, 2} - recycled index
```

### Validation

```cpp
bool isValid(const EntityHandle handle) const noexcept;
```

Checks if a handle refers to a living entity:

```cpp
auto handle = registry.create();
registry.isValid(handle);  // true

registry.destroy(handle);
registry.isValid(handle);  // false (version mismatch)
```

### Version Query

```cpp
VersionId version(EntityId entityId) const;
```

Returns the current version for an entity index:

```cpp
auto handle = registry.create();        // {5, 1}
registry.version(5);                    // 1

registry.destroy(handle);
registry.version(5);                    // 2 (incremented)
```

## Versioning System

The versioning system prevents the **ABA problem**:

```
Timeline:
─────────────────────────────────────────────────────────────
1. create()     → Handle {id:0, ver:1}    versions_[0] = 1
2. destroy()    → Index 0 freed           versions_[0] = 2
3. create()     → Handle {id:0, ver:2}    (recycled)
4. Old handle {id:0, ver:1} is STALE      (ver 1 ≠ ver 2)
─────────────────────────────────────────────────────────────
```

## Index Recycling

Destroyed indices are recycled to keep memory compact:

```cpp
auto a = registry.create();  // {0, 1}
auto b = registry.create();  // {1, 1}
auto c = registry.create();  // {2, 1}

registry.destroy(b);         // freeList_ = [1]
registry.destroy(a);         // freeList_ = [1, 0]

auto d = registry.create();  // {0, 2} - reuses index 0
auto e = registry.create();  // {1, 2} - reuses index 1
```

## Thread Safety

`EntityRegistry` is **not thread-safe**. All operations must be performed from a single thread or externally synchronized.

## Performance

| Operation | Complexity |
|-----------|------------|
| `create()` | O(1) |
| `destroy()` | O(1) |
| `isValid()` | O(1) |
| `version()` | O(1) |

## See Also

- [EntityHandle](entity-handle.md) - Versioned entity reference
- [EntityManager](entity-manager.md) - Combines registry with component storage
- [GameObject](gameobject.md) - High-level entity wrapper

