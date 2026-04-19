# helios::ecs::types

ECS type layer (handles, ids, and type-index helpers).

## Overview

This submodule contains strongly-typed ECS identifiers and runtime helper
structures used by registries, managers, and reflectors.

## Types

| Type | Purpose |
|------|---------|
| `EntityHandle<TStrongId>` | Versioned entity reference scoped by strong-id domain |
| `ComponentTypeId<THandle>` | Domain-scoped component type id generator |
| `ComponentOps` | Type-erased lifecycle callback function set |
| `TypeDefs` (`EntityId`, `VersionId`, `EntityTombstone`) | ECS primitive typedefs and sentinel values |

## See Also

- [Concepts](../concepts/README.md) — constraints used by type templates
- [EntityManager](../EntityManager.ixx) — primary consumer of these types
- [ComponentReflector](../ComponentReflector.ixx) — lifecycle registration via `ComponentOps`

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::ecs::types
@brief ECS type layer (handles, ids, and type-index helpers).
@details Provides identity, typing, and callback abstraction primitives used across ECS containers.
</p></details>

