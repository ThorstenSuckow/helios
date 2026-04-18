# helios::gameplay::spawn::scheduling

Spawn scheduling and plan management.

## Overview

This module provides the scheduling layer for the spawn system. Schedulers
(`SpawnScheduler<THandle>`) evaluate spawn rules and produce
`ScheduledSpawnPlan<THandle>` instances that are forwarded through command
buffers to `SpawnManager<THandle>`.

## Architecture

```
┌─────────────────────────────────────────────────────────────────────────┐
│                    SpawnScheduler<THandle> (abstract)                   │
│  ┌───────────────────────────────────────────────────────────────────┐  │
│  │ evaluate()            → process rules, produce plans              │  │
│  │ drainScheduledPlans() → return and clear pending plans            │  │
│  │ commit()              → update rule state after successful spawn  │  │
│  └───────────────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────────────┘
                    │                                │
        ┌───────────┴────────────┐       ┌───────────┴─────────────┐
        │ DefaultSpawnScheduler  │       │  CyclicSpawnScheduler   │
        │       <THandle>        │       │     <THandle, N>        │
        └───────────┬────────────┘       └───────────┬─────────────┘
                    │                                │
                    └───────────┬────────────────────┘
                                │
                    ┌───────────┴────────────┐
                    │ DefaultRuleProcessor   │
                    │      <THandle>         │
                    └────────────────────────┘
```

## Key Types

| Type | Purpose |
|------|---------|
| `SpawnScheduler<THandle>` | Scheduler interface |
| `DefaultSpawnScheduler<THandle>` | Evaluate all rules each update |
| `CyclicSpawnScheduler<THandle, N>` | Round-robin rule evaluation with fixed-size ring buffer |
| `RuleProcessor<THandle>` | Strategy interface for rule processing |
| `DefaultRuleProcessor<THandle>` | Standard processing implementation |
| `RuleConfig<THandle>` | Profile/rule pairing used by `CyclicSpawnScheduler` |
| `SpawnPlan` | Rule result (`ruleId`, `amount`) |
| `ScheduledSpawnPlan<THandle>` | `SpawnPlan` + profile id + typed context |

## Scheduler Strategies

### DefaultSpawnScheduler<THandle>

Evaluates all registered rules each update tick.

```cpp
DefaultSpawnScheduler<EnemyHandle> scheduler;
scheduler
    .addRule(enemyProfileId, std::make_unique<TimerSpawnRule>(ruleA, 2.0f, 3))
    .addRule(pickupProfileId, std::make_unique<TimerSpawnRule>(ruleB, 5.0f, 1));

// Both rules evaluated each frame
scheduler.evaluate(updateContext, spawnContext);
```

### CyclicSpawnScheduler<THandle, N>

Evaluates one active rule per frame in a fixed-size (`N`) ring buffer and
advances cyclically after a successful spawn. Ideal for wave-based or
sequential spawn patterns.

```cpp
CyclicSpawnScheduler<EnemyHandle, 3> scheduler;
scheduler
    .addRule(wave1ProfileId, std::make_unique<TimerSpawnRule>(rule1, 1.0f, 5))
    .addRule(wave2ProfileId, std::make_unique<TimerSpawnRule>(rule2, 1.0f, 5))
    .addRule(bossProfileId, std::make_unique<TimerSpawnRule>(rule3, 2.0f, 1));

// Evaluates wave1 until spawn occurs, then advances to wave2, etc.
// Cycle: Wave1 → Wave2 → Boss → Wave1 → ...
scheduler.evaluate(updateContext, spawnContext);
```

## Workflow

1. Register `SpawnRule<THandle>` entries.
2. Call `evaluate(gameWorld, updateContext, spawnContext)`.
3. Drain `ScheduledSpawnPlan<THandle>` entries.
4. Queue `ScheduledSpawnPlanCommand<THandle>`.
5. `SpawnManager<THandle>` executes plans during flush.

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::gameplay::spawn::scheduling
@brief Spawn scheduling and plan management.
@details Template-based schedulers and rule processors for typed spawn flows.
</p></details>
