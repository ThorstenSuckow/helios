# helios::engine::mechanics::lifecycle

Lifecycle management for entity components with delayed activation support.

## Overview

This module provides mechanisms for controlling when components become active during an entity's lifecycle. The primary use case is delaying component activation after spawn, enabling patterns like collision immunity, staggered wave spawns, and effect sequencing.

## Key Classes

| Class | Purpose |
|-------|---------|
| `DelayedComponentEnabler` | Component that tracks pending activations with timers |
| `DelayedComponentEnablerSystem` | System that decrements timers and enables components |

## Architecture

```
DelayedComponentEnablerInitializer
        │
        ▼ defer(ComponentTypeId, delay)
┌───────────────────────────┐
│  DelayedComponentEnabler  │
│  - deferredComponents_[]  │
│    { delta, typeId }      │
└───────────────────────────┘
        │
        ▼ each frame
┌────────────────────────────────┐
│  DelayedComponentEnablerSystem │
│  - decrement delta             │
│  - enable() when delta <= 0    │
│  - sync() to remove completed  │
└────────────────────────────────┘
```

## Usage

```cpp
// During spawn initialization, defer collision component by 0.5 seconds
auto* enabler = entity.get<DelayedComponentEnabler>();
enabler->defer(ComponentTypeId::id<CollisionComponent>(), 0.5f);

// Or use the initializer for batch spawns with staggered activation
auto initializer = std::make_unique<DelayedComponentEnablerInitializer<
    CollisionComponent,
    RenderableComponent
>>(0.1f);  // 0.1s delay between each entity in batch
```

## Common Patterns

**Collision Immunity**
```cpp
// Projectiles ignore collisions for 0.2s after spawn
DelayedComponentEnablerInitializer<CollisionComponent>(0.2f)
```

**Staggered Wave Spawns**
```cpp
// 10 enemies appear one by one, 0.5s apart, then pattern repeats
DelayedComponentEnablerInitializer<RenderableComponent, AIComponent>(0.5f, 10)
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::lifecycle
@brief Lifecycle management for entity components.
@details Provides delayed component activation mechanisms for spawn immunity, staggered effects, and sequenced initialization patterns.
</p></details>
