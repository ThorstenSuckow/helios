# helios::engine

Main engine module aggregating core infrastructure and game systems.

## Overview

This module provides access to the complete helios engine functionality, including the ECS architecture, game systems, messaging infrastructure, and development tooling.

## Submodules

| Module | Purpose |
|--------|---------|
| `helios.engine.core` | Data structures, messaging re-exports, units |
| `helios.engine.ecs` | ECS base classes (GameObject, Component, System) and query system |
| `helios.engine.runtime` | Runtime infrastructure (world, gameloop, pooling, messaging, factory) |
| `helios.engine.modules` | Domain-specific components and systems (physics, spatial, scene) |
| `helios.engine.mechanics` | Gameplay mechanics (bounds, combat, spawn, input) |
| `helios.engine.tooling` | Frame pacing and performance metrics |

## Architecture

```
helios.engine
├── core/              # Infrastructure
│   ├── data/          # Type-indexed containers, pool IDs
│   └── units/         # Measurement units (meters, seconds)
├── ecs/               # ECS base classes
│   ├── Component      # Data container base
│   ├── GameObject     # Entity container
│   ├── System         # Logic processor base
│   └── query/         # GameObjectFilter, GameObjectView
├── runtime/           # Runtime infrastructure
│   ├── world/         # GameWorld, Level, UpdateContext, Manager
│   ├── gameloop/      # GameLoop, Phase, Pass
│   ├── pooling/       # GameObjectPool, PoolRegistry, PoolFacade
│   ├── messaging/     # CommandBuffer, Dispatchers, EventBus
│   └── factory/       # GameObjectFactory
├── modules/           # Domain-specific components and systems
│   ├── physics/       # Collision, motion systems
│   ├── spatial/       # Transform components
│   ├── scene/         # Scene graph integration
│   ├── rendering/     # Renderable components
│   └── pool/          # Pool ID components
├── mechanics/         # Gameplay mechanics
│   ├── bounds/        # Level boundary behavior
│   ├── combat/        # Shooting, projectiles
│   ├── spawn/         # Entity spawning
│   └── input/         # Twin-stick input systems
└── tooling/           # Performance monitoring
```

## Usage

```cpp
import helios.engine;

// Access ECS classes
helios::engine::runtime::world::GameWorld world;
auto entity = std::make_unique<helios::engine::ecs::GameObject>();
world.addGameObject(std::move(entity));

// Access game loop - systems are registered with phases/passes
helios::engine::runtime::gameloop::GameLoop gameLoop;
gameLoop.phase(PhaseType::Main)
    .addPass()
    .addSystem<Move2DSystem>(&world);

// Access tooling
helios::engine::tooling::FramePacer pacer;
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine
@brief Main engine module aggregating core infrastructure and game systems.
@details This module provides access to the complete helios engine functionality, including the ECS architecture, game systems, messaging infrastructure, and development tooling.
</p></details>

