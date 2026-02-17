# helios::engine::ecs::systems

Core ECS systems for entity management and hierarchy propagation.

## Overview

This submodule contains systems that operate on fundamental ECS components. These systems are typically registered early in the game loop to ensure proper entity state before game-specific systems run.

## Systems

| System | Purpose |
|--------|---------|
| `HierarchyPropagationSystem` | Propagates activation state through entity hierarchies |

## HierarchyPropagationSystem

Processes entities with `HierarchyComponent` and propagates their active/inactive state to all descendants.

**Behavior:**

1. Finds root entities (no parent) that are marked dirty
2. For active roots: recursively activates all children
3. For inactive roots: recursively deactivates all children
4. Clears dirty flag after propagation

**Required Components:**
- `HierarchyComponent` – Hierarchy relationships
- `Active` or `Inactive` – Lifecycle tag components

**Usage:**
```cpp
// Register in game loop
systemRegistry.add<HierarchyPropagationSystem>(Phase::Pre);

// When entity activation changes
auto* hc = entity.get<HierarchyComponent>();
hc->markDirty();  // Children will be updated next frame
```

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.engine.ecs.components` | Components processed by these systems |
| `helios.engine.mechanics.lifecycle` | Active/Inactive components |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::ecs::systems
@brief Core ECS systems for entity management.
@details Provides systems that operate on fundamental ECS components, including hierarchy propagation for parent-child entity relationships.
</p></details>

