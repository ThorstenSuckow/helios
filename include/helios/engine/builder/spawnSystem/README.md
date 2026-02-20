# helios.engine.builder.spawnSystem

Fluent, ID-centric builder API for configuring the spawn system: pools, profiles, and schedulers.

## Overview

`SpawnSystemFactory` provides an API where each **pool ID** is the entry point for all related configuration. Profiles and scheduling rules are nested under their pool, ensuring co-location of related IDs and reducing configuration errors.

## Architecture

```
spawnSystem/
├── _module.ixx                  # Module facade
├── SpawnSystemFactory.ixx       # Entry point (analog to GameObjectFactory)
├── README.md
├── builders/
│   ├── _module.ixx
│   ├── SchedulerBuilder.ixx     # Standalone scheduler registration
│   └── configs/
│       ├── _module.ixx
│       ├── SpawnPoolConfig.ixx  # ID-centric: pool → profiles → rules
│       └── SchedulerConfig.ixx  # Standalone rule config (for SchedulerBuilder)
```

## Usage

### ID-centric API (recommended)

```cpp
using namespace helios::engine::builder::spawnSystem;

SpawnSystemFactory::configure(poolManager, spawnManager)

    // Simple pool: no scheduling
    .pool(ProjectilePoolId, projectilePrefab, 50)
        .profile(ProjectileProfileId)
            .emitterPlacement()
            .done()
        .commit()

    // Pool with scheduled spawning
    .pool(EnemyPoolId, enemyPrefab, 200)
        .profile(EnemyProfileId)
            .randomPlacement()
            .randomDirectionInitializer()
            .scheduledBy(EnemyRuleId)
                .timerCondition(5.0f)
                .fixedAmount(1)
                .done()
            .done()
        .commit()

    // Multiple profiles with cyclic scheduling
    .pool(OrangePoolId, orangePrefab, 100)
        .profile(LeftProfileId)
            .axisPlacement(leftAxis, leftOrigin)
            .moveInitializer(rightDir)
            .scheduledBy(LeftRuleId)
                .timerWithAvailabilityCondition(15.0f)
                .fixedAmount(10)
                .done()
            .done()
        .profile(RightProfileId)
            .axisPlacement(rightAxis, rightOrigin)
            .moveInitializer(leftDir)
            .scheduledBy(RightRuleId)
                .timerWithAvailabilityCondition(15.0f)
                .fixedAmount(10)
                .done()
            .done()
        .commitCyclic<2>();
```

### Chained Pool Configuration

`commit()` and `commitCyclic<N>()` return a `SpawnSystemConfigurator` that provides `pool()` for starting the next pool configuration:

```cpp
SpawnSystemFactory::configure(poolManager, spawnManager)
    .pool(ProjectilePoolId, prefab1, 50)
        .profile(ProjectileProfileId).emitterPlacement().done()
        .commit()                    // ← returns configurator
    .pool(EnemyPoolId, prefab2, 200) // ← next pool on the return value
        .profile(EnemyProfileId).randomPlacement().done()
        .commit();
```

### Separate Scheduling with commitProfilesOnly()

When the same profiles need different scheduling strategies (e.g., in different levels), commit profiles without rules and attach schedulers separately:

```cpp
// Register pool + profiles (no schedulers created)
spawns.pool(OrangePoolId, orangePrefab, 100)
    .profile(LeftProfileId).axisPlacement(leftAxis, leftOrigin).moveInitializer(rightDir).done()
    .profile(RightProfileId).axisPlacement(rightAxis, rightOrigin).moveInitializer(leftDir).done()
    .commitProfilesOnly();

// Attach scheduling independently via SchedulerBuilder
SchedulerBuilder sb(spawnManager);
sb.cyclicScheduler(
    SchedulerConfig(LeftProfileId, LeftRuleId).timerCondition(5.0f).fixedAmount(10),
    SchedulerConfig(RightProfileId, RightRuleId).timerCondition(5.0f).fixedAmount(10)
);
```

## Design Decisions

- **ID-centric**: Pool ID is the root of all related config — no mismatched IDs possible
- **No GameLoop dependency**: Dispatcher registration is the caller's responsibility
- **commit() vs commitCyclic\<N\>()**: `commit()` creates per-profile DefaultSpawnSchedulers; `commitCyclic<N>()` bundles all rules into a single CyclicSpawnScheduler
- **commitProfilesOnly()**: Registers pool and profiles without schedulers, enabling separate scheduling via `SchedulerBuilder`
- **done() chaining**: Nested configs return to their parent via `done()`, enabling natural indentation
- **Move-only configs**: SpawnPoolConfig is not copyable (unique_ptr members). For reusable profile setups with different scheduling, use `commitProfilesOnly()` + `SchedulerBuilder`

## See Also

- [GameObjectFactory](../gameObject/README.md) — analogous builder for GameObjects
- [SpawnManager](../../runtime/spawn/README.md) — processes spawn/despawn commands
- [GameObjectPoolManager](../../runtime/pooling/README.md) — manages pre-allocated pools





