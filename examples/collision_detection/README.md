# Collision Detection Example

This example demonstrates the grid-based collision detection system with multiple entity types, collision responses, and the `SpawnSystemFactory` DSL for complex spawn patterns including cyclic edge spawning.

## Features

- **Grid-Based Collision Detection** - Spatial partitioning with `GridCollisionDetectionSystem`
- **Multiple Collision Behaviors** - Bounce, Reflect, Despawn based on entity type
- **Collision Events** - `TriggerCollisionEvent` and `SolidCollisionEvent` for game logic
- **SpawnSystemFactory DSL** - Declarative spawn configuration with custom placers and cyclic schedulers
- **Projectile System** - Twin-stick shooting with bullet pooling via `PrefabId`
- **State Management** - Game state tracking via `Session` with typed `GameState` transitions
- **Automated Rendering** - `SceneRenderingSystem` handles viewport-based rendering
- **AI Chase System** - Blue enemies chase the player using `ChaseSystem`
- **Typed Passes** - Systems execute conditionally based on `GameState`
- **Complete GameLoop** - Full Pre/Main/Post phase pipeline with commit points

## Building

```bash
cmake -S . -B build
cmake --build build --target collision_detectionmain
```

## Running

```bash
./build/examples/collision_detection/main
```

## Controls

| Input | Action |
|-------|--------|
| Left Stick | Move spaceship / Rotate |
| Right Stick | Aim direction / Fire projectiles |
| ESC | Exit application |
| ~ (Tilde) | Toggle ImGui overlay |

## Code Structure

| File | Purpose |
|------|---------|
| `main.cpp` | Application entry point with collision setup |
| `CollisionId.ixx` | Collision layer identifiers |
| `_module.ixx` | Module aggregation |

### Components Used

| Component | Purpose |
|-----------|---------|
| `CollisionComponent` | Collision layer and mask configuration |
| `CollisionStateComponent` | Per-frame collision results |
| `AabbColliderComponent` | World-space bounding box |
| `LevelBoundsBehaviorComponent` | Arena boundary response |
| `Move2DComponent` | 2D physics simulation |
| `SteeringComponent` | Direction-based rotation |
| `ShootComponent` | Projectile firing with cooldown |
| `Aim2DComponent` | Aiming direction and fire frequency |
| `SpinComponent` | Continuous rotation animation |
| `ChaseComponent` | AI chasing behavior |
| `PrefabIdComponent` | Identifies prefab type for pooling |
| `Active` / `Inactive` | Entity activation state tags |

### Systems Used

| System | Purpose |
|--------|---------|
| `GridCollisionDetectionSystem` | Spatial grid broadphase + AABB narrowphase |
| `CollisionStateResponseSystem` | Processes collision events |
| `CollisionStateClearSystem` | Clears collision flags per frame |
| `BoundsUpdateSystem` | Updates AABB from transforms |
| `LevelBoundsBehaviorSystem` | Handles arena boundary collisions |
| `ProjectileSpawnSystem` | Spawns projectiles from ShootComponent |
| `GameObjectSpawnSystem` | Processes spawn schedulers |
| `ChaseSystem` | AI pursuit behavior |
| `HierarchyPropagationSystem` | Propagates transforms through hierarchies |
| `StateToViewportPolicyUpdateSystem` | Activates viewports based on game state |
| `SceneSyncSystem` | Syncs transforms to scene graph |
| `SceneRenderingSystem` | Renders scenes to associated viewports |

## Collision Layer System

Entities are assigned collision layers and masks via the `GameObjectFactory`:

```cpp
.withCollision([](auto& cb) {
    cb.collision()
      .layerId(CollisionId::Player)
      .useBoundingBox()
      .hitPolicy(HitPolicy::OneHit)
      .reportCollisions(true)
      .solidCollisionMask(CollisionId::Enemy)
      .onSolidCollision(CollisionId::Enemy, CollisionBehavior::Despawn);

    cb.levelBoundsCollision()
      .onCollision(CollisionBehavior::Bounce);
})
```

## Spawn System Configuration (DSL)

The example uses complex spawn patterns including cyclic edge spawning:

