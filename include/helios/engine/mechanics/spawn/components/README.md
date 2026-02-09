# helios::engine::mechanics::spawn::components

Components for spawn-related entity tracking.

## Overview

This module provides components that track spawn-related metadata on GameObjects. These components enable the spawn/despawn pipeline to route entities to the correct pools and managers.

## Key Classes

| Class | Purpose |
|-------|---------|
| `SpawnedByProfileComponent` | Tracks which spawn profile created an entity |

## Usage

```cpp
// Attach to prefab during pool configuration
auto prefab = std::make_unique<GameObject>();
prefab->add<SpawnedByProfileComponent>();

// Set profile ID during spawn
auto* comp = entity.get<SpawnedByProfileComponent>();
comp->setSpawnProfileId(enemyProfileId);

// Read profile ID during despawn
auto profileId = entity.get<SpawnedByProfileComponent>()->spawnProfileId();
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::spawn::components
@brief Components for spawn-related entity tracking.
@details Contains components that store spawn metadata on GameObjects, enabling proper routing during the spawn/despawn lifecycle.
</p></details>
