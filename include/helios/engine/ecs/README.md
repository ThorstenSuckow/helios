# helios::engine::ecs

Core Entity-Component-System architecture for the helios game engine.

## Overview

This module provides the foundational classes for the composition-based game architecture. It separates data (Components) from behavior (Systems). Entity lifecycle and world management are provided by `helios.engine.runtime.world`.

## Key Classes

| Class | Purpose |
|-------|---------|
| `GameObject` | Container for components representing a game entity |
| `Component` | Base class for data containers attached to GameObjects |
| `CloneableComponent` | CRTP base for components that support cloning |
| `System` | Abstract base for logic processors operating on GameWorld |
| `Updatable` | Interface for per-frame updatable objects |

## Submodules

### query/

Entity query utilities for filtering and iterating GameObjects:

| Class | Purpose |
|-------|---------|
| `GameObjectFilter` | Bitmask for filtering by active/enabled state |
| `GameObjectView` | Lazy range for component-based iteration |

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.engine.runtime.world` | GameWorld, Level, UpdateContext, Manager, SystemRegistry |
| `helios.engine.runtime.gameloop` | GameLoop, Phase, Pass |

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
import helios.engine.runtime.world.GameWorld;

// Create game world
helios::engine::runtime::world::GameWorld world;

// Add entity with components
auto entity = std::make_unique<helios::engine::ecs::GameObject>();
entity->add<Move2DComponent>();
entity->add<SceneNodeComponent>(sceneNode);
auto* player = world.addGameObject(std::move(entity));

// Query entities by component
for (auto [obj, move] : world.find<Move2DComponent>().each()) {
    // Process matching entities
}
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::ecs
@brief Core Entity-Component-System architecture.
@details This module provides the foundational classes for the composition-based game architecture. It includes entity base classes (GameObject, Component, CloneableComponent), system infrastructure (System), and query utilities (GameObjectFilter, GameObjectView).
</p></details>

