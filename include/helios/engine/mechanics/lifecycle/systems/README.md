# helios::engine::mechanics::lifecycle::systems

Systems for processing entity lifecycle transitions.

## Overview

This module provides systems that handle lifecycle-related logic each
frame: delayed component activation and entity death processing.

## Systems

| System | Description |
|--------|-------------|
| `DelayedComponentEnablerSystem` | Decrements delay timers and enables components when expired |
| `GameObjectLifecycleSystem` | Reacts to `HealthDepletedEvent` and despawns or deactivates entities |

## DelayedComponentEnablerSystem

Each frame, the system:
1. Iterates all active entities with `DelayedComponentEnabler`
2. Decrements each deferred component's timer by `deltaTime`
3. Enables components whose timers reach zero via `ComponentOpsRegistry`
4. Removes activated entries from the deferred list

## GameObjectLifecycleSystem

Reads `HealthDepletedEvent`s from the pass bus. For each affected entity:
1. Inspects the `HealthDepletedBehavior` flags on the `HealthComponent`
2. If `Despawn` is set and the entity has a `SpawnedByProfileComponent`,
   a `DespawnCommand` is enqueued via the command buffer
3. Otherwise the entity is deactivated via `setActive(false)`

## Scheduling

Both systems should be scheduled in the gameplay phase:

```
DelayedComponentEnablerSystem  → after spawn, before collision
GameObjectLifecycleSystem      → after health manager flush
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::lifecycle::systems
@brief Systems for processing entity lifecycle transitions.
@details Provides frame-based processing of delayed activations, health-depletion reactions, and entity despawning.
</p></details>
