# helios::engine::runtime::spawn::policy::conditions

Spawn conditions for controlling spawn timing.

## Overview

This module provides `SpawnCondition` implementations that determine when
spawning should occur based on time, pool state, or other game conditions.

> `SpawnCondition` is not template-based. Conditions operate on
> `SpawnRuleState`, `GameObjectPoolSnapshot`, and `UpdateContext`.

## Key Classes

| Class | Purpose |
|-------|---------|
| `TimerSpawnCondition` | Triggers spawning at fixed time intervals |
| `RequestedAmountIsAvailableCondition` | Passes only when the pool has enough inactive entities |
| `SpawnConditionAll` | Composite AND over multiple conditions (short-circuit) |

## Usage

```cpp
// Spawn every 2 seconds
auto condition = std::make_unique<TimerSpawnCondition>(2.0f);

// Combine conditions: timer AND pool availability
auto composite = std::make_unique<SpawnConditionAll>(
    std::make_unique<TimerSpawnCondition>(2.0f),
    std::make_unique<RequestedAmountIsAvailableCondition>()
);

// Use with SpawnRule
using Handle = GameObjectHandle;
auto rule = std::make_unique<SpawnRule<Handle>>(
    std::move(composite),
    std::make_unique<FixedSpawnAmount<Handle>>(3),
    SpawnRuleId{1}
);
```

## Implementing Custom Conditions

```cpp
class WaveSpawnCondition : public SpawnCondition {
public:
    bool isSatisfied(size_t amount, const SpawnRuleState& state,
                     const GameObjectPoolSnapshot& pool,
                     const UpdateContext& ctx) const noexcept override {
        return state.sinceLastSpawn() >= waveInterval_
            && pool.inactiveCount >= amount;
    }

    void onCommit(SpawnRuleState& state, size_t count) const override {
        state.setSinceLastSpawn(0.0f);
    }

    void onReset(SpawnRuleState& state) const noexcept override {
        state.setSinceLastSpawn(0.0f);
    }
};
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::spawn::policy::conditions
@brief Spawn conditions for controlling spawn timing.
@details Provides SpawnCondition implementations that determine when spawning should occur based on timers, triggers, or game state.
</p></details>
