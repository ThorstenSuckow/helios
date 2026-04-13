# helios::ecs::concepts

Compile-time constraints for ECS extension points.

## Overview

This submodule defines concepts that constrain the template parameters used
across `helios.ecs`. They ensure that user-provided types satisfy the
structural requirements expected by the generic ECS infrastructure at compile
time.

## Concepts

| Concept | Purpose |
|---------|---------|
| `IsStrongIdCollisionResolverLike` | Constrains lookup strategies used by `EntityRegistry` for strong ID collision detection |

## IsStrongIdCollisionResolverLike

Requires a type to provide `add`, `remove`, and `has` operations on `StrongId_t`
values, each returning `bool`. Any type satisfying this concept can be used as the
`TLookupStrategy` parameter of `EntityRegistry`.

**Satisfied by:**
- `HashedLookupStrategy` — O(1) amortised, default strategy
- `LinearLookupStrategy` — O(n) scan, suitable for small registries

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.ecs.EntityRegistry` | Primary consumer of the lookup strategy concept |
| `helios.ecs.HashedLookupStrategy` | Hash-based strategy implementation |
| `helios.ecs.LinearLookupStrategy` | Linear-scan strategy implementation |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::ecs::concepts
@brief Compile-time constraints for ECS extension points.
@details Defines concepts that constrain template parameters used by the generic ECS infrastructure, such as lookup strategies for EntityRegistry.
</p></details>

