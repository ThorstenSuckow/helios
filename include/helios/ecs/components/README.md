# helios::ecs::components

ECS core components for entity lifecycle and relationships.

## Overview

This submodule provides fundamental components used by the ECS architecture
itself, as opposed to game-specific components defined in mechanics or modules.
It includes tag components for entity activation state as well as hierarchy
management for parent-child entity graphs.

## Components

| Component | Purpose |
|-----------|---------|
| `Active` | Tag component marking an entity as active |
| `Inactive` | Tag component marking an entity as inactive |
| `HierarchyComponent` | Parent-child relationships for entity graphs |

## Active / Inactive

Empty tag structs used to filter entities by activation state in views.
Managed automatically by `Entity::setActive()`:

- `setActive(true)` → adds `Active`, removes `Inactive`, calls `onActivate()`
- `setActive(false)` → adds `Inactive`, removes `Active`, calls `onDeactivate()`

```cpp
// Query only active entities
for (auto [entity, health, active] : gameWorld->view<
    HealthComponent, Active
>().whereEnabled()) { ... }
```

## HierarchyComponent

Enables entities to form tree structures where state changes propagate from parent to child entities.

**Members:**
- `children_` – Child entity handles
- `parent_` – Optional parent handle  
- `isDirty_` – Signals pending hierarchy propagation

**Usage:**
```cpp
auto* hc = entity.add<HierarchyComponent>();
hc->setParent(parentHandle);
hc->addChild(childHandle);
hc->markDirty();  // Triggers propagation next frame
```

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.ecs.Entity` | Manages `Active`/`Inactive` tags via `setActive()` |
| `helios.ecs.systems` | Systems that process ECS components |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::ecs::components
@brief ECS core components for entity lifecycle and relationships.
@details Provides tag components for entity activation state (Active, Inactive) and hierarchy management for parent-child entity relationships.
</p></details>

