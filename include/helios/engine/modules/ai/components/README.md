# helios::engine::modules::ai::components

AI behavior data components.

## Overview

Components storing AI behavior state. These are pure data containers processed by the corresponding systems in `helios.engine.modules.ai.systems`.

## Components

| Component | Purpose |
|-----------|---------|
| `ChaseComponent` | Tracks a target entity and controls pursuit timing |

## ChaseComponent

Stores data for chase/pursuit behavior:

| Property | Type | Purpose |
|----------|------|---------|
| `target_` | `EntityHandle` | Handle of the entity to chase |
| `cooldown_` | `float` | Interval between direction updates (seconds) |
| `cooldownTimer_` | `float` | Elapsed time since last update |

### API

```cpp
// Set target entity
void setTarget(EntityHandle handle);
EntityHandle target() const;

// Configure update frequency
void setCooldown(float seconds);
float cooldown() const;

// Timer management (used by ChaseSystem)
void updateCooldownTimerBy(float delta);
void setCooldownTimer(float timer);
float cooldownTimer() const;
```

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.engine.modules.ai.systems` | ChaseSystem processes this component |
| `helios.util.Guid` | Target identification |

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::ai::components
@brief AI behavior data components.
@details Components storing AI behavior state such as chase targets and timing data. Processed by corresponding systems in the ai::systems namespace.
</p></details>
