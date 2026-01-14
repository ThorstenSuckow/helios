# helios::engine::mechanics::spawn::manager

Managers for spawn and despawn request processing.

This namespace contains the SpawnManager which handles the actual spawning and despawning of GameObjects from pools.

## Managers

| Manager | Purpose |
|---------|---------|
| `SpawnManager` | Processes spawn/despawn requests, manages pool lifecycle |

## Responsibilities

The SpawnManager:

1. Receives SpawnRequest and DespawnRequest instances
2. Queues requests for deferred processing
3. During flush: processes despawns first, then spawns
4. Uses GameObjectFactory to populate pools
5. Registers as PoolRequestHandler for its assigned pool

## Usage

```cpp
auto factory = std::make_unique<GameObjectFactory>(...);
auto manager = std::make_unique<SpawnManager>(poolId, std::move(factory));

// Initialize and register with GameWorld
manager->init(gameWorld);

// Register as manager in game loop
gameLoop.addManager(std::move(manager));
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::spawn::manager
@brief Managers for spawn and despawn request processing.
@details This namespace contains the SpawnManager which handles the actual spawning and despawning of GameObjects, integrating with the pool system and factory pattern.
</p></details>

