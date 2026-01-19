# helios::engine::runtime::spawn::policy::conditions

Spawn conditions for controlling spawn timing.

## Overview

This module provides SpawnCondition implementations that determine when spawning should occur based on time, triggers, or other game state.

## Key Classes

| Class | Purpose |
|-------|---------|
| `TimerSpawnCondition` | Triggers spawning at fixed time intervals |

## Usage

```cpp
// Spawn every 2 seconds
auto condition = std::make_unique<TimerSpawnCondition>(2.0f);

// Use with SpawnRule
auto rule = std::make_unique<SpawnRule>(
    std::move(condition),
    std::make_unique<FixedSpawnAmount>(3),
    SpawnRuleId{1}
);

// Add to scheduler
scheduler.addRule(enemyProfileId, std::move(rule));
```

## Implementing Custom Conditions

```cpp
class WaveSpawnCondition : public SpawnCondition {
public:
    bool isSatisfied(size_t amount, const SpawnRuleState& state,
                     const GameObjectPoolSnapshot& pool,
                     const UpdateContext& ctx) const override {
        // Spawn when wave timer expires and pool has capacity
        return state.sinceLastSpawn() >= waveInterval_ 
            && pool.inactiveCount >= amount;
    }
    
    void onCommit(SpawnRuleState& state, size_t count) const override {
        state.setSinceLastSpawn(0.0f);  // Reset timer
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
