# helios::engine::runtime::spawn::behavior::placements

Concrete SpawnPlacer implementations.

## Overview

This module provides ready-to-use SpawnPlacer implementations for determining spawn positions.

## Key Classes

| Class | Purpose |
|-------|---------|
| `RandomSpawnPlacer` | Places entities at random positions within level bounds |
| `EmitterSpawnPlacer` | Places entities at the emitter's position |

## Usage

```cpp
// Enemies spawn at random locations
auto enemyProfile = SpawnProfile{
    .gameObjectPoolId = enemyPoolId,
    .spawnPlacer = std::make_unique<RandomSpawnPlacer>(),
    .spawnInitializer = std::make_unique<RandomDirectionInitializer>()
};

// Projectiles spawn at the firing entity's position
auto bulletProfile = SpawnProfile{
    .gameObjectPoolId = bulletPoolId,
    .spawnPlacer = std::make_unique<EmitterSpawnPlacer>(),
    .spawnInitializer = std::make_unique<EmitterInitializer>()
};
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::spawn::behavior::placements
@brief Concrete SpawnPlacer implementations.
@details Provides ready-to-use placers for common spawn patterns like random and emitter-relative positioning.
</p></details>
