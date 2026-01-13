# helios::engine::game::gameplay::spawn::dispatcher

Command dispatchers for spawn operations.

This namespace contains dispatchers that route spawn and despawn commands to the appropriate managers for processing.

## Dispatchers

| Dispatcher | Purpose |
|------------|---------|
| `SpawnCommandDispatcher` | Routes SpawnCommands to the SpawnManager |
| `DespawnCommandDispatcher` | Routes DespawnCommands to the SpawnManager |

## Architecture

Dispatchers implement the TypedWorldCommandDispatcher interface and are registered with the CommandBuffer. When commands are flushed, they are routed through the dispatcher to the appropriate manager.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::game::gameplay::spawn::dispatcher
@brief Command dispatchers for spawn operations.
@details This namespace contains typed command dispatchers that route spawn and despawn commands to the SpawnManager for deferred processing.
</p></details>

