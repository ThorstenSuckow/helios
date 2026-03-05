# helios::engine::runtime::world

World state management, resource registry, and per-frame update context.

## Overview

This module provides the core classes for managing runtime game state. `GameWorld` is the root container owning the entity system, the `ResourceRegistry` for type-indexed O(1) resource access, the current `Level`, and the `Session`. `UpdateContext` provides the per-frame context passed to systems during updates.

## Key Classes

| Class | Purpose |
|-------|---------|
| `GameWorld` | Central game state container for entities, resources, and the active level |
| `ResourceRegistry` | Type-indexed store for Managers, CommandBuffers, and CommandHandlers with O(1) lookup |
| `Session` | Cross-frame state tracking (game/match states, scores) |
| `Level` | Game level with world bounds and root scene node |
| `UpdateContext` | Per-frame context with delta time, event buses, and `queueCommand<T>()` |
| `SystemRegistry` | Container for System instances within a pass |
| `Manager` | Abstract base for deferred operation handlers |

## Usage

```cpp
// Create world and register resources
helios::engine::runtime::world::GameWorld gameWorld;
auto& poolMgr = gameWorld.registerManager<GameObjectPoolManager>();
auto& spawnMgr = gameWorld.registerManager<SpawnManager>();

gameWorld.init(); // Initializes all Managers in registration order

// Create entities
auto player = gameWorld.addGameObject();
player.add<TransformComponent>(position);
player.setActive(true);

// Query entities via views
for (auto [entity, transform, active] : gameWorld.view<
    TransformComponent, Active
>().whereEnabled()) {
    // Process matching entities
}
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::world
@brief World state management, resource registry, and per-frame update context.
@details Provides GameWorld as the root game state container, ResourceRegistry for type-indexed O(1) resource access, UpdateContext for frame state, Session for cross-frame state, Level for world bounds, and SystemRegistry for system organization.
</p></details>

