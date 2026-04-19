# helios::runtime::pooling

Entity pooling for efficient object recycling.

## Overview

This module provides infrastructure for memory-efficient entity management through object pooling. Pools pre-allocate Entities that can be acquired and released during gameplay, avoiding repeated allocation overhead.

## Key Classes

| Class | Purpose |
|-------|---------|
| `EntityPool` | Container holding pre-allocated Entity identifiers with O(1) acquire/release |
| `EntityPoolConfig` | Configuration data for pool initialization (ID, prefab, size) |
| `EntityPoolManager` | High-level manager for pool lifecycle and acquire/release operations |
| `EntityPoolRegistry` | Central registry mapping pool IDs to pool instances |
| `EntityPoolSnapshot` | Read-only snapshot of pool state (active/inactive counts) |

## Architecture

The pooling system follows a layered architecture:

1. **EntityPool**: Low-level handle-based tracking (active/inactive lists)
2. **EntityPoolRegistry**: Pool lookup by typed ID
3. **EntityPoolManager**: Integration with GameWorld for prefab cloning and entity lookup
4. **EntityPoolConfig**: Declarative pool configuration

Spawn/despawn request handling has been moved to `helios::gameplay::spawn`.

## Usage

```cpp
// 1. Create a prefab template
auto bulletPrefab = EntityFactory::entity()
    .withMotion([](auto& m) { m.move2D().speed(10.0f); })
    .withRendering([&](auto& r) { r.renderable().shape(circleShape); })
    .make(false);  // inactive initially

// 2. Configure and create pool
EntityPoolConfig config{
    .poolId = EntityPoolId{1},
    .prefab = bulletPrefab.get(),
    .initialSize = 50
};

auto poolManager = std::make_unique<EntityPoolManager>(&gameWorld);
poolManager->createPool(config);

// 3. Acquire/Release via SpawnManager (preferred)
// Or directly via poolManager for low-level access
auto* bullet = poolManager->acquire(EntityPoolId{1});
bullet->setActive(true);

// Later: release back to pool
poolManager->release(EntityPoolId{1}, bullet->guid());
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::runtime::pooling
@brief Entity pooling for efficient object recycling.
@details Provides EntityPool for handle-based entity tracking, registries for pool lookup, configuration structures for declarative pool setup, and managers for integration with GameWorld.
</p></details>

