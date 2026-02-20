# Object Pooling

helios provides an **object pooling** system for efficient lifecycle management of frequently spawned entities like projectiles, particles, or enemies. The system eliminates runtime allocation overhead by pre-allocating and recycling GameObjects.

## Overview

The pooling system consists of several layers:

- **GameObjectPool:** Low-level storage tracking active/inactive EntityHandles
- **GameObjectPoolManager:** High-level manager for pool configuration and operations
- **GameObjectPoolRegistry:** Central registry mapping pool IDs to pool instances
- **GameObjectPoolConfig:** Declarative configuration for pool setup

```
┌─────────────────────────────────────────────────────────────────────┐
│                      POOLING ARCHITECTURE                           │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  CONFIGURATION LAYER                                                │
│  ┌─────────────────────────────────────────────────────────────┐    │
│  │  GameObjectPoolConfig                                       │    │
│  │  - poolId: GameObjectPoolId                                 │    │
│  │  - prefab: GameObject*                                      │    │
│  │  - amount: size_t                                           │    │
│  └─────────────────────────────────────────────────────────────┘    │
│                           │                                         │
│                           ▼                                         │
│  MANAGER LAYER                                                      │
│  ┌─────────────────────────────────────────────────────────────┐    │
│  │  GameObjectPoolManager                                      │    │
│  │  - addPoolConfig()  → register configuration                │    │
│  │  - init()           → create pools, clone prefabs, lock     │    │
│  │  - acquire()        → get inactive entity                   │    │
│  │  - release()        → return entity to pool                 │    │
│  └─────────────────────────────────────────────────────────────┘    │
│                           │                                         │
│                           ▼                                         │
│  STORAGE LAYER                                                      │
│  ┌─────────────────────────────────────────────────────────────┐    │
│  │  GameObjectPoolRegistry                                     │    │
│  │  ┌─────────────────┐  ┌─────────────────┐                   │    │
│  │  │ GameObjectPool  │  │ GameObjectPool  │  ...              │    │
│  │  │ (bullets)       │  │ (enemies)       │                   │    │
│  │  └─────────────────┘  └─────────────────┘                   │    │
│  └─────────────────────────────────────────────────────────────┘    │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

## Key Concepts

### Pool Lifecycle

The pooling system follows a strict lifecycle:

1. **Configuration:** Register pool configurations via `addPoolConfig()`
2. **Initialization:** Call `init()` to create pools, clone prefabs, and **lock** pools
3. **Runtime:** Use `acquire()` and `release()` to manage entity lifecycle

```cpp
// 1. Create manager and configure pools
GameObjectPoolManager poolManager;

// Create prefab via GameWorld
auto bulletPrefab = gameWorld.addGameObject();
bulletPrefab.add<RenderableComponent>(mesh, material);
bulletPrefab.add<Move2DComponent>(bulletSpeed);
bulletPrefab.add<CollisionComponent>(BULLET_LAYER);
bulletPrefab.setActive(false);  // Prefabs should be inactive

auto bulletConfig = std::make_unique<GameObjectPoolConfig>(
    GameObjectPoolId{"bullets"},
    bulletPrefab,
    100  // pool size
);
poolManager.addPoolConfig(std::move(bulletConfig));

// 2. Initialize (clones prefabs, locks pools)
poolManager.init(gameWorld);

// 3. Runtime operations - acquire returns std::optional<GameObject>
if (auto bullet = poolManager.acquire(GameObjectPoolId{"bullets"})) {
    bullet->get<TranslationStateComponent>()->setTranslation(spawnPos);
    bullet->setActive(true);
}

// Later: return to pool
poolManager.release(GameObjectPoolId{"bullets"}, bullet->entityHandle());
```

### Pool Locking

After `init()` completes, each pool is **locked**. A locked pool:

- Cannot accept new EntityHandles via `addInactive()`
- Has its sparse arrays sized based on min/max EntityIds
- Is ready for O(1) acquire/release operations

This design optimizes memory layout but means **pool size is fixed at initialization time**.

### Internal Data Structure

`GameObjectPool` uses a sparse-set pattern for O(1) operations:

```
┌─────────────────────────────────────────────────────────────────────┐
│  GameObjectPool (sparse-set)                                        │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  activeIndex_:   [0]  [1]  [2]  [3]  ...   (EntityId → dense idx)   │
│                   ↓    ↓    ↓    ↓                                  │
│  versionIndex_:  [1]  [1]  [2]  [1]  ...   (version per EntityId)   │
│                                                                     │
│  activeGameObjects_:   [handle0, handle2, handle5, ...]  (dense)    │
│  inactiveGameObjects_: [handle1, handle3, handle4, ...]  (stack)    │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘

