# helios::engine::game::gameplay::spawn

Entity spawning and lifecycle management.

This module provides the infrastructure for spawning and despawning GameObjects at runtime based on configurable conditions and strategies.

## Architecture

The spawn system follows a command-based architecture with separation of concerns:

```
┌─────────────────────┐    ┌─────────────────────┐
│  SpawnCondition     │    │   SpawnStrategy     │
│  (when to spawn)    │    │   (how to spawn)    │
└─────────┬───────────┘    └──────────┬──────────┘
          │                           │
          ▼                           ▼
┌─────────────────────────────────────────────────┐
│           GameObjectSpawnSystem                 │
│   (calculates budget, emits SpawnCommands)      │
└─────────────────────┬───────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────┐
│           SpawnCommandDispatcher                │
│   (routes commands to SpawnManager)             │
└─────────────────────┬───────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────┐
│              SpawnManager                       │
│   (acquires from pool, applies strategy)        │
└─────────────────────────────────────────────────┘
```

## Submodules

| Directory | Purpose |
|-----------|---------|
| `commands/` | SpawnCommand and DespawnCommand for deferred execution |
| `dispatcher/` | Command dispatchers that route to managers |
| `logic/` | Spawn conditions and strategies |
| `manager/` | SpawnManager for pool-based spawning |
| `requests/` | Data structures for spawn/despawn requests |
| `systems/` | GameObjectSpawnSystem for game loop integration |

## Usage

```cpp
// 1. Create a spawn condition (when to spawn)
auto condition = std::make_unique<TimerSpawnCondition>(2.0f);  // Every 2 seconds

// 2. Create a spawn strategy (how to configure spawned objects)
auto strategy = std::make_unique<CallbackSpawnStrategy>(
    [](const GameWorld& world, GameObject& obj, const UpdateContext& ctx) {
        auto* transform = obj.get<TranslationStateComponent>();
        transform->setTranslation({0.0f, 0.0f, 0.0f});
        return true;
    }
);

// 3. Create the spawn system
auto spawnSystem = std::make_unique<GameObjectSpawnSystem>(
    poolId, std::move(condition)
);

// 4. Register with the game loop
gameLoop.phase(PhaseType::Main)
    .addPass()
    .addSystem(std::move(spawnSystem));
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::game::gameplay::spawn
@brief Entity spawning and lifecycle management.
@details This namespace contains the spawning subsystem of the gameplay layer. It provides a flexible, command-based architecture using conditions (when to spawn), strategies (how to spawn), and managers (pool integration) to control entity creation and destruction at runtime.
</p></details>

