# helios::engine::core::data

Data structures for efficient entity management and querying.

This module provides core data structures used for managing GameObjects within the helios engine.

## Components

- **CollisionLayer** - Strongly-typed identifier for collision layers.
- **ComponentTypeId** - Compile-time type identifier for O(1) component indexing within GameObject.
- **EntityId** - Unique identifier for an entity within an EntityPool.
- **FontId** - Strongly-typed identifier for referencing fonts.
- **GameObjectPool** - Low-level O(1) object pooling for entity recycling.
- **GameObjectPoolId** - Strongly-typed identifier for referencing pools.
- **GameObjectPoolRegistry** - Central registry managing multiple pools by ID.
- **GameObjectFilter** - Bitmask enum for filtering entities by active/inactive state and component state.
- **GameObjectView** - Lazy range adapter for component-filtered iteration with structured binding support.
- **ScorePoolId** - Strongly-typed identifier for score pools.
- **ScoreTypeId** - Strongly-typed identifier for score types.
- **SpawnProfileId** - Strongly-typed identifier for spawn profiles.
- **SpawnRuleId** - Strongly-typed identifier for spawn rules.
- **VersionId** - Version number for stale entity handle detection.
- **ViewportId** - Strongly-typed identifier for viewports.

## Strongly-Typed IDs

### String-Based IDs (FNV-1a Hashing)

The ID types `FontId`, `GameObjectPoolId`, `ScorePoolId`, `SpawnProfileId`, `SpawnRuleId`, and `ViewportId` provide type-safe identifiers constructed from string literals using FNV-1a hashing:

```cpp
// Compile-time constant IDs from strings
constexpr GameObjectPoolId ENEMY_POOL{"enemies"};
constexpr SpawnProfileId ENEMY_PROFILE{"enemy_spawn"};
constexpr SpawnRuleId WAVE_RULE{"wave_spawn"};
constexpr ScorePoolId PLAYER_SCORE{"player"};
constexpr ViewportId UI_VIEWPORT{"Ui"};

// Use in registries and schedulers
poolManager.createPool(ENEMY_POOL, 100);
spawnManager.registerProfile(ENEMY_PROFILE, profile);
scheduler.addRule(ENEMY_PROFILE, std::make_unique<TimerSpawnRule>(WAVE_RULE, 2.0f, 5));
```

### Type-Based IDs (TypeIndexer)

`ComponentTypeId` and `ScoreTypeId` use `TypeIndexer` to generate unique, monotonically increasing IDs for each type at compile time:

```cpp
// Get unique ID for a component type
auto typeId = ComponentTypeId::id<HealthComponent>();

// Get unique ID for a score type
auto scoreId = ScoreTypeId::id<KillScore>();
```

### Entity IDs (EntityPool)

`EntityId` and `VersionId` are used together in `EntityHandle` for safe entity references within an `EntityPool`:

```cpp
// EntityHandle combines EntityId + VersionId
struct EntityHandle {
    EntityId entityId;   // Index into sparse array
    VersionId versionId; // Detects stale references
};

// Pool operations return handles
EntityPool<GameObject> pool;
EntityHandle handle = pool.emplace(std::make_unique<GameObject>());

// Handle validation detects removed entities
auto* entity = pool.get(handle);  // nullptr if handle is stale
```

All ID types support:
- Hashing for `std::unordered_map` / `std::unordered_set`
- Comparison operators for `std::map` / `std::set`
- Equality comparison

> **Note:** `GameObjectView.h` is intentionally a .h header instead of a .ixx module interface due to MSVC ICE issues with structured bindings.

> **Tip:** Use `GameObjectPoolFacade` (in `helios.engine.facade`) to coordinate pool operations with the GameWorld. The facade validates entities, handles stale pool entries, and deactivates released objects automatically.

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::core::data
@brief Data structures for efficient entity management.
@details This namespace provides core data structures for managing GameObjects, including object pools for memory-efficient entity recycling (GameObjectPool, GameObjectPoolId, GameObjectPoolRegistry), filtered views for querying entities based on state or components (GameObjectFilter, GameObjectView). For coordinated pool/world operations, see GameObjectPoolFacade.
</p></details>
