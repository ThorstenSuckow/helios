# helios::engine::ecs::query

Entity query utilities for filtering and iterating GameObjects by component.

## Overview

This module provides efficient mechanisms for querying GameObjects based on their component composition. The query system supports lazy evaluation and flexible filtering.

## Key Classes

| Class | Purpose |
|-------|---------|
| `GameObjectFilter` | Bitmask enum for filtering by active state and component enabled state |
| `GameObjectView` | Lazy range adapter for component-based iteration with structured bindings |

## GameObjectFilter

Filter options for entity queries:

| Flag | Effect |
|------|--------|
| `Active` | Only `obj->isActive() == true` |
| `Inactive` | Only `obj->isActive() == false` |
| `ComponentEnabled` | Only objects where queried components are enabled |
| `ComponentDisabled` | Only objects where queried components are disabled |

Filters can be combined: `Active | ComponentEnabled` (default).

## Usage Example

```cpp
import helios.engine.ecs.query.GameObjectFilter;

using helios.engine.ecs.query.GameObjectFilter;

// Default: active objects with enabled components
for (auto* obj : gameWorld.find<Move2DComponent>()) {
    // ...
}

// Custom filter: inactive objects only
for (auto* obj : gameWorld.find<HealthComponent>(GameObjectFilter.Inactive)) {
    // Process inactive entities (e.g., respawn logic)
}

// Using each() for structured bindings
for (auto [obj, move, collision] : gameWorld.find<Move2DComponent, CollisionComponent>().each()) {
    move.get().setVelocity({1.0f, 0.0f, 0.0f});
}
```

## Implementation Note

`GameObjectView` is implemented as a `.h` header instead of a `.ixx` module interface unit due to MSVC Internal Compiler Errors (ICE) when using structured bindings with the `each()` iterator in module contexts.

## Related Modules

- `helios.engine.ecs.GameWorld` — Provides `find<Components...>()` methods
- `helios.engine.ecs.GameObject` — Entity container being queried
- `helios.engine.ecs.Component` — Base class for component types

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::ecs::query
@brief Entity query utilities for filtering and iterating GameObjects.
@details This module provides efficient mechanisms for querying GameObjects based on their component composition. It includes GameObjectFilter for filtering by active/enabled state and GameObjectView for lazy component-based iteration with structured binding support.
</p></details>
