# helios::core::container

Generic container types shared across the engine.

## Overview

This module provides reusable container infrastructure that is not specific
to a single subsystem. It contains the type-erased registry pattern used by
systems/managers and generic handle-mapping utilities used across runtime and
rendering code.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `ConceptModelRegistry` | Generic type-indexed registry for type-erased wrappers (Concept/Model pattern) |
| `HandleMultiMap` | Dense one-to-many handle association map with reverse lookup |
| `buffer/` | Buffer and double-buffer infrastructure for frame-based message passing |

## Key Types

### ConceptModelRegistry

A generic container that stores type-erased wrapper instances indexed by a
compile-time ID provider. Provides O(1) lookup by concrete type and
insertion-order iteration for deterministic processing.

Used as backend for multiple registries, for example:

| Alias | AnyT | IdProvider |
|-------|------|------------|
| `SystemRegistry` | `System` | `SystemTypeId` |
| `ManagerRegistry` | `Manager` | `ResourceTypeId` |
| `CommandBufferRegistry` | `CommandBuffer` | `CommandBufferTypeId` |

### HandleMultiMap

`HandleMultiMap<TOneHandle, TManyHandle>` stores a dense one-to-many relation
(`TOneHandle -> span<TManyHandle>`) plus reverse lookup (`TManyHandle -> TOneHandle`).

Typical use case:

- map one render target handle to many child handles,
- resolve owner handle from a child handle in O(1)-style indexed access.

## See Also

- [Buffer](buffer/README.md) — buffer and double-buffer containers
- [Buffer Concepts](buffer/concepts/README.md) — concept constraints for type-indexed buffer utilities
- [Core Types](../types/README.md) — foundational id/type utilities used by container templates
- [Resource Registry](../../../../docs/core-concepts/resource-registry.md) — engine-side registry integration

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::core::container
@brief Generic container types shared across the engine.
</p></details>

