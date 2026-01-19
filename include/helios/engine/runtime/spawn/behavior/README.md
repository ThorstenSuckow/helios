# helios::engine::runtime::spawn::behavior

Spawn behavior strategies for positioning and initializing entities.

## Overview

This module provides the behavior layer for the spawn system. It defines abstract interfaces and concrete implementations for determining where entities spawn and how they are initialized.

## Key Classes

| Class | Purpose |
|-------|---------|
| `SpawnPlacer` | Abstract interface for calculating spawn positions |
| `SpawnInitializer` | Abstract interface for configuring spawned entity state |

## Submodules

| Directory | Purpose |
|-----------|---------|
| `placements/` | Concrete SpawnPlacer implementations |
| `initializers/` | Concrete SpawnInitializer implementations |

## Available Placers

| Placer | Description |
|--------|-------------|
| `RandomSpawnPlacer` | Places entities at random positions within level bounds |
| `EmitterSpawnPlacer` | Places entities relative to an emitter position |

## Available Initializers

| Initializer | Description |
|-------------|-------------|
| `EmitterInitializer` | Inherits velocity/direction from emitter |
| `RandomDirectionInitializer` | Applies random velocity within constraints |

## Usage

```cpp
auto profile = std::make_unique<SpawnProfile>(SpawnProfile{
    .gameObjectPoolId = enemyPoolId,
    .spawnPlacer = std::make_unique<RandomSpawnPlacer>(),
    .spawnInitializer = std::make_unique<RandomDirectionInitializer>(minSpeed, maxSpeed)
});
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::spawn::behavior
@brief Spawn behavior strategies for positioning and initializing entities.
@details Provides SpawnPlacer and SpawnInitializer interfaces with concrete implementations for common spawn patterns.
</p></details>
