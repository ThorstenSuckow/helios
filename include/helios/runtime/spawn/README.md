# helios::runtime::spawn

Entity spawning infrastructure for the helios engine.

## Overview

This module provides a complete spawning pipeline for runtime entity lifecycle
management. The architecture separates concerns into:

- **Policy layer** (*when/how many*)
- **Behavior layer** (*where/how*)
- **Execution layer** (*pool acquisition/release and command handling*)

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
| `events/` | Spawn pipeline events (e.g. plan execution confirmation) |
| `policy/` | Rules, conditions, and amount providers |
| `scheduling/` | Scheduler implementations and plan production |
| `types/` | Shared ids, contexts, profiles, cursors |

## Key Types

| Type | Purpose |
|------|---------|
| `SpawnManager<THandle>` | Executes spawn/despawn command queues and pool operations |
| `SpawnProfile<THandle>` | Binds pool, placer, and initializer for a profile id |
| `SpawnScheduler<THandle>` | Produces scheduled spawn plans from rules |
| `SpawnContext<THandle>` | Context payload for placement/initialization |
| `EmitterContext<THandle>` | Optional emitter/source context |

## Usage

```cpp
using Handle = GameObjectHandle;

auto profile = std::make_unique<SpawnProfile<Handle>>(SpawnProfile<Handle>{
    .gameObjectPoolId = bulletPoolId,
    .spawnPlacer = std::make_unique<EmitterSpawnPlacer<Handle>>(),
    .spawnInitializer = std::make_unique<EmitterInitializer<Handle>>()
});

auto& spawnManager = gameWorld.registerManager<SpawnManager<Handle>>();
spawnManager.addSpawnProfile(bulletProfileId, std::move(profile));
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::runtime::spawn
@brief Entity spawning infrastructure for the helios engine.
@details Template-based spawn pipeline with separated policy, scheduling,
behavior, and execution responsibilities.
</p></details>
