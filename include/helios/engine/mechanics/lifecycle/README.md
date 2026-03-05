# helios::engine::mechanics::lifecycle

Lifecycle management for entity components and world-level lifecycle operations.

## Overview

This module provides mechanisms for controlling when components become active during an entity's lifecycle, as well as world-level lifecycle operations such as resetting the game world. The primary use cases are delayed component activation after spawn (collision immunity, staggered wave spawns, effect sequencing) and deferred world reset via the command pipeline.

## Key Classes

| Class | Purpose |
|-------|---------|
| `Active` | Tag component marking an entity as active |
| `Inactive` | Tag component marking an entity as inactive |
| `DeadTagComponent` | Marker component attached on health depletion |
| `DelayedComponentEnabler` | Component that tracks pending activations with timers |
| `DelayedComponentEnablerSystem` | System that decrements timers and enables components |
| `GameObjectLifecycleSystem` | System that reacts to health depletion and despawns entities |
| `WorldLifecycleManager` | Manager that processes deferred world lifecycle commands |
| `WorldLifecycleCommand` | Command carrying a `WorldLifecycleAction` |
| `WorldLifecycleAction` | Enum defining available lifecycle operations (e.g. `Reset`) |

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

## World Lifecycle

The `WorldLifecycleManager` enables deferred world-level operations through the command pipeline. Systems and state listeners submit `WorldLifecycleCommand` instances instead of calling `GameWorld` methods directly.

```cpp
// Request a world reset (e.g. from a game-over state listener)
updateContext.queueCommand<WorldLifecycleCommand>(WorldLifecycleAction::Reset);
```

The manager is auto-registered by `bootstrap::makeGameWorld()` and processes pending commands during its flush cycle. A `Reset` action calls `GameWorld::reset()`, which reinitializes all managers and the session.

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::lifecycle
@brief Lifecycle management for entity components and world-level operations.
@details Provides delayed component activation mechanisms for spawn immunity, staggered effects, and sequenced initialization patterns. Also includes world-level lifecycle management for deferred reset operations through the command pipeline.
</p></details>
