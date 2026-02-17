# helios::engine::ecs::components

ECS core components for entity relationships and state management.

## Overview

This submodule provides fundamental components used by the ECS architecture itself, as opposed to game-specific components defined in mechanics or modules.

## Components

| Component | Purpose |
|-----------|---------|
| `HierarchyComponent` | Parent-child relationships for entity graphs |

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
| `helios.engine.ecs.systems` | Systems that process ECS components |
| `helios.engine.mechanics.lifecycle` | Active/Inactive tag components |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::ecs::components
@brief ECS core components for entity relationships.
@details Provides fundamental components for the ECS architecture including hierarchy management for parent-child entity relationships.
</p></details>

