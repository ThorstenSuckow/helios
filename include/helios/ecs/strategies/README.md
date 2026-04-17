# helios::ecs::strategies

Lookup strategies for strong-id collision tracking in `EntityRegistry`.

## Overview

This submodule provides strategy implementations for the
`IsStrongIdCollisionResolverLike` contract used by `EntityRegistry`.

## Strategies

| Strategy | Characteristics | Typical Use |
|----------|-----------------|-------------|
| `HashedLookupStrategy` | `std::unordered_set`-based, O(1) average add/remove/has | Default for larger registries |
| `LinearLookupStrategy<TCapacity>` | contiguous vector + linear scan | Small registries with low cardinality |

## See Also

- [Concepts](../concepts/README.md) — strategy constraints
- [EntityRegistry](../EntityRegistry.ixx) — strategy consumer

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::ecs::strategies
@brief Lookup strategies for strong-id collision tracking.
@details Contains pluggable collision lookup implementations used by EntityRegistry.
</p></details>