Performance:
┌────────────┬────────────┬─────────────────────────────────────────┐
│ Operation  │ Complexity │ Notes                                   │
├────────────┼────────────┼─────────────────────────────────────────┤
│ acquire()  │ O(1)       │ Pop from inactive, add to active        │
│ release()  │ O(1)       │ Swap-and-pop from active, push inactive │
│ lock()     │ O(n)       │ One-time sparse array allocation        │
└────────────┴────────────┴─────────────────────────────────────────┘
```

### EntityHandle Validation

Pools track entity versions to detect stale handles:

```cpp
struct EntityHandle {
    EntityId entityId;    // Index into sparse array
    VersionId versionId;  // Detects stale references
};
```

When an entity is released and re-acquired, its version may change. Attempting to release with an outdated handle is safely ignored.

## Trade-offs

### Fixed Capacity

Pool size cannot grow after initialization. If more entities are needed than the pool capacity, `acquire()` returns `nullptr`.

**Mitigation:** Size pools based on expected maximum concurrent usage plus a safety margin.

```cpp
// Estimate: max 50 bullets on screen at once
// Safety margin: 2x
auto config = std::make_unique<GameObjectPoolConfig>(
    GameObjectPoolId{"bullets"}, bulletPrefab.get(), 100
);
```

### Memory Overhead

Sparse arrays are sized for the EntityId range (max - min), which may waste memory if EntityIds are not contiguous.

**Example:** If a pool contains EntityIds 1000-1099, the sparse arrays will have 100 slots regardless of how many entities are actually in the pool.

### Initialization Cost

All prefab clones are created upfront during `init()`, which may cause a startup delay for large pools.

**Mitigation:** 
- Use loading screens during initialization
- Consider lazy initialization for non-critical pools (not currently supported)

## Integration with Spawn System

The pooling system integrates with the [Spawn System](./spawn-system.md) through `SpawnManager`. The recommended way to configure both pool and spawn system together is via the `SpawnSystemFactory` builder, which co-locates pool and profile configuration under a single pool ID:

```cpp
using namespace helios::engine::builder::spawnSystem;

auto spawns = SpawnSystemFactory::configure(poolManager, spawnManager);

spawns.pool(GameObjectPoolId{"enemies"}, enemyPrefab, 50)
    .profile(SpawnProfileId{"enemy_random"})
        .randomPlacement()
        .randomDirectionInitializer()
        .scheduledBy(SpawnRuleId{"enemy_timer"})
            .timerCondition(3.0f)
            .fixedAmount(2)
            .done()
        .done()
    .commit();
```

This registers the pool with `GameObjectPoolManager`, the profile with `SpawnManager`, and creates a `DefaultSpawnScheduler` — all in one fluent call.

### Manual Integration

For cases where the builder is not suitable:

```cpp
// SpawnProfile references a pool
SpawnProfile profile{
    .gameObjectPoolId = GameObjectPoolId{"enemies"},
    .spawnPlacer = std::make_unique<RandomSpawnPlacer>(),
    .spawnInitializer = std::make_unique<MoveInitializer>(DirectionType::Random)
};

// SpawnManager uses pools internally
spawnManager.addSpawnProfile(SpawnProfileId{"enemy_spawn"}, std::move(profile));
```

When a spawn is requested, the SpawnManager:
1. Acquires an entity from the pool
2. Applies the placer to set position
3. Runs the initializer to configure the entity
4. Activates the entity

When despawning:
1. Deactivates the entity
2. Calls `onRelease()` for cleanup
3. Returns the entity to the pool

## Best Practices

### 1. Size Pools Appropriately

```cpp
// Too small: acquire() may return nullptr during gameplay
auto config = std::make_unique<GameObjectPoolConfig>(poolId, prefab, 10);

// Better: estimate peak usage with safety margin
auto config = std::make_unique<GameObjectPoolConfig>(poolId, prefab, 100);
```

### 2. Implement onAcquire/onRelease

Components should implement lifecycle callbacks for proper state reset:

```cpp
class ProjectileComponent : public CloneableComponent<ProjectileComponent> {
public:
    void onAcquire() noexcept {
        // Reset to initial state
        velocity_ = vec3f{0, 0, 0};
        damage_ = defaultDamage_;
    }

    void onRelease() noexcept {
        // Cleanup if needed
    }
};
```

### 3. Use Pool Snapshots for Debugging

```cpp
auto snapshot = poolManager.poolSnapshot(GameObjectPoolId{"bullets"});
logger.debug("Bullets: {} active, {} available",
    snapshot.activeCount, snapshot.inactiveCount);
```

### 4. Handle Pool Exhaustion Gracefully

```cpp
auto* entity = poolManager.acquire(poolId);
if (!entity) {
    // Pool exhausted - skip spawn or use fallback behavior
    logger.warn("Pool '{}' exhausted", poolId);
    return;
}
```

## Related Modules

| Module | Purpose |
|--------|---------|
| `helios.engine.runtime.pooling` | Pool classes and manager |
| `helios.engine.runtime.spawn` | Spawn system integration |
| `helios.engine.ecs` | EntityHandle, GameObject |
| `helios.engine.core.data` | EntityId, VersionId, GameObjectPoolId |

## See Also

- [Spawn System](./spawn-system.md) - Entity spawning and despawning
- [Command System](./command-system.md) - Command-based spawn requests
- [Component System](./component-system.md) - CloneableComponent lifecycle

