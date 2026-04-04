# helios::core::ecs

Generic, reusable ECS primitives shared across the engine.

## Overview

This module provides the low-level building blocks for entity management and
component storage that are independent of any specific engine subsystem.
The types defined here are policy-based templates that can be specialised
for different domains (e.g. game entities, rendering resources, scene handles).

Higher-level, engine-specific ECS facilities (e.g. `GameObject`, `View`,
`EntityManager`) are located in `helios.engine.ecs`.

## Key Types

| Class | Purpose |
|-------|---------|
| `EntityHandle<TStrongId>` | Versioned, strongly-typed handle for safe entity references |
| `EntityRegistry<TStrongId, TLookupStrategy, TAllowRemoval>` | Handle allocation, version tracking, and validation |
| `SparseSet<T, TAllowRemoval>` | Generic O(1) sparse set for entity-keyed storage |
| `SparseSetBase` | Type-erased base for polymorphic sparse set access |
| `HashedLookupStrategy` | O(1) hash-based strong ID collision detection |
| `LinearLookupStrategy` | Linear-scan strong ID collision detection for small registries |

## EntityHandle

`EntityHandle` combines an `EntityId`, a `VersionId`, and a domain-specific
`TStrongId` into a single lightweight value. The version enables stale-handle
detection after entity removal.

```cpp
import helios.core.ecs.EntityHandle;

// Handle is a value type — safe to copy and compare
helios::core::ecs::EntityHandle<MyStrongId> handle;
if (handle.isValid()) { /* version >= 1 */ }
```

## EntityRegistry

The `EntityRegistry` is the authoritative source for entity lifecycle
management. It supports:

- **Handle creation** with optional strong ID auto-assignment
- **Validation** via combined version + strong ID checks
- **Destruction** with index recycling through a free list
- **Configurable removal** (`TAllowRemoval = false` disables `destroy()`)
- **Pluggable lookup strategies** for strong ID collision detection

```cpp
import helios.core.ecs.EntityRegistry;

helios::core::ecs::EntityRegistry<MyStrongId> registry(1024);

auto handle = registry.create();
assert(registry.isValid(handle));

registry.destroy(handle);
assert(!registry.isValid(handle));
```

## SparseSet

A generic sparse set mapping `EntityId` to densely packed data of type `T`.
Provides O(1) amortised insertion, lookup, and swap-and-pop removal with
cache-friendly iteration over the dense storage.

```
Sparse:  [  2  |  ×  |  0  |  ×  |  1  ]   (dense index per EntityId)
Dense:   [ T[0](id=2) | T[1](id=4) | T[2](id=0) ]
× = Tombstone
```

The `TAllowRemoval` template parameter guards against accidental removal
in pools that are intended to be append-only.

## Lookup Strategies

`EntityRegistry` delegates strong ID collision detection to a strategy object:

| Strategy | Lookup | Insertion | Removal | Use Case |
|----------|--------|-----------|---------|----------|
| `HashedLookupStrategy` | O(1) avg | O(1) avg | O(1) avg | Default — large registries |
| `LinearLookupStrategy` | O(n) | O(n) | O(1) | Small registries where hash overhead is undesirable |

## See Also

- [Engine ECS](../../engine/ecs/README.md) — `GameObject`, `EntityManager`, `View`, `ComponentReflector`
- [Core Types](../types.ixx) — `EntityId`, `VersionId`, `EntityTombstone`, `StrongId_t`
- [Container](../container/README.md) — `DenseRuntimeHandleRegistry`, `ConceptModelRegistry`

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::core::ecs
@brief Generic, reusable ECS primitives.
@details Provides policy-based templates for entity handle management, version tracking, and sparse-set storage that are independent of any specific engine subsystem.
</p></details>

