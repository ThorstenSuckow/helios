# helios::core::types

Foundational type and id utilities shared across the engine.

## Overview

This module provides small, reusable building blocks for strong ids, common
function aliases, and shared type definitions. These types are intentionally
minimal and are used by ECS, rendering, runtime, and platform code.

## Key Types

| Type | Purpose |
|------|---------|
| `StrongId<TTag>` | Strongly-typed id wrapper for domain-scoped identifiers |
| `FuncDefs` | Common `std::function` aliases used across modules |
| `TypeDefs` | Shared integer aliases and small type helpers |

## Notes

- `StrongId` is the base for domain-specific ids such as window, scene, or
  render-resource ids.
- `TypeDefs` stays generic; domain-specific ids live in their respective
  modules.

## See Also

- [Core](../README.md) — core utilities overview
- [Containers](../container/README.md) — registries and handle maps
- [ECS Types](../../ecs/types/README.md) — ECS type layer

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::core::types
@brief Foundational type and id utilities shared across the engine.
</p></details>

