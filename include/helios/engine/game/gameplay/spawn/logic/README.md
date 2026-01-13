# helios::engine::game::gameplay::spawn::logic

Spawn conditions and strategies for controlling entity creation.

This namespace provides the policy interfaces and implementations for the spawn system. Conditions determine *when* to spawn, while strategies handle *how* to configure spawned entities.

## Classes

| Class | Purpose |
|-------|---------|
| `SpawnCondition` | Abstract base for determining spawn timing and quantity |
| `TimerSpawnCondition` | Spawns at fixed time intervals |
| `MaxPerSecCondition` | Rate-limited spawning based on requests per second |
| `SpawnStrategy` | Abstract base for configuring spawned GameObjects |
| `CallbackSpawnStrategy` | Delegates spawning logic to a callback function |

## Usage

```cpp
// Timer-based spawning (one object every 2 seconds)
auto condition = std::make_unique<TimerSpawnCondition>(2.0f);

// Rate-limited spawning (max 10 per second)
auto condition = std::make_unique<MaxPerSecCondition>(10);

// Callback-based strategy for flexible initialization
auto strategy = std::make_unique<CallbackSpawnStrategy>(
    [](const GameWorld& w, GameObject& o, const UpdateContext& c) {
        // Initialize object...
        return true;
    }
);
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::game::gameplay::spawn::logic
@brief Spawn conditions and strategies for controlling entity creation.
@details This namespace provides the policy interfaces and implementations for the spawn system. Conditions determine timing (interval-based or rate-limited), while strategies handle entity initialization and configuration.
</p></details>

