# helios::gameplay::lifecycle::components

Components for managing entity lifecycle states.

## Overview

This module provides tag components for entity activation state, a marker for dead entities, and a component for scheduling delayed activation of other components after spawn.

## Components

| Component | Description |
|-----------|-------------|
| `Active` | Tag component added to active entities; used as a view filter |
| `Inactive` | Tag component added to inactive entities; used as a view filter |
| `DeadTagComponent` | Marker component attached when health is depleted; move-only |
| `DelayedComponentEnabler` | Tracks components pending delayed activation with countdown timers |

## Active / Inactive

Managed automatically by `GameObject::setActive()`:

- `setActive(true)` → adds `Active`, removes `Inactive`, calls `onActivate()` on components
- `setActive(false)` → adds `Inactive`, removes `Active`, calls `onDeactivate()` on components

Systems filter on these tags via views:

```cpp
// Only process active entities
for (auto [entity, health, active] : gameWorld->view<
    HealthComponent, Active
>().whereEnabled()) { ... }
```

## DeadTagComponent

Attached by `HealthManager` when an entity's health reaches zero and the
`HealthDepletedBehavior::DeadTag` flag is set. Systems can query for this
tag to apply death-related logic (scoring, VFX, cleanup) without
re-checking health values. Move-only; copying is not permitted.

## DelayedComponentEnabler

Schedules delayed activation of other components. Works in conjunction
with `DelayedComponentEnablerSystem`.

```cpp
auto* enabler = entity.get<DelayedComponentEnabler>();
enabler->defer(entity, ComponentTypeId::id<CollisionComponent>(), 0.5f);
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::gameplay::lifecycle::components
@brief Components for managing entity lifecycle states.
@details Provides tag components for activation state, a dead-entity marker, and a component for tracking delayed component activations.
</p></details>
