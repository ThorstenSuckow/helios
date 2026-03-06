# helios::engine::core::data

Strongly-typed identifiers and entity primitives for the helios engine.

## Overview

This module provides compile-time type-safe IDs used across the engine.
All IDs are lightweight value types supporting hashing, comparison, and
equality. Types that were previously in this module (GameObjectPool,
GameObjectView, ResourceTypeId, SystemTypeId) have been moved to their
respective subsystem modules.

## Contents

### String-Based IDs (StrongId / FNV-1a Hashing)

| Type | Purpose |
|------|---------|
| `CollisionLayer` | Strongly-typed identifier for collision layers |
| `FontId` | Identifier for referencing fonts |
| `GameObjectPoolId` | Identifier for referencing entity pools |
| `GameTimerId` | Identifier for game timers |
| `MenuId` | Identifier for menu instances |
| `PrefabId` | Identifier for prefab templates |
| `SceneId` | Identifier for scenes |
| `ViewportId` | Identifier for viewports |

### Type-Based IDs (TypeIndexer)

| Type | Purpose |
|------|---------|
| `CommandTypeId` | Compile-time type identifier for command types |
| `ComponentTypeId` | Compile-time type identifier for O(1) component indexing |
| `StateTypeId` | Compile-time type identifier for state types |

### Entity Primitives

| Type | Purpose |
|------|---------|
| `EntityId` | Unique identifier for an entity within the ECS |
| `VersionId` | Version number for stale entity handle detection |
| `EntityTombstone` | Sentinel value for invalid sparse array indices |

## Usage

```cpp
// Compile-time constant IDs from strings (FNV-1a)
constexpr GameObjectPoolId ENEMY_POOL{"enemies"};
constexpr ViewportId UI_VIEWPORT{"Ui"};
constexpr FontId MAIN_FONT{"arial"};

// Type-based IDs (monotonically increasing)
auto typeId = ComponentTypeId::id<HealthComponent>();
auto cmdId  = CommandTypeId::id<DespawnCommand>();
```

## Relocated Types

The following types were moved out of `engine.core.data`:

| Type | New Location |
|------|-------------|
| `ConceptModelRegistry` | `helios.core.container` |
| `ResourceTypeId` | `helios.engine.runtime.world.types` |
| `SystemTypeId` | `helios.engine.runtime.world.types` |
| `ScorePoolId`, `ScoreTypeId` | `helios.engine.mechanics.scoring` |
| `SpawnProfileId`, `SpawnRuleId` | `helios.engine.runtime.spawn` |
| `GameObjectPool`, `GameObjectPoolRegistry` | `helios.engine.runtime.pooling` |
| `GameObjectFilter`, `GameObjectView` | `helios.engine.ecs` |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::core::data
@brief Strongly-typed identifiers and entity primitives for the helios engine.
</p></details>
