# helios::engine::ecs

Core Entity-Component-System architecture for the helios game engine.

## Overview

This module provides the foundational classes for the composition-based game architecture. It separates data (Components) from behavior (Systems) and manages entity lifecycle through the GameWorld.

## Key Classes

| Class | Purpose |
|-------|---------|
| `GameObject` | Container for components representing a game entity |
| `Component` | Base class for data containers attached to GameObjects |
| `CloneableComponent` | CRTP base for components that support cloning |
| `System` | Abstract base for logic processors operating on GameWorld |
| `GameWorld` | Root container managing entities, managers, and pools |
| `Manager` | Abstract base for deferred operation handlers |
| `UpdateContext` | Frame context passed to systems during updates |
| `Level` | Game level with world bounds and root scene node |
| `Updatable` | Interface for per-frame updatable objects |
| `SystemRegistry` | Container for System instances within a game loop pass |

## Submodules

### query/

Entity query utilities for filtering and iterating GameObjects:

| Class | Purpose |
|-------|---------|
| `GameObjectFilter` | Bitmask for filtering by active/enabled state |
| `GameObjectView` | Lazy range for component-based iteration |

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                        GameWorld                            │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐  │
│  │ GameObjects │  │  Managers   │  │  Pools/Registry     │  │
│  │  (entities) │  │ (deferred)  │  │  (object recycling) │  │
│  └─────────────┘  └─────────────┘  └─────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
          │                │
          ▼                ▼
┌─────────────────────────────────────────────────────────────┐
│                        GameLoop                             │
│  ┌─────────┐  ┌─────────┐  ┌─────────┐                      │
│  │   Pre   │─>│  Main   │─>│  Post   │  (Phases)            │
│  │ Phase   │  │ Phase   │  │ Phase   │                      │
│  └─────────┘  └─────────┘  └─────────┘                      │
│       │            │            │                           │
│       ▼            ▼            ▼                           │
│    Systems      Systems      Systems     (execute in order) │
└─────────────────────────────────────────────────────────────┘
```

## Usage Example

```cpp
import helios.engine.ecs;

// Create game world
helios.engine.ecs.GameWorld world;

// Add entity with components
auto entity = std.make_unique<GameObject>();
entity->add<Move2DComponent>();
entity->add<SceneNodeComponent>(sceneNode);
auto* player = world.addGameObject(std.move(entity));

// Query entities by component
for (auto [obj, move] : world.find<Move2DComponent>().each()) {
    // Process matching entities
}
```

## Related Modules

- `helios.engine.gameloop` — Game loop orchestration with phases and passes
- `helios.engine.core.messaging` — Command and event systems
- `helios.engine.game` — Domain-specific components and systems

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::ecs
@brief Core Entity-Component-System architecture.
@details This module provides the foundational classes for the composition-based game architecture. It includes entity management (GameObject, GameWorld), component base classes (Component, CloneableComponent), system infrastructure (System, SystemRegistry, Manager), and query utilities (GameObjectFilter, GameObjectView).
</p></details>

