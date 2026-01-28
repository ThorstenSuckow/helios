# helios::engine::mechanics::bounds

Level boundary behavior components and systems.

This module handles entity behavior when interacting with level boundaries, including bouncing, wrapping, and despawning.

## Submodules

| Submodule | Purpose |
|-----------|---------|
| `components/` | Boundary behavior configuration per entity |
| `systems/` | Boundary check and response logic |

## Key Classes

| Class | Purpose |
|-------|---------|
| `LevelBoundsBehaviorComponent` | Stores behavior type (bounce, wrap, despawn) and restitution |
| `LevelBoundsBehaviorSystem` | Checks bounds and applies configured behavior |

## Behavior Types

| Type | Effect |
|------|--------|
| `Bounce` | Reflects velocity off boundary with restitution |
| `Wrap` | Teleports entity to opposite side |
| `Despawn` | Removes entity from world |
| `Clamp` | Stops at boundary edge |

## Usage

```cpp
// Configure projectile to despawn at boundaries
auto bullet = GameObjectFactory::gameObject()
    .withCollision([](auto& c) {
        c.levelBounds()
         .behavior(LevelBoundsBehavior::Despawn);
    })
    .make(true);

// Configure enemy to bounce off boundaries
auto enemy = GameObjectFactory::gameObject()
    .withCollision([](auto& c) {
        c.levelBounds()
         .behavior(LevelBoundsBehavior::Bounce)
         .restitution(0.8f);  // 80% velocity retained
    })
    .make(true);

// Add system to game loop
gameLoop.phase(PhaseType::Main).addPass()
    .addSystem<LevelBoundsBehaviorSystem>(&gameWorld);
```

---

<details>
<summary>Doxygen</summary><p>
@details This namespace contains components and systems for handling entity interactions with level boundaries. It provides configurable behaviors such as bounce, wrap, clamp, and despawn.
@brief Level boundary behavior components and systems.
@namespace helios::engine::mechanics::bounds
</p></details>

