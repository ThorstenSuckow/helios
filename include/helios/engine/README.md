# helios::engine

Main engine module aggregating core infrastructure and game systems.

## Overview

This module provides access to the complete helios engine functionality, including the ECS architecture, game systems, messaging infrastructure, and development tooling.

## Submodules

| Module | Purpose |
|--------|---------|
| `helios.engine.bootstrap` | Component registration and GameWorld/GameLoop factory |
| `helios.core` | Data structures, messaging re-exports, units |
| `helios.ecs` | ECS base classes (GameObject, Component, System) and query system |
| `helios.runtime` | Runtime infrastructure (world, gameloop, pooling, messaging, factory) |
| `helios` | Domain-specific components and systems (physics, spatial, scene) |
| `helios.gameplay` | Gameplay mechanics (bounds, combat, spawn, input) |
| `helios.state` | State bindings and transition types |
| `helios.engine.builder` | Builder utilities for entity construction |
| `helios.engine.common` | Shared tags and common types |
| `helios.tooling` | Frame pacing and performance metrics |

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

// Bootstrap creates a pre-configured GameWorld + GameLoop pair
auto [gameWorldPtr, gameLoopPtr] = helios::engine::bootstrap::bootstrapGameWorld();
auto& gameWorld = *gameWorldPtr;
auto& gameLoop  = *gameLoopPtr;

// Register application-specific systems
gameLoop.phase(PhaseType::Main)
    .addPass()
    .addSystem<Move2DSystem>();

// Initialize and run
gameWorld.init();
gameLoop.init(gameWorld);
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine
@brief Main engine module aggregating core infrastructure and game systems.
@details This module provides access to the complete helios engine functionality, including the ECS architecture, game systems, messaging infrastructure, and development tooling.
</p></details>

