# helios::core::data

Core data primitives for identifiers, handles, and type indexing.

This module provides the foundational value types used across the engine
for stable identification and efficient runtime lookups.

## Key Types

| Type | Purpose |
|------|---------|
| `StrongId<Tag>` | Tag-based, strongly-typed identifier with compile-time string hashing (FNV-1a) |
| `RuntimeHandle<StrongIdentifier, RuntimeId>` | Lightweight pair of a stable `StrongId` and a dense runtime index |
| `TypeIndexer` | Compile-time type-to-index mapping |

## See Also

- [Container](../container/README.md) — `DenseRuntimeHandleRegistry` that produces `RuntimeHandle` instances

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::core::data
@brief Core data primitives for identifiers, handles, and type indexing.
@details Provides StrongId (tag-based compile-time identifiers), RuntimeHandle
(stable-ID + dense-index pairs), and TypeIndexer for type-based lookups.
</p></details>



