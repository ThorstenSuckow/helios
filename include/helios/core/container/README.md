# helios::core::container

Generic container types shared across the engine.

## Overview

This module provides reusable container infrastructure that is not specific
to any engine subsystem. It contains the type-erased registry pattern used
by Systems and Managers, as well as double-buffered message-passing containers
used by the event system.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `ConceptModelRegistry` | Generic type-indexed registry for type-erased wrappers (Concept/Model pattern) |
| `buffer/` | Double-buffering infrastructure for thread-safe message passing |

## Key Type

### ConceptModelRegistry

A generic container that stores type-erased wrapper instances indexed by a
compile-time ID provider. Provides O(1) lookup by concrete type and
insertion-order iteration for deterministic processing.

Used as the backend for:

| Alias | AnyT | IdProvider |
|-------|------|------------|
| `SystemRegistry` | `System` | `SystemTypeId` |
| `ManagerRegistry` | `Manager` | `ResourceTypeId` |

## See Also

- [Buffer](buffer/README.md) — Double-buffered message containers
- [System](../../engine/runtime/world/README.md) — System architecture using SystemRegistry
- [Resource Registry](../../../../docs/core-concepts/resource-registry.md) — ManagerRegistry integration

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::core::container
@brief Generic container types shared across the engine.
</p></details>

