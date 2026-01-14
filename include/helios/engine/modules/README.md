# helios::engine::modules

Domain-specific components, systems, and gameplay mechanics built on top of the ECS architecture.

## Overview

This module provides concrete game logic implementations using the composition-based component model. While `helios.engine.ecs` provides the base classes (GameObject, Component, System), this module contains the actual gameplay functionality.

## Submodules

| Module | Purpose |
|--------|---------|
| `gameplay/` | High-level mechanics (bounds, combat, spawn) |
| `physics/` | Physics simulation (collision, motion) |
| `spatial/` | Transform management (translation, rotation, scale) |
| `scene/` | Scene graph integration |
| `rendering/` | Renderable components |
| `model/` | Model/mesh-related components |
| `input/` | Input handling components |
| `pool/` | Object pool components |

## Architecture

```
helios.engine.game
├── gameplay/
│   ├── bounds/      # Level boundary behavior
│   ├── combat/      # Shooting, projectiles
│   └── spawn/       # Entity spawning
├── physics/
│   ├── collision/   # AABB, collision detection
│   └── motion/      # Movement, steering
├── spatial/
│   └── transform/   # Translation, rotation, scale components
├── scene/           # SceneNode integration
├── rendering/       # Renderable components
├── model/           # Model AABB components
├── input/           # Twin-stick input
└── pool/            # Pool ID components
```

## Relationship to ECS

This module **imports** from `helios.engine.ecs`:

- `GameObject`, `Component` — Base classes extended by domain components
- `System` — Base class extended by domain systems
- `GameWorld` — Used by systems for entity queries
- `Manager` — Extended by domain managers (ProjectilePoolManager)

## Usage Example

```cpp
import helios.engine.game;

// Add domain components
entity->add<Move2DComponent>();
entity->add<CollisionComponent>(layerId, mask);
entity->add<SceneNodeComponent>(sceneNode);

// Add domain systems to game loop
gameLoop.phase(PhaseType.Main)
    .addPass()
    .addSystem<Move2DSystem>(gameWorld)
    .addSystem<GridCollisionDetectionSystem>(gameWorld);
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules
@brief Domain-specific components, systems, and gameplay mechanics.
@details This namespace provides concrete game logic implementations built on top of the ECS architecture. It includes physics systems (collision, motion), gameplay mechanics (combat, spawn, bounds), scene integration, and input handling components.
</p></details>
