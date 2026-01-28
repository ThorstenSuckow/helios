# helios::engine::runtime::spawn::behavior::initializers

Concrete SpawnInitializer implementations.

## Overview

This module provides ready-to-use SpawnInitializer implementations for configuring spawned entities.

## Key Classes

| Class | Purpose |
|-------|---------|
| `EmitterInitializer` | Configures entity direction/velocity based on emitter state |
| `MoveInitializer` | Sets movement direction using various strategies (random, axis, point) |
| `DelayedComponentEnablerInitializer` | Defers component activation with configurable delay |

## Usage

```cpp
// Projectiles inherit direction from firing entity
auto profile = SpawnProfile{
    .gameObjectPoolId = bulletPoolId,
    .spawnPlacer = std::make_unique<EmitterSpawnPlacer>(),
    .spawnInitializer = std::make_unique<EmitterInitializer>()
};

// Enemies spawn with random movement directions
auto enemyProfile = SpawnProfile{
    .gameObjectPoolId = enemyPoolId,
    .spawnPlacer = std::make_unique<RandomSpawnPlacer>(),
    .spawnInitializer = std::make_unique<MoveInitializer>(DirectionType::Random)
};

// Spawn with collision immunity for 0.5 seconds
auto immuneProfile = SpawnProfile{
    .gameObjectPoolId = enemyPoolId,
    .spawnInitializer = std::make_unique<DelayedComponentEnablerInitializer>(
        0.5f,  // delay in seconds
        std::vector<ComponentTypeId>{
            ComponentTypeId::of<CollisionComponent>()
        }
    )
};
```

## Delayed Component Activation

The `DelayedComponentEnablerInitializer` works with entities that have a `DelayedComponentEnabler` component attached (via `LifecycleBuilder`). It supports:

- **Fixed delay:** All spawned entities activate after the same duration
- **Staggered delay:** Sequential entities in a wave activate progressively

```cpp
// Staggered activation: each entity in wave activates 0.1s after previous
auto staggeredInit = std::make_unique<DelayedComponentEnablerInitializer<CollisionComponent>>(
    0.1f,  // base delay per entity
    5      // cycle length (resets after 5 entities)
);
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::spawn::behavior::initializers
@brief Concrete SpawnInitializer implementations.
@details Provides ready-to-use initializers for common spawn patterns like emitter-relative initialization, directional movement, and delayed component activation.
</p></details>
