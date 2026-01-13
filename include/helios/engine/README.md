# helios::engine

Main engine module aggregating core infrastructure and game systems.

## Overview

This module provides access to the complete helios engine functionality, including the ECS architecture, game systems, messaging infrastructure, and development tooling.

## Submodules

| Module | Purpose |
|--------|---------|
| `helios.engine.core` | Messaging (commands, events), pools, units |
| `helios.engine.ecs` | Entity-Component-System base classes (GameObject, Component, System, GameWorld) |
| `helios.engine.game` | Domain-specific components, systems, and gameplay mechanics |
| `helios.engine.runtime.gameloop` | Phase/Pass game loop orchestration |
| `helios.engine.factory` | GameObject creation utilities |
| `helios.engine.facade` | Pool access facade |
| `helios.engine.tooling` | Frame pacing and performance metrics |

## Architecture

```
helios.engine
├── core/          # Infrastructure (messaging, pools, units)
├── ecs/           # ECS base classes and query system
├── game/          # Domain components and systems
│   ├── gameplay/  # Bounds, combat, spawn mechanics
│   ├── physics/   # Collision, motion systems
│   ├── spatial/   # Transform components
│   ├── scene/     # Scene graph integration
│   └── ...
├── gameloop/      # Game loop phases and passes
├── factory/       # GameObject creation
├── facade/        # Pool access
└── tooling/       # Performance monitoring
```

## Usage

```cpp
import helios.engine;

// Access ECS classes
helios.engine.runtime.world.GameWorld world;
auto entity = std.make_unique<helios.engine.ecs.GameObject>();

// Access game systems
world.add<helios.engine.game.Move2DSystem>();

// Access tooling
helios.engine.tooling.FramePacer pacer;
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine
@brief Main engine module aggregating core infrastructure and game systems.
@details This module provides access to the complete helios engine functionality, including the ECS architecture, game systems, messaging infrastructure, and development tooling.
</p></details>

