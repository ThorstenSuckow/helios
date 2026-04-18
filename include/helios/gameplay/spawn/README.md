# helios::gameplay::spawn

Entity spawning infrastructure and lifecycle management for the helios engine.

## Overview

This module provides a complete spawning pipeline for runtime entity lifecycle
management. The architecture separates concerns into:

- **Policy layer** (*when/how many*) — Rules, conditions, amount providers
- **Behavior layer** (*where/how*) — Placement and initialization strategies
- **Scheduling layer** — Evaluates rules, produces spawn plans
- **Command layer** — Typed spawn/despawn command payloads
- **Execution layer** — Pool acquisition/release and command handling via `SpawnManager`

The full stack is template-based on handle type (`THandle`) so spawn logic can
be reused across handle domains with compile-time type safety.

## Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                         SPAWN PIPELINE                              │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  POLICY LAYER (when/how many)                                       │
│  ┌─────────────┐   ┌──────────────┐   ┌─────────────────┐           │
│  │ SpawnRule   │──>│SpawnCondition│──>│SpawnAmount      │           │
│  │ <THandle>   │   │              │   │Provider<THandle>│           │
│  └─────────────┘   └──────────────┘   └─────────────────┘           │
│         │                                                           │
│         ▼                                                           │
│  SCHEDULING LAYER                                                   │
│  ┌─────────────────────────────────────────┐                        │
│  │        SpawnScheduler<THandle>          │                        │
│  │   (evaluates rules, produces plans)     │                        │
│  └────────────────────┬────────────────────┘                        │
│                       │                                             │
│                       ▼                                             │
│  COMMAND LAYER                                                      │
│  ┌─────────────────────────────────────────┐                        │
│  │ ScheduledSpawnPlanCommand<THandle>      │                        │
│  └────────────────────┬────────────────────┘                        │
│                       │                                             │
│                       ▼                                             │
│  GAME LOOP INTEGRATION                                              │
│  ┌─────────────────────────────────────────┐                        │
│  │       GameObjectSpawnSystem             │                        │
│  │  (reads frame events, enqueues commands)│                        │
│  └────────────────────┬────────────────────┘                        │
│                       │                                             │
│                       ▼                                             │
│  EXECUTION LAYER                                                    │
│  ┌─────────────────────────────────────────┐                        │
│  │        SpawnManager<THandle>            │                        │
│  │  ┌────────────────┐ ┌────────────────┐  │                        │
│  │  │SpawnPlacer     │ │SpawnInitializer│  │                        │
│  │  │<THandle>       │ │<THandle>       │  │                        │
│  │  └────────────────┘ └────────────────┘  │                        │
│  └─────────────────────────────────────────┘                        │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

## Submodules

| Directory | Purpose |
|-----------|---------|
| `behavior/` | `SpawnPlacer<THandle>` and `SpawnInitializer<THandle>` abstractions + implementations |
| `commands/` | Typed spawn/despawn command payloads |
| `components/` | Spawn-related components (e.g. `SpawnedByProfileComponent`) |
| `events/` | Spawn pipeline events (e.g. plan execution confirmation) |
| `policy/` | Rules, conditions, and amount providers |
| `scheduling/` | Scheduler implementations and plan production |
| `systems/` | `GameObjectSpawnSystem` for game loop integration |
| `types/` | Shared ids, contexts, profiles, cursors |

## Key Types

| Type | Purpose |
|------|---------|
| `SpawnManager<THandle>` | Executes spawn/despawn command queues and pool operations |
| `SpawnProfile<THandle>` | Binds pool, placer, and initializer for a profile id |
| `SpawnScheduler<THandle>` | Produces scheduled spawn plans from rules |
| `SpawnContext<THandle>` | Context payload for placement/initialization |
| `EmitterContext<THandle>` | Optional emitter/source context |
| `GameObjectSpawnSystem` | System reading frame events and enqueuing spawn commands |

## Related Modules

- `helios.runtime.pooling` — `GameObjectPoolManager`

## Usage

```cpp
using Handle = GameObjectHandle;

// 1. Register a spawn profile with placer and initializer
auto profile = std::make_unique<SpawnProfile<Handle>>(SpawnProfile<Handle>{
    .gameObjectPoolId = bulletPoolId,
    .spawnPlacer = std::make_unique<EmitterSpawnPlacer<Handle>>(),
    .spawnInitializer = std::make_unique<EmitterInitializer<Handle>>()
});

auto& spawnManager = gameWorld.registerManager<SpawnManager<Handle>>();
spawnManager.addSpawnProfile(bulletProfileId, std::move(profile));

// 2. Create spawn rules with conditions
auto scheduler = std::make_unique<SpawnScheduler<Handle>>();
scheduler->addRule(
    SpawnRuleId{1},
    std::make_unique<TimerSpawnCondition>(2.0f),  // Every 2 seconds
    enemyProfileId
);

// 3. Create the spawn system with the scheduler
auto spawnSystem = std::make_unique<GameObjectSpawnSystem>(
    std::move(scheduler)
);

// 4. Register with the game loop
gameLoop.phase(PhaseType::Main)
    .addPass()
    .addSystem(std::move(spawnSystem));
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::gameplay::spawn
@brief Entity spawning infrastructure and lifecycle management for the helios engine.
@details This namespace contains the spawning subsystem. It provides a template-based,
scheduler-driven spawn pipeline with separated policy (rules, conditions, amount providers),
scheduling, behavior (placement, initialization), command, and execution responsibilities.
Integrates with pool managers to control entity creation and destruction at runtime.
</p></details>
