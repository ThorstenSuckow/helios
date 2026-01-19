# helios::engine::runtime::spawn::behavior::initializers

Concrete SpawnInitializer implementations.

## Overview

This module provides ready-to-use SpawnInitializer implementations for configuring spawned entities.

## Key Classes

| Class | Purpose |
|-------|---------|
| `EmitterInitializer` | Configures entity direction/velocity based on emitter state |
| `RandomDirectionInitializer` | Applies a random 2D direction to spawned entities |

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
    .spawnInitializer = std::make_unique<RandomDirectionInitializer>()
};
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::spawn::behavior::initializers
@brief Concrete SpawnInitializer implementations.
@details Provides ready-to-use initializers for common spawn patterns like emitter-relative and random direction initialization.
</p></details>
