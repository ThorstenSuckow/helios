# helios::engine::runtime::spawn::policy

Spawn rules, conditions, and amount providers.

## Overview

This module provides the policy layer for the spawn system. It defines rules that combine conditions (when to spawn) with amount providers (how many to spawn).

## Key Classes

| Class | Purpose |
|-------|---------|
| `SpawnRule` | Combines condition and amount provider into evaluatable rule |
| `SpawnCondition` | Abstract interface for spawn timing conditions |
| `SpawnRuleState` | Runtime state tracked per rule (e.g., time since last spawn) |

## Submodules

| Directory | Purpose |
|-----------|---------|
| `amount/` | SpawnAmountProvider implementations |
| `conditions/` | SpawnCondition implementations |

## Usage

```cpp
// Create a rule: spawn 3 enemies every 2 seconds
auto rule = std::make_unique<SpawnRule>(
    std::make_unique<TimerSpawnCondition>(2.0f),
    std::make_unique<FixedSpawnAmount>(3),
    SpawnRuleId{1}
);

// Add to scheduler
scheduler.addRule(enemyProfileId, std::move(rule));
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::spawn::policy
@brief Spawn rules, conditions, and amount providers.
@details Provides the policy layer for spawn timing and quantity control, separating "when" from "how many" logic.
</p></details>
