# helios::engine::runtime::spawn::behavior

Spawn behavior strategies for positioning and initializing entities.

## Overview

This module provides the behavior layer for spawn execution. Behavior is split
into two responsibilities:

- `SpawnPlacer<THandle>` computes spawn positions.
- `SpawnInitializer<THandle>` writes initial state to acquired entities.

Both interfaces are template-based on `THandle`.

## Key Types

| Type | Purpose |
|------|---------|
| `SpawnPlacer<THandle>` | Strategy interface for spawn-position computation |
| `SpawnInitializer<THandle>` | Strategy interface for initialization logic |

## Submodules

| Directory | Purpose |
|-----------|---------|
| `placements/` | Concrete placers (random, emitter-relative, axis-distributed) |
| `initializers/` | Concrete initializers (emitter, random direction, move, delayed activation, composite) |

## Behavior Flow

1. `SpawnManager<THandle>` acquires entity from pool.
2. `SpawnPlacer<THandle>` computes spawn position.
3. Position/transform components are updated.
4. `SpawnInitializer<THandle>` applies initial velocity/state.
5. Entity is activated.

## Usage

```cpp
using Handle = GameObjectHandle;

auto profile = std::make_unique<SpawnProfile<Handle>>(SpawnProfile<Handle>{
    .gameObjectPoolId = enemyPoolId,
    .spawnPlacer = std::make_unique<RandomSpawnPlacer<Handle>>(),
    .spawnInitializer = std::make_unique<RandomDirectionInitializer<Handle>>(minSpeed, maxSpeed)
});
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::spawn::behavior
@brief Spawn behavior strategies for positioning and initializing entities.
@details Template-based placement/initialization interfaces used by spawn profiles.
</p></details>
