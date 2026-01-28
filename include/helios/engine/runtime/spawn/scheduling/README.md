# helios::engine::runtime::spawn::scheduling

Spawn scheduling and plan management.

## Overview

This module provides the scheduling layer for the spawn system. Schedulers evaluate spawn rules and produce ScheduledSpawnPlan instances that are processed by the command pipeline. The architecture separates scheduling strategies from rule processing logic.

## Key Classes

| Class | Purpose |
|-------|---------|
| `SpawnScheduler` | Abstract base class defining the scheduler interface |
| `DefaultSpawnScheduler` | Evaluates all rules every frame |
| `CyclicSpawnScheduler<N>` | Round-robin evaluation, advances on successful spawn |
| `RuleProcessor` | Abstract interface for rule evaluation logic |
| `DefaultRuleProcessor` | Standard rule processing implementation |
| `SpawnPlan` | Data describing a planned spawn (rule, amount) |
| `ScheduledSpawnPlan` | A plan paired with profile ID and context |
| `RuleConfig` | Configuration pairing profile ID with a rule |

## Architecture

```
┌─────────────────────────────────────────────────────────────────────────┐
│                        SpawnScheduler (abstract)                        │
│  ┌───────────────────────────────────────────────────────────────────┐  │
│  │ evaluate()           → Process rules, produce plans               │  │
│  │ drainScheduledPlans() → Return and clear pending plans            │  │
│  │ commit()             → Update rule state after spawn              │  │
│  └───────────────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────────────┘
                    │                                │
        ┌───────────┴────────────┐       ┌───────────┴────────────┐
        │  DefaultSpawnScheduler │       │  CyclicSpawnScheduler  │
        │  (all rules/frame)     │       │  (advance on success)  │
        └───────────┬────────────┘       └───────────┬────────────┘
                    │                                │
                    └───────────┬────────────────────┘
                                │
                    ┌───────────┴────────────┐
                    │   DefaultRuleProcessor │
                    │   (shared logic)       │
                    └────────────────────────┘
```

## Scheduler Strategies

### DefaultSpawnScheduler

Evaluates **all registered rules** every frame. Best for independent spawn sources that should trigger simultaneously.

```cpp
DefaultSpawnScheduler scheduler;
scheduler
    .addRule(enemyProfileId, std::make_unique<TimerSpawnRule>(ruleA, 2.0f, 3))
    .addRule(pickupProfileId, std::make_unique<TimerSpawnRule>(ruleB, 5.0f, 1));

// Both rules evaluated each frame
scheduler.evaluate(updateContext, spawnContext);
```

### CyclicSpawnScheduler

Evaluates the **current rule** each frame and advances to the next rule **only after a spawn was successfull committed**. 
The same rule is re-evaluated until its conditions are met. Best for sequential wave patterns where each wave must complete before the next begins.

```cpp
CyclicSpawnScheduler<3> scheduler;
scheduler
    .addRule(wave1ProfileId, std::make_unique<TimerSpawnRule>(rule1, 1.0f, 5))
    .addRule(wave2ProfileId, std::make_unique<TimerSpawnRule>(rule2, 1.0f, 5))
    .addRule(bossProfileId, std::make_unique<TimerSpawnRule>(rule3, 2.0f, 1));

// Evaluates wave1 until spawn occurs, then advances to wave2, etc.
// Cycle: Wave1 → Wave2 → Boss → Wave1 → ...
scheduler.evaluate(updateContext, spawnContext);
```

## Workflow

```
SpawnRule::evaluate() → SpawnPlan → ScheduledSpawnPlan → Command → SpawnManager
```

1. **Registration:** Rules added via `addRule()` with profile IDs
2. **Evaluation:** `evaluate()` called each frame by GameObjectSpawnSystem
3. **Processing:** RuleProcessor evaluates rules, updates state
4. **Scheduling:** Rules that pass produce ScheduledSpawnPlans
5. **Draining:** `drainScheduledPlans()` returns pending plans
6. **Commit:** `commit()` called after spawns to update rule state

## Rule Processing

The `RuleProcessor` abstraction separates evaluation logic from scheduling strategy:

```cpp
class RuleProcessor {
public:
    virtual SpawnPlan processRule(
        const UpdateContext& updateContext,
        const SpawnContext& spawnContext,
        SpawnProfileId spawnProfileId,
        SpawnRule& spawnRule,
        SpawnRuleState& spawnRuleState
    ) noexcept = 0;
};
```

`DefaultRuleProcessor` provides the standard implementation:
1. Retrieve pool snapshot from GameObjectPoolManager
2. Update rule state with delta time
3. Evaluate rule conditions
4. Return SpawnPlan with spawn amount

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.engine.runtime.spawn.policy` | SpawnRule implementations |
| `helios.engine.runtime.spawn` | SpawnManager, SpawnProfile |
| `helios.engine.runtime.pooling` | GameObjectPoolManager |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::spawn::scheduling
@brief Spawn scheduling and plan management.
@details Provides schedulers for evaluating spawn rules and producing spawn plans. Supports multiple scheduling strategies through SpawnScheduler implementations and separates rule processing logic via the RuleProcessor abstraction.
</p></details>
