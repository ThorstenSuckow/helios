# helios::engine::modules

Domain-specific components and systems built on top of the ECS architecture.

## Overview

This module provides concrete game logic implementations using the composition-based component model. While `helios.engine.ecs` provides the base classes (GameObject, Component, System), this module contains domain-specific functionality for physics, scene integration, and rendering.

For gameplay mechanics (bounds, combat, spawn, input), see `helios.engine.mechanics`.

## Submodules

| Module | Purpose |
|--------|---------|
| `physics/` | Physics simulation (collision, motion) |
| `spatial/` | Transform management (translation, rotation, scale) |
| `scene/` | Scene graph integration |
| `rendering/` | Renderable components |
| `pool/` | Object pool components |

## Architecture

```
helios.engine.modules
├── physics/
│   ├── collision/   # AABB, collision detection
│   └── motion/      # Movement, steering
├── spatial/
│   └── transform/   # Translation, rotation, scale components
├── scene/           # SceneNode integration
├── rendering/       # Renderable components
└── pool/            # Pool ID components
```

## Relationship to ECS and Runtime

This module **imports** from `helios.engine.ecs`:

- `GameObject`, `Component` — Base classes extended by domain components
- `System` — Base class extended by domain systems

This module **imports** from `helios.engine.runtime.world`:

- `GameWorld` — Used by systems for entity queries
- `Manager` — Extended by domain managers (ProjectileManager)

## Usage Example

```cpp
import helios.engine.modules.physics.motion.components.Move2DComponent;
import helios.engine.modules.physics.collision.components.CollisionComponent;
import helios.engine.modules.scene.components.SceneNodeComponent;

// Add domain components
entity->add<Move2DComponent>();
entity->add<CollisionComponent>(layerId, mask);
entity->add<SceneNodeComponent>(sceneNode);

// Add domain systems to game loop
gameLoop.phase(PhaseType::Main)
    .addPass()
    .addSystem<Move2DSystem>(&gameWorld)
    .addSystem<GridCollisionDetectionSystem>(&gameWorld);
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules
@brief Domain-specific components and systems.
@details This namespace provides concrete game logic implementations built on top of the ECS architecture. It includes physics systems (collision, motion), spatial transform components, scene graph integration, and rendering components. For gameplay mechanics (combat, spawn, bounds), see `helios::engine::mechanics`.
</p></details>
