# helios::gameplay::spawn::behavior::placements

Concrete `SpawnPlacer<THandle>` implementations.

## Overview

This module provides ready-to-use `SpawnPlacer<THandle>` implementations for
determining spawn positions.

## Key Classes

| Class | Purpose |
|-------|---------|
| `RandomSpawnPlacer<THandle>` | Places entities at random positions within level bounds |
| `EmitterSpawnPlacer<THandle>` | Places entities at the emitter's position |
| `AxisSpawnPlacer<THandle>` | Distributes entities evenly along a normalized axis |

## Usage

```cpp
using Handle = GameObjectHandle;

// Enemies spawn at random locations
auto enemyProfile = SpawnProfile<Handle>{
    .gameObjectPoolId = enemyPoolId,
    .spawnPlacer = std::make_unique<RandomSpawnPlacer<Handle>>(),
    .spawnInitializer = std::make_unique<RandomDirectionInitializer<Handle>>()
};

// Projectiles spawn at the firing entity's position
auto bulletProfile = SpawnProfile<Handle>{
    .gameObjectPoolId = bulletPoolId,
    .spawnPlacer = std::make_unique<EmitterSpawnPlacer<Handle>>(),
    .spawnInitializer = std::make_unique<EmitterInitializer<Handle>>()
};

// Line formation along positive X-axis
auto formationProfile = SpawnProfile<Handle>{
    .gameObjectPoolId = enemyPoolId,
    .spawnPlacer = std::make_unique<AxisSpawnPlacer<Handle>>(
        helios::math::vec3f{1.0f, 0.0f, 0.0f},   // axis (normalized)
        helios::math::vec3f{-100.0f, 0.0f, 0.0f}  // origin
    ),
    .spawnInitializer = std::make_unique<MoveInitializer<Handle>>()
};
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::gameplay::spawn::behavior::placements
@brief Concrete SpawnPlacer implementations.
@details Provides ready-to-use placers for common spawn patterns like random, emitter-relative, and axis-distributed positioning.
</p></details>
