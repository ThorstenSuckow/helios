# helios::engine::mechanics::spawn::systems

Systems for spawn lifecycle management within the game loop.

## Overview

This module contains the GameObjectSpawnSystem which integrates spawning into the game loop. The system evaluates spawn conditions via a SpawnScheduler and enqueues spawn commands for deferred execution.

## Key Classes

| System | Purpose |
|--------|---------|
| `GameObjectSpawnSystem` | Evaluates SpawnScheduler rules and enqueues ScheduledSpawnPlanCommands |

## System Flow

The GameObjectSpawnSystem performs the following each frame:

1. **Read Frame Events:** Consumes `SpawnPlanCommandExecutedEvent` from the previous frame
2. **Commit Completed Spawns:** Updates the scheduler with successful spawn counts
3. **Evaluate Conditions:** Checks all spawn rules against current conditions
4. **Drain Scheduled Plans:** Enqueues ready plans as `ScheduledSpawnPlanCommand`

## Usage

```cpp
// Create scheduler with spawn rules
auto scheduler = std::make_unique<SpawnScheduler>();
scheduler->addRule(enemyRuleId, std::make_unique<TimerSpawnCondition>(2.0f), enemyProfileId);

// Create system with scheduler
auto system = std::make_unique<GameObjectSpawnSystem>(std::move(scheduler));

// Register with game loop
gameLoop.phase(PhaseType::Main)
    .addPass()
    .addSystem(std::move(system));
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::spawn::systems
@brief Systems for spawn lifecycle management.
@details Contains the GameObjectSpawnSystem which integrates spawning into the game loop by evaluating a SpawnScheduler and enqueuing spawn commands for deferred execution.
</p></details>
