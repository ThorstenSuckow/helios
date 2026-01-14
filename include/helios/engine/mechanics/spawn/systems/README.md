# helios::engine::mechanics::spawn::systems

Systems for spawn lifecycle management within the game loop.

This namespace contains the GameObjectSpawnSystem which integrates spawning into the game loop by evaluating conditions and emitting spawn commands.

## Systems

| System | Purpose |
|--------|---------|
| `GameObjectSpawnSystem` | Evaluates SpawnConditions and emits SpawnCommands |

## Usage

```cpp
auto condition = std::make_unique<TimerSpawnCondition>(2.0f);
auto system = std::make_unique<GameObjectSpawnSystem>(poolId, std::move(condition));

gameLoop.phase(PhaseType::Main)
    .addPass()
    .addSystem(std::move(system));
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::spawn::systems
@brief Systems for spawn lifecycle management.
@details This namespace contains the GameObjectSpawnSystem which integrates spawning into the game loop by evaluating conditions and emitting spawn commands.
</p></details>
