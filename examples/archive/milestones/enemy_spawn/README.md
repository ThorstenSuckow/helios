# Enemy Spawn Example

This example demonstrates the spawn system with timed enemy spawning, object pooling, and movement behaviors using the helios game loop architecture with typed passes and state management.

## Features

- **GameLoop Architecture** - Phase-based game loop with Pre/Main/Post phases, typed passes, and commit points
- **SpawnSystemFactory DSL** - Declarative spawn configuration with fluent builder API
- **Object Pooling** - Efficient enemy management via `GameObjectPoolManager` with `PrefabId`
- **Random Placement** - Enemies spawn at random positions within level bounds
- **MoveInitializer** - Random initial movement direction for spawned entities
- **SpinComponent** - Visual rotation effect on enemies
- **Level Bounds** - Enemies reflect off arena boundaries
- **GameObjectFactory** - Fluent builder pattern for creating GameObjects
- **State Management** - Game state tracking via `Session`
- **Automated Rendering** - `SceneRenderingSystem` handles viewport-based rendering

## Building

```bash
cmake -S . -B build
cmake --build build --target enemy_spawnmain
```

## Running

```bash
./build/examples/enemy_spawn/main
```

## Controls

| Input | Action |
|-------|--------|
| Left Stick | Move spaceship / Rotate |
| ESC | Exit application |
| ~ (Tilde) | Toggle ImGui overlay |

## Code Structure

| File | Purpose |
|------|---------|
| `main.cpp` | Application entry point with spawn configuration |

### Components Used

| Component | Purpose |
|-----------|---------|
| `SceneNodeComponent` | Links GameObject to scene graph |
| `Move2DComponent` | 2D physics with velocity and acceleration |
| `SpinComponent` | Continuous rotation animation |
| `ComposeTransformComponent` | Local/world transform composition |
| `ScaleStateComponent` | Unit-based sizing (meters) |
| `LevelBoundsBehaviorComponent` | Reflect behavior at arena boundaries |
| `AabbColliderComponent` | World-space collision bounds |
| `SpawnedByProfileComponent` | Links entity to spawn profile |
| `PrefabIdComponent` | Identifies prefab type for pooling |
| `Active` / `Inactive` | Entity activation state tags |

### Systems Used

| System | Purpose |
|--------|---------|
| `GameObjectSpawnSystem` | Processes spawn schedulers and triggers spawning |
| `SpinSystem` | Applies rotation from SpinComponent |
| `Move2DSystem` | Physics simulation (velocity integration) |
| `SteeringSystem` | Direction-based rotation |
| `ScaleSystem` | Applies ScaleStateComponent sizing |
| `BoundsUpdateSystem` | Updates AABB colliders from transforms |
| `LevelBoundsBehaviorSystem` | Handles boundary reflections |
| `ComposeTransformSystem` | Computes final transforms |
| `StateToViewportPolicyUpdateSystem` | Activates viewports based on game state |
| `SceneSyncSystem` | Syncs transforms to scene graph |
| `SceneRenderingSystem` | Renders scenes to associated viewports |
| `TransformClearSystem` | Clears dirty flags post-frame |

## Spawn System Configuration (DSL)

```cpp
SpawnSystemFactory::configure(poolManager, spawnManager)
    .pool(EnemyPoolId, EnemyPrefabId, 200)
        .profile(RandomSpawnProfileId)
            .randomPlacement()
            .randomDirectionInitializer()
            .scheduledBy(SpawnRuleId)
                .timerCondition(5.0f)
                .fixedAmount(1)
                .done()
            .done()
        .commit();
```

## GameLoop Phase Configuration

```cpp
// Pre-Phase: Input, Spawning, Physics
gameLoop.phase(PhaseType::Pre)
    .addPass<GameState>(GameState::Any)
    .addSystem<TwinStickInputSystem>(shipGameObject)
    .addCommitPoint(CommitPoint::Structural)
    .addPass<GameState>(GameState::Any)
    .addSystem<GameObjectSpawnSystem>(spawnManager)
    .addCommitPoint(CommitPoint::Structural)
    .addPass<GameState>(GameState::Any)
    .addSystem<ScaleSystem>()
    .addSystem<SteeringSystem>()
    .addSystem<SpinSystem>()
    .addSystem<Move2DSystem>();

// Main-Phase: Collision Detection
gameLoop.phase(PhaseType::Main)
    .addPass<GameState>(GameState::Any)
    .addSystem<BoundsUpdateSystem>()
    .addSystem<LevelBoundsBehaviorSystem>()
    .addCommitPoint();

// Post-Phase: Transform and Rendering
gameLoop.phase(PhaseType::Post)
    .addPass<GameState>(GameState::Any)
    .addSystem<ComposeTransformSystem>()
    .addSystem<StateToViewportPolicyUpdateSystem<GameState, MatchState>>(stateToViewportMap)
    .addSystem<SceneSyncSystem>(sceneToViewportMap)
    .addSystem<SceneRenderingSystem>(renderingDevice, sceneToViewportMap)
    .addSystem<TransformClearSystem>();
```

## See Also

- [Spawn System](../../docs/core-concepts/spawn-system.md) - Spawn pipeline and DSL builder
- [Object Pooling](../../docs/core-concepts/object-pooling.md) - Entity pooling system
- [Game Loop Architecture](../../docs/core-concepts/gameloop-architecture.md) - Phase-based game loop
- [Collision Detection Example](../collision_detection/README.md) - Grid-based collision
- [Scoring Demo](../scoring_demo/README.md) - Full gameplay with scoring and menus
