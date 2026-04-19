# helios::gameplay::spawn::behavior::initializers

Concrete `SpawnInitializer<THandle>` implementations.

## Overview

This module provides ready-to-use `SpawnInitializer<THandle>` implementations
for configuring spawned entities after placement.

## Key Classes

| Class | Purpose |
|-------|---------|
| `EmitterInitializer<THandle>` | Configures entity direction/velocity based on emitter state |
| `MoveInitializer<THandle>` | Sets movement direction using various strategies (random, axis, point) |
| `RandomDirectionInitializer<THandle>` | Assigns a random movement direction |
| `DelayedComponentEnablerInitializer<THandle, ...ComponentTypes>` | Defers component activation with configurable delay |
| `InitializerList<THandle, N>` | Composite: chains `N` initializers in sequence |

## Usage

```cpp
using Handle = GameObjectHandle;

// Projectiles inherit direction from firing entity
auto profile = SpawnProfile<Handle>{
    .entityPoolId = bulletPoolId,
    .spawnPlacer = std::make_unique<EmitterSpawnPlacer<Handle>>(),
    .spawnInitializer = std::make_unique<EmitterInitializer<Handle>>()
};

// Enemies spawn with random movement directions
auto enemyProfile = SpawnProfile<Handle>{
    .entityPoolId = enemyPoolId,
    .spawnPlacer = std::make_unique<RandomSpawnPlacer<Handle>>(),
    .spawnInitializer = std::make_unique<RandomDirectionInitializer<Handle>>()
};
```

## Composite Initialization

`InitializerList<THandle, N>` executes multiple initializers in sequence:

```cpp
auto initializer = std::make_unique<InitializerList<Handle, 2>>(
    std::make_unique<MoveInitializer<Handle>>(...),
    std::make_unique<DelayedComponentEnablerInitializer<Handle, CollisionComponent>>(0.5f)
);
```

## Delayed Component Activation

`DelayedComponentEnablerInitializer<THandle, ...ComponentTypes>` works with
entities that have a `DelayedComponentEnabler` component attached. It supports:

- **Fixed delay:** All spawned entities activate after the same duration
- **Staggered delay:** Sequential entities in a wave activate progressively

```cpp
// Staggered activation: each entity in wave activates progressively
auto staggeredInit = std::make_unique<
    DelayedComponentEnablerInitializer<Handle, CollisionComponent>
>(0.1f, 5);  // base delay per entity, cycle length
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::gameplay::spawn::behavior::initializers
@brief Concrete SpawnInitializer implementations.
@details Provides ready-to-use initializers for common spawn patterns like emitter-relative initialization, directional movement, and delayed component activation.
</p></details>
