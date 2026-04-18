# helios::runtime::spawn::policy::amount

Spawn amount providers for determining spawn quantities.

## Overview

This module provides `SpawnAmountProvider<THandle>` implementations that
determine how many entities should spawn when a rule's condition is satisfied.

## Key Classes

| Class | Purpose |
|-------|---------|
| `SpawnAmountProvider<THandle>` | Abstract interface for calculating spawn quantity |
| `FixedSpawnAmount<THandle>` | Always returns a fixed count |
| `SpawnAmountByCallback<THandle>` | Delegates to a user-provided callback function |
| `SpawnAll<THandle>` | Returns the number of inactive pool entities |

## Usage

```cpp
using Handle = GameObjectHandle;

// Fixed amount: always spawn 3 entities
auto amount = std::make_unique<FixedSpawnAmount<Handle>>(3);

// Callback-based: dynamic amount based on game state
auto dynamicAmount = std::make_unique<SpawnAmountByCallback<Handle>>(
    [](const GameObjectPoolId& poolId, const SpawnRuleState& state,
       const GameWorld& gameWorld, const UpdateContext& ctx) -> size_t {
        return 2;  // dynamic logic here
    }
);

// Use with SpawnRule
auto rule = std::make_unique<SpawnRule<Handle>>(
    std::make_unique<TimerSpawnCondition>(2.0f),
    std::move(amount),
    SpawnRuleId{1}
);
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::runtime::spawn::policy::amount
@brief Spawn amount providers for determining spawn quantities.
@details Provides implementations that calculate how many entities should spawn based on fixed values or dynamic game state.
</p></details>
