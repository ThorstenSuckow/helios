# EntityHandle

An **EntityHandle** is a lightweight, versioned reference to an entity in the ECS. It combines an entity ID with a version number to provide safe references that can detect stale (deleted) entities.

## Overview

```cpp
struct EntityHandle {
    EntityId entityId;   // Unique identifier within the pool
    VersionId versionId; // Version for stale detection
};
```

The handle is designed to be:
- **Lightweight** - Only 8 bytes (two 32-bit integers)
- **Copyable** - Safe to pass by value
- **Hashable** - Can be used as a key in `std::unordered_map`
- **Comparable** - Supports equality and ordering operators

## Versioned References

The version number solves the **ABA problem** in entity systems:

1. Entity A is created at index 5 with version 1
2. Entity A is destroyed - index 5 is recycled, version becomes 2
3. Entity B is created at index 5 with version 2
4. Old handles to Entity A (index 5, version 1) are now **stale**

```cpp
EntityRegistry registry;

auto handleA = registry.create();  // {entityId: 5, versionId: 1}
registry.destroy(handleA);

auto handleB = registry.create();  // {entityId: 5, versionId: 2}

// handleA is now stale - same index, different version
assert(!registry.isValid(handleA));  // false
assert(registry.isValid(handleB));   // true
```

## API Reference

### Construction

```cpp
// Default constructor (invalid handle)
EntityHandle handle;

// Explicit construction
EntityHandle handle{entityId, versionId};
```

### Comparison

```cpp
EntityHandle a{1, 1};
EntityHandle b{1, 1};
EntityHandle c{2, 1};

a == b;  // true - same entity ID and version
a == c;  // false - different entity ID
a < c;   // true - compares by entity ID
```

### Validity Check

```cpp
EntityHandle handle{5, 1};
handle.isValid();  // true (version >= 1)

EntityHandle invalid{};
invalid.isValid();  // false (default version is 0)
```

> **Note:** `isValid()` only checks if the handle *could* be valid (version >= 1). To check if the entity actually exists, use `EntityRegistry::isValid()` or `EntityManager::isValid()`.

### Hashing

EntityHandle provides a `std::hash` specialization for use in hash-based containers:

```cpp
std::unordered_map<EntityHandle, std::string> entityNames;
entityNames[handle] = "Player";

std::unordered_set<EntityHandle> selectedEntities;
selectedEntities.insert(handle);
```

## Memory Layout

```
┌─────────────────────────────────────┐
│           EntityHandle              │
├──────────────────┬──────────────────┤
│    entityId      │    versionId     │
│    (32 bits)     │    (32 bits)     │
└──────────────────┴──────────────────┘
         Total: 8 bytes
```

## Best Practices

1. **Store handles, not pointers** - Handles survive entity recycling detection

2. **Validate before use** - Always check `registry.isValid(handle)` before accessing components

3. **Pass by value** - Handles are small enough to copy efficiently

4. **Don't rely on ordering** - Entity IDs may be recycled in any order

## See Also

- [EntityRegistry](entity-registry.md) - Handle allocation and validation
- [EntityManager](entity-manager.md) - Component storage
- [GameObject](gameobject.md) - High-level entity wrapper

