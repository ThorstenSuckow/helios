# helios::engine::mechanics::lifecycle::systems

Systems for processing entity lifecycle transitions.

## Overview

This module provides systems that process lifecycle-related components each frame. The primary system handles delayed component activation by decrementing timers and enabling components when their delay expires.

## Key Classes

| Class | Purpose |
|-------|---------|
| `DelayedComponentEnablerSystem` | Processes delayed activations each frame |

## System Behavior

Each frame, the system:
1. Iterates all entities with `DelayedComponentEnabler`
2. Decrements each deferred component's timer by `deltaTime`
3. Enables components whose timers reach zero
4. Removes activated entries from the deferred list

## Scheduling

Schedule in the gameplay phase after spawn systems but before collision/physics:

```cpp
gameLoop.addSystem<DelayedComponentEnablerSystem>(Phase::Gameplay);
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::lifecycle::systems
@brief Systems for processing entity lifecycle transitions.
@details Provides frame-based processing of delayed activations and lifecycle state changes.
</p></details>
