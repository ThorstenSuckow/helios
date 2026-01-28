# helios::engine::runtime::spawn

Entity spawning infrastructure for the helios engine.

## Overview

This module provides a complete spawning pipeline for managing entity lifecycle at runtime. It separates concerns into scheduling (when/how many), behavior (where/how), and execution (pool integration).

## Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                         SPAWN PIPELINE                              │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  POLICY LAYER (when/how many)                                       │
│  ┌─────────────┐    ┌─────────────┐    ┌──────────────┐             │
│  │ SpawnRule   │───>│SpawnCondition───>│AmountProvider│             │
│  └─────────────┘    └─────────────┘    └──────────────┘             │
│         │                                                           │
│         ▼                                                           │
│  SCHEDULING LAYER                                                   │
│  ┌─────────────────────────────────────────┐                        │
│  │           SpawnScheduler                │                        │
│  │  (evaluates rules, produces plans)      │                        │
│  └────────────────────┬────────────────────┘                        │
│                       │                                             │
│                       ▼                                             │
│  COMMAND LAYER                                                      │
│  ┌─────────────────────────────────────────┐                        │
│  │     ScheduledSpawnPlanCommand           │                        │
│  └────────────────────┬────────────────────┘                        │
│                       │                                             │
│                       ▼                                             │
│  EXECUTION LAYER                                                    │
│  ┌─────────────────────────────────────────┐                        │
│  │           SpawnManager                  │                        │
│  │  ┌───────────┐    ┌───────────┐         │                        │
│  │  │SpawnPlacer│    │Initializer│         │                        │
│  │  └───────────┘    └───────────┘         │                        │
│  └─────────────────────────────────────────┘                        │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

## Submodules

| Directory | Purpose |
|-----------|---------|
| `behavior/` | SpawnPlacer and SpawnInitializer interfaces with implementations |
| `commands/` | Spawn and despawn commands for deferred execution |
| `dispatcher/` | Command dispatchers routing to managers |
| `events/` | Frame-level events for spawn confirmation |
| `policy/` | Spawn rules, conditions, and amount providers |
| `scheduling/` | SpawnScheduler and SpawnPlan management |

## Key Classes

| Class | Purpose |
|-------|---------|
| `SpawnManager` | Manager processing spawn/despawn requests via pools |
| `SpawnProfile` | Configuration bundling pool ID, placer, and initializer |
| `SpawnScheduler` | Evaluates rules and produces scheduled spawn plans |
| `SpawnContext` | Context passed to placers/initializers during spawn |
| `EmitterContext` | Source entity state for emitter-relative spawning |

## Usage

```cpp
// 1. Create spawn profile with behaviors
auto profile = std::make_unique<SpawnProfile>(SpawnProfile{
    .gameObjectPoolId = bulletPoolId,
    .spawnPlacer = std::make_unique<EmitterSpawnPlacer>(),
    .spawnInitializer = std::make_unique<EmitterInitializer>()
});

// 2. Register profile with spawn manager
spawnManager.addSpawnProfile(bulletProfileId, std::move(profile));

// 3. Create scheduler with rules
auto scheduler = std::make_unique<SpawnScheduler>();
scheduler->addRule(bulletProfileId, std::make_unique<SpawnRule>(
    std::make_unique<TimerSpawnCondition>(0.1f),
    std::make_unique<FixedSpawnAmount>(1),
    SpawnRuleId{1}
));

// 4. Add spawn system to game loop
gameLoop.phase(PhaseType::Main).addPass()
    .addSystem<GameObjectSpawnSystem>(std::move(scheduler));
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::spawn
@brief Entity spawning infrastructure for the helios engine.
@details Provides a complete spawning pipeline separating scheduling (when/how many), behavior (where/how), and execution (pool integration) concerns.
</p></details>
