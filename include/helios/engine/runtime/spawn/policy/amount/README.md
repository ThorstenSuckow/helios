# helios::engine::runtime::spawn::policy::amount

Spawn amount providers for determining spawn quantities.

## Overview

This module provides SpawnAmountProvider implementations that determine how many entities should spawn when a rule's condition is satisfied.

## Key Classes

| Class | Purpose |
|-------|---------|
| `SpawnAmountProvider` | Abstract interface for calculating spawn quantity |
| `FixedSpawnAmount` | Always returns a fixed count |
| `SpawnAmountByCallback` | Delegates to a user-provided callback function |

## Usage

```cpp
// Fixed amount: always spawn 3 entities
auto amount = std::make_unique<FixedSpawnAmount>(3);

// Callback-based: dynamic amount based on game state
auto dynamicAmount = std::make_unique<SpawnAmountByCallback>(
    [&DIFFICULTY_LEVEL](const GameObjectPoolId& poolId, const SpawnRuleState& state,
       const UpdateContext& ctx) -> size_t {
        // Spawn more enemies as difficulty increases
        return DIFFICULTY_LEVEL + 1;
    }
);

// Use with SpawnRule
auto rule = std::make_unique<SpawnRule>(
    std::make_unique<TimerSpawnCondition>(2.0f),
    std::move(amount),
    SpawnRuleId{1}
);
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::spawn::policy::amount
@brief Spawn amount providers for determining spawn quantities.
@details Provides implementations that calculate how many entities should spawn based on fixed values or dynamic game state.
</p></details>
