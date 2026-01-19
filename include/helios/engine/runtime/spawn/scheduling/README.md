# helios::engine::runtime::spawn::scheduling

Spawn scheduling and plan management.

## Overview

This module provides the scheduling layer for the spawn system. The SpawnScheduler evaluates spawn rules and produces ScheduledSpawnPlan instances that are then processed by the command pipeline.

## Key Classes

| Class | Purpose |
|-------|---------|
| `SpawnScheduler` | Evaluates rules and produces scheduled plans |
| `SpawnPlan` | Data describing a planned spawn (rule, amount, pool) |
| `ScheduledSpawnPlan` | A plan paired with its spawn profile ID |

## Workflow

```
SpawnRule::evaluate() → SpawnPlan → ScheduledSpawnPlan → Command → SpawnManager
```

## Usage

```cpp
SpawnScheduler scheduler;

// Add rules with their profiles
scheduler.addRule(enemyProfileId, std::make_unique<TimerSpawnRule>(
    ruleId, 2.0f, 3  // Every 2s, spawn up to 3
));

// Each frame in GameObjectSpawnSystem
scheduler.evaluate(updateContext);
auto plans = scheduler.drainScheduledPlans();

// Plans are converted to commands for execution
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::spawn::scheduling
@brief Spawn scheduling and plan management.
@details Provides the SpawnScheduler for evaluating rules and producing spawn plans that feed into the command pipeline.
</p></details>
