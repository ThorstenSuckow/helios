# helios::engine::runtime::world

World state management and per-frame update context.

## Overview

This module provides the core classes for managing game state during runtime. The `GameWorld` class serves as the central registry for all GameObjects, while `UpdateContext` provides the frame context passed to systems during updates.

## Key Classes

| Class | Purpose |
|-------|---------|
| `GameWorld` | Central registry for GameObjects, managers, and pool access |
| `Level` | Game level with world bounds and root scene node |
| `UpdateContext` | Per-frame context with delta time, event buses, and command buffer |
| `SystemRegistry` | Container for System instances within a pass |
| `Manager` | Abstract base for deferred operation handlers |

## Usage

```cpp
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;

// Create world and add entities
helios::engine::runtime::world::GameWorld world;
auto entity = std::make_unique<GameObject>();
world.addGameObject(std::move(entity));

// Query entities by component
for (auto* obj : world.find<ComposeTransformComponent>()) {
    // Process matching entities
}
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::world
@brief World state management and per-frame update context.
@details Provides GameWorld for entity management, UpdateContext for frame state, Level for world bounds, and SystemRegistry for system organization.
</p></details>

