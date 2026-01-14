# helios::engine::game::pool

Object pool components for entity recycling.

## Overview

This module provides components related to object pooling, enabling efficient entity reuse without repeated allocation/deallocation.

## Components

| Component | Purpose |
|-----------|---------|
| `PoolIdComponent` | Identifies which pool a GameObject belongs to |

## Usage

```cpp
import helios.engine.game.pool.components.PoolIdComponent;

// During pool population
gameObject->add<PoolIdComponent>(bulletPoolId);

// During despawn
auto* pic = gameObject->get<PoolIdComponent>();
if (pic) {
    auto* handler = gameWorld.poolRequestHandler(pic->gameObjectPoolId());
    handler->submit(DespawnRequest{gameObject->guid(), pic->gameObjectPoolId()});
}
```

## Related Modules

- `helios.engine.runtime.pooling.GameObjectPool` — Pool data structure
- `helios.engine.core.data.GameObjectPoolId` — Pool identifier
- `helios.engine.runtime.pooling.PoolRequestHandler` — Spawn/despawn request handling

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::game::pool
@brief Object pool components for entity recycling.
@details This module provides components related to object pooling, enabling efficient entity reuse without repeated allocation/deallocation. Includes PoolIdComponent for identifying which pool a GameObject belongs to.
</p></details>
