# helios::core::concepts

Shared compile-time constraints used across multiple domains.

## Overview

This submodule centralizes core concepts that are not specific to ECS, rendering,
or other engine subsystems. These constraints help keep templates consistent
across modules.

## Concepts

| Concept | Purpose |
|---------|---------|
| `IsStrongIdLike<T>` | Constrains strong-id style types with `value()`/`isValid()` |

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.ecs` | Uses `IsStrongIdLike` to constrain registry and handle types |
| `helios.core.types` | Provides `StrongId` implementations that satisfy the concept |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::core::concepts
@brief Shared compile-time constraints used across multiple domains.
</p></details>

