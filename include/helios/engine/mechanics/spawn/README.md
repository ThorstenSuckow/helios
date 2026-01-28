# helios::engine::mechanics::spawn

Entity spawning and lifecycle management.

This module provides the infrastructure for spawning and despawning GameObjects at runtime based on configurable conditions and scheduling rules.

## Architecture

The spawn system follows a scheduler-based architecture with separation of concerns:

```
┌─────────────────────┐    ┌─────────────────────┐
│  SpawnCondition     │    │   SpawnStrategy     │
│  (when to spawn)    │    │   (how to spawn)    │
└─────────┬───────────┘    └──────────┬──────────┘
          │                           │
          ▼                           ▼
┌─────────────────────────────────────────────────┐
│              SpawnScheduler                     │
│   (evaluates rules, produces ScheduledPlans)    │
└─────────────────────┬───────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────┐
│         GameObjectSpawnSystem                   │
│   (reads frame events, enqueues SpawnCommands)  │
└─────────────────────┬───────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────┐
│          ScheduledSpawnPlanCommand              │
│   (dispatched to SpawnManager for execution)    │
└─────────────────────────────────────────────────┘
```

## Submodules

| Directory | Purpose |
|-----------|---------|
| `components/` | Spawn-related components (SpawnedByProfileComponent) |
| `systems/` | GameObjectSpawnSystem for game loop integration |

## Related Modules

The spawn system integrates with several other modules:

- `helios.engine.mechanics.spawn.logic` — SpawnScheduler, SpawnCondition, SpawnStrategy
- `helios.engine.mechanics.spawn.commands` — ScheduledSpawnPlanCommand
- `helios.engine.runtime.spawn` — SpawnManager, SpawnCommandHandler
- `helios.engine.runtime.pooling` — GameObjectPoolManager

## Usage

```cpp
// 1. Create spawn rules with conditions
auto scheduler = std::make_unique<SpawnScheduler>();
scheduler->addRule(
    SpawnRuleId{1},
    std::make_unique<TimerSpawnCondition>(2.0f),  // Every 2 seconds
    enemyProfileId
);

// 2. Create the spawn system with the scheduler
auto spawnSystem = std::make_unique<GameObjectSpawnSystem>(
    std::move(scheduler)
);

// 3. Register with the game loop
gameLoop.phase(PhaseType::Main)
    .addPass()
    .addSystem(std::move(spawnSystem));
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::spawn
@brief Entity spawning and lifecycle management.
@details This namespace contains the spawning subsystem of the gameplay layer. It provides a scheduler-based architecture using conditions (when to spawn), strategies (how to spawn), and integration with pool managers to control entity creation and destruction at runtime.
</p></details>