```cpp
SpawnSystemFactory::configure(poolManager, spawnManager)
    // Projectiles: emitter-relative placement
    .pool(ProjectilePoolId, ProjectilePrefabId, 50)
        .profile(ProjectileSpawnProfileId)
            .emitterPlacement()
            .done()
        .commit()

    // Purple enemies: random placement, timed
    .pool(PurpleEnemyPoolId, PurpleEnemyPrefabId, 50)
        .profile(RandomSpawnProfileId)
            .randomPlacement()
            .randomDirectionInitializer()
            .scheduledBy(PurpleSpawnRuleId)
                .timerCondition(5.0f)
                .fixedAmount(1)
                .done()
            .done()
        .commit()

    // Orange enemies: cyclic edge spawning from four directions
    .pool(OrangeEnemyPoolId, OrangeEnemyPrefabId, OBJECT_AMOUNT_X)
        .profile(LeftColumnProfileId)
            .axisPlacement(vec3f(0, -1, 0).normalize(), TOP_LEFT)
            .moveInitializer(X_AXISf)
            .scheduledBy(LeftColumnRuleId)
                .timerWithAvailabilityCondition(15.0f)
                .fixedAmount(OBJECT_AMOUNT_Y)
                .done()
            .done()
        // ... profiles for Top, Right, Bottom edges
        .commitCyclic<4>();
```

## Collision Behaviors

Different entities respond differently to collisions:

| Entity | Level Bounds | Entity Collision |
|--------|--------------|------------------|
| Player | Bounce | Despawn on enemy hit |
| Purple Enemy | Reflect | Despawn on player/projectile |
| Orange Enemy | Bounce + align heading | Despawn on player/projectile |
| Blue Enemy | Reflect + align heading | Despawn on player/projectile |
| Projectile | Despawn | Despawn on enemy hit |

## GameLoop Phase Configuration

```cpp
// Pre-Phase: Input, Spawning, Physics
gameLoop.phase(PhaseType::Pre)
    .addPass<GameState>(GameState::Any)
    .addSystem<TwinStickInputSystem>(shipGameObject)
    .addCommitPoint(CommitPoint::Structural)
    .addPass<GameState>(GameState::Any)
    .addSystem<GameObjectSpawnSystem>(spawnManager)
    .addSystem<ProjectileSpawnSystem>(ProjectileSpawnProfileId)
    .addCommitPoint(CommitPoint::Structural)
    .addPass<GameState>(GameState::Any)
    .addSystem<ChaseSystem>()
    .addSystem<ScaleSystem>()
    .addSystem<SteeringSystem>()
    .addSystem<SpinSystem>()
    .addSystem<Move2DSystem>();

// Main-Phase: Collision Detection and Response
gameLoop.phase(PhaseType::Main)
    .addPass<GameState>(GameState::Any)
    .addSystem<HierarchyPropagationSystem>()
    .addSystem<BoundsUpdateSystem>()
    .addSystem<LevelBoundsBehaviorSystem>()
    .addSystem<GridCollisionDetectionSystem>(bounds, cellSize)
    .addCommitPoint()
    .addPass<GameState>(GameState::Any)
    .addSystem<CollisionStateResponseSystem>();

// Post-Phase: Transform and Rendering
gameLoop.phase(PhaseType::Post)
    .addPass<GameState>(GameState::Any)
    .addSystem<ComposeTransformSystem>()
    .addSystem<StateToViewportPolicyUpdateSystem<GameState, MatchState>>(stateToViewportMap)
    .addSystem<SceneSyncSystem>(sceneToViewportMap)
    .addSystem<SceneRenderingSystem>(renderingDevice, sceneToViewportMap)
    .addSystem<TransformClearSystem>()
    .addSystem<DelayedComponentEnablerSystem>()
    .addSystem<CollisionStateClearSystem>();
```

## See Also

- [Component System](../../docs/core-concepts/component-system.md) - Component architecture and lifecycle
- [Spawn System](../../docs/core-concepts/spawn-system.md) - Spawn pipeline and DSL builder
- [Game Loop Architecture](../../docs/core-concepts/gameloop-architecture.md) - Phase-based game loop
- [Enemy Spawn Example](../enemy_spawn/README.md) - Simpler spawn system demo
- [Scoring Demo](../scoring_demo/README.md) - Full gameplay with scoring and menus
