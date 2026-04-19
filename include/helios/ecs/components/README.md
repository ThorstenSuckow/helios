# helios::ecs::components

Core ECS components for activation state and hierarchy.

## Overview

This submodule contains foundational ECS components used by infrastructure
code instead of gameplay domains. Components are templated by handle type to
support domain-scoped ECS worlds.

## Components

| Component | Purpose |
|-----------|---------|
| `Active<THandle>` | Marker tag for active entities |
| `Inactive<THandle>` | Marker tag for inactive entities |
| `HierarchyComponent<THandle>` | Parent-child relationships and dirty propagation flag |

## Active / Inactive

`Active<THandle>` and `Inactive<THandle>` are empty marker components used in
views for state filtering.

Typical usage:

```cpp
for (auto [entity, health, active] : world.view<MyHandle, HealthComponent, Active<MyHandle>>()) {
    // active entities only
}
```

## HierarchyComponent

`HierarchyComponent<THandle>` stores:

- `children_` — child handles
- `parent_` — optional parent handle
- `isDirty_` — marks pending hierarchy propagation

Typical usage:

```cpp
auto* hc = entity.add<HierarchyComponent<MyHandle>>();
hc->setParent(parentHandle);
hc->addChild(childHandle);
hc->markDirty();
```

## See Also

- [Entity](../Entity.ixx) — facade API often used with these components
- [View](../View.ixx) — typed query interface used for tag filtering

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::ecs::components
@brief Core ECS components for activation state and hierarchy.
@details Provides domain-templated marker and relationship components used by ECS infrastructure.
</p></details>
