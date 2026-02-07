# helios::engine::runtime::pooling

GameObject pooling for efficient object recycling.

## Overview

This module provides infrastructure for memory-efficient entity management through object pooling. Pools pre-allocate GameObjects that can be acquired and released during gameplay, avoiding repeated allocation overhead.

## Key Classes

| Class | Purpose |
|-------|---------|
| `GameObjectPool` | Container holding pre-allocated GameObject identifiers with O(1) acquire/release |
| `GameObjectPoolConfig` | Configuration data for pool initialization (ID, prefab, size) |
| `GameObjectPoolManager` | High-level manager for pool lifecycle and acquire/release operations |
| `GameObjectPoolRegistry` | Central registry mapping pool IDs to pool instances |
| `GameObjectPoolSnapshot` | Read-only snapshot of pool state (active/inactive counts) |

## Architecture

The pooling system follows a layered architecture:

1. **GameObjectPool**: Low-level handle-based tracking (active/inactive lists)
2. **GameObjectPoolRegistry**: Pool lookup by typed ID
3. **GameObjectPoolManager**: Integration with GameWorld for prefab cloning and entity lookup
4. **GameObjectPoolConfig**: Declarative pool configuration

Spawn/despawn request handling has been moved to `helios::engine::runtime::spawn`.

## Usage

```cpp
// 1. Create a prefab template
auto bulletPrefab = GameObjectFactory::gameObject()
    .withMotion([](auto& m) { m.move2D().speed(10.0f); })
    .withRendering([&](auto& r) { r.renderable().shape(circleShape); })
    .make(false);  // inactive initially

// 2. Configure and create pool
GameObjectPoolConfig config{
    .poolId = GameObjectPoolId{1},
    .prefab = bulletPrefab.get(),
    .initialSize = 50
};

auto poolManager = std::make_unique<GameObjectPoolManager>(&gameWorld);
poolManager->createPool(config);

// 3. Acquire/Release via SpawnManager (preferred)
// Or directly via poolManager for low-level access
auto* bullet = poolManager->acquire(GameObjectPoolId{1});
bullet->setActive(true);

// Later: release back to pool
poolManager->release(GameObjectPoolId{1}, bullet->guid());
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::runtime::pooling
@brief GameObject pooling for efficient object recycling.
@details Provides GameObjectPool for handle-based entity tracking, registries for pool lookup, configuration structures for declarative pool setup, and managers for integration with GameWorld.
</p></details>

