# EntityHandle

An **EntityHandle** is a lightweight, versioned, strongly-typed reference to an entity in the ECS. It combines an entity ID with a version number and a domain-specific strong ID to provide safe references that can detect stale (deleted) entities.

> **Migration note:** `EntityHandle` has been moved from `helios.engine.ecs` to `helios.core.ecs` and generalised with a `TStrongId` template parameter, adding domain-specific strong ID support alongside the existing entity ID and version.

## Overview

```cpp
template<typename TStrongId>
requires IsStrongIdLike<TStrongId>
struct EntityHandle {
    EntityId  entityId;    // Unique identifier within the registry
    VersionId versionId;   // Version for stale detection
    TStrongId strongId;    // Domain-specific strong ID
};
```

The handle is designed to be:
- **Lightweight** — Entity ID + Version + Strong ID
- **Copyable** — Safe to pass by value
- **Hashable** — Can be used as a key in `std::unordered_map`
- **Comparable** — Supports equality and ordering operators
- **Domain-aware** — The `TStrongId` carries domain semantics

## Template Parameters

```cpp
template<typename TStrongId>
requires helios::core::data::concepts::IsStrongIdLike<TStrongId>
struct EntityHandle;
```

| Parameter | Description |
|-----------|-------------|
| `TStrongId` | A strong ID type satisfying the `IsStrongIdLike` concept. Carries domain-specific semantics (e.g. `GameStrongId`, `UiStrongId`). Must provide `isValid()` and `value()`. |

The `TStrongId` parameter ensures that handles from different domains are
distinct types at compile time — a `EntityHandle<GameStrongId>` cannot be
accidentally mixed with an `EntityHandle<UiStrongId>`.

## Versioned References

The version number solves the **ABA problem** in entity systems:

1. Entity A is created at index 5 with version 1
2. Entity A is destroyed — index 5 is recycled, version becomes 2
3. Entity B is created at index 5 with version 2
4. Old handles to Entity A (index 5, version 1) are now **stale**

```cpp
EntityRegistry<MyStrongId> registry;

auto handleA = registry.create();  // {entityId: 5, versionId: 1, strongId: ...}
registry.destroy(handleA);

auto handleB = registry.create();  // {entityId: 5, versionId: 2, strongId: ...}

// handleA is now stale — same index, different version
assert(!registry.isValid(handleA));  // false
assert(registry.isValid(handleB));   // true
```

## Strong ID Validation

In addition to version checking, `EntityRegistry::isValid()` also verifies
that the strong ID stored in the handle matches the one in the registry:

```cpp
// All three must match for the handle to be valid:
// 1. versionId matches registry version
// 2. strongId matches registry strong ID
// 3. strongId.isValid() returns true
```

## API Reference

### Construction

```cpp
// Default constructor (invalid handle)
EntityHandle<MyStrongId> handle;

// Explicit construction
EntityHandle<MyStrongId> handle{entityId, versionId, strongId};
```

### Comparison

```cpp
EntityHandle<MyStrongId> a{1, 1, id1};
EntityHandle<MyStrongId> b{1, 1, id1};
EntityHandle<MyStrongId> c{2, 1, id2};

a == b;  // true — same entity ID, version and strong ID
a == c;  // false — different entity ID
a < c;   // true — compares by entity ID
```

### Validity Check

```cpp
EntityHandle<MyStrongId> handle{5, 1, strongId};
handle.isValid();  // true (version >= 1)

EntityHandle<MyStrongId> invalid{};
invalid.isValid();  // false (default version is 0)
```

> **Note:** `isValid()` only checks if the handle *could* be valid (version >= 1). To check if the entity actually exists, use `EntityRegistry::isValid()` or `EntityManager::isValid()`.

### Hashing

EntityHandle provides a `std::hash` specialization for use in hash-based containers:

```cpp
std::unordered_map<EntityHandle<MyStrongId>, std::string> entityNames;
entityNames[handle] = "Player";

std::unordered_set<EntityHandle<MyStrongId>> selectedEntities;
selectedEntities.insert(handle);
```

## Memory Layout

`TStrongId` wraps a single `StrongId_t` (`uint32_t`), so the typical
total size is **12 bytes** (before padding):

```
┌──────────────────────────────────────────────────────┐
│                    EntityHandle<TStrongId>           │
├──────────────────┬──────────────────┬────────────────┤
│    entityId      │    versionId     │   strongId     │
│   (uint32_t)     │   (uint32_t)     │  (uint32_t)    │
│    32 bits       │    32 bits       │   32 bits      │
└──────────────────┴──────────────────┴────────────────┘
               Total: 12 bytes (96 bits)
```

> **Note:** The actual size of `TStrongId` depends on its concrete type.
> The built-in `StrongId<Tag>` holds a single `uint32_t` (32 bits), but
> custom strong ID types may differ.

## Best Practices

1. **Store handles, not pointers** — Handles survive entity recycling detection

2. **Validate before use** — Always check `registry.isValid(handle)` before accessing components

3. **Pass by value** — Handles are small enough to copy efficiently

4. **Don't rely on ordering** — Entity IDs may be recycled in any order

5. **Use distinct strong ID types per domain** — Prevents accidental cross-domain handle usage

## See Also

- [EntityRegistry](entity-registry.md) - Handle allocation and validation
- [EntityManager](entity-manager.md) - Component storage
- [Entity](entity.md) - High-level entity wrapper
- [TypedHandleWorld](typed-handle-world.md) - Multi-domain world
