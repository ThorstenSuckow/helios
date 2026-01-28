# helios::engine::mechanics::lifecycle::components

Components for managing entity lifecycle states.

## Overview

This module provides components that track and manage the activation state of other components on an entity. The primary use case is delayed activation after spawn.

## Key Classes

| Class | Purpose |
|-------|---------|
| `DelayedComponentEnabler` | Tracks components pending delayed activation with countdown timers |

## Usage

```cpp
// Defer collision component activation by 0.5 seconds
auto* enabler = entity->get<DelayedComponentEnabler>();
enabler->defer(ComponentTypeId::id<CollisionComponent>(), 0.5f);

// Check currently deferred components
for (auto& deferred : enabler->deferredComponents()) {
    // deferred.delta - remaining time
    // deferred.componentTypeId - which component
}
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::lifecycle::components
@brief Components for managing entity lifecycle states.
@details Provides data containers for tracking delayed component activations and lifecycle transitions.
</p></details>
