# Spaceship Shooting Example

This example extends the spaceship control demo with a complete twin-stick shooting system, demonstrating projectile spawning via the `SpawnSystemFactory` DSL, aiming components, level bounds collision, and state management.

## Features

- **Twin-Stick Shooter Controls** - Left stick for movement, right stick for aiming and shooting
- **GameLoop Architecture** - Phase-based execution with Pre/Main/Post phases, typed passes, and commit points
- **GameObjectFactory** - Fluent builder pattern for creating player, projectiles, and grid
- **SpawnSystemFactory DSL** - Declarative spawn configuration for projectile pooling
- **Projectile Spawning** - `ProjectileSpawnSystem` with spawn profiles and pooling
- **Level Bounds System** - Arena boundaries with bounce/despawn behavior
- **State Management** - Game state tracking via `Session` with typed `GameState` transitions
- **Automated Rendering** - `SceneRenderingSystem` handles viewport-based rendering
- **ShootComponent** - Configurable fire rate and projectile speed
- **Aim2DComponent** - Direction tracking and firing frequency management
- **Object Pooling** - Efficient projectile management via `GameObjectPoolManager`
- **PrefabId** - Strongly-typed prefab identification for pooled entities

## Building

```bash
cmake -S . -B build
cmake --build build --target spaceship_shootingmain
```

## Running

```bash
./build/examples/spaceship_shooting/main
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
| `main.cpp` | Application entry point and game loop |
| `SpaceshipWidget.ixx` | ImGui widget for physics and fire rate parameter tuning |

### Components Used

| Component | Purpose |
|-----------|---------|
| `SceneNodeComponent` | Links GameObject to scene graph |
| `Move2DComponent` | 2D physics with acceleration and dampening |
| `SteeringComponent` | Rotation control and direction |
| `Aim2DComponent` | Tracks aim direction and firing frequency |
| `ShootComponent` | Manages projectile firing with cooldown |
| `LevelBoundsBehaviorComponent` | Bounce/despawn at arena boundaries |
| `AabbColliderComponent` | World-space collision bounds |
| `SpawnedByProfileComponent` | Links entity to spawn profile |
| `PrefabIdComponent` | Identifies prefab type for pooling |
| `Active` / `Inactive` | Entity activation state tags |

### Systems Used

| System | Purpose |
|--------|---------|
| `TwinStickInputSystem` | Dual analog stick input handling |
| `ProjectileSpawnSystem` | Spawns projectiles from ShootComponent |
| `HierarchyPropagationSystem` | Propagates transforms through entity hierarchies |
| `ScaleSystem` | Applies ScaleStateComponent sizing |
| `SteeringSystem` | Rotation and direction updates |
| `Move2DSystem` | Physics simulation (velocity integration) |
| `BoundsUpdateSystem` | Updates AABB colliders from transforms |
| `LevelBoundsBehaviorSystem` | Handles boundary collisions |
| `ComposeTransformSystem` | Computes final transforms |
| `StateToViewportPolicyUpdateSystem` | Activates viewports based on game state |
| `SceneSyncSystem` | Syncs transforms to scene graph |
| `SceneRenderingSystem` | Renders scenes to associated viewports |
| `TransformClearSystem` | Clears dirty flags post-frame |

## Spawn System Configuration (DSL)

```cpp
SpawnSystemFactory::configure(poolManager, spawnManager)
    .pool(ProjectilePoolId, ProjectilePrefabId, 50)
        .profile(ProjectileSpawnProfileId)
            .emitterPlacement()
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
    .addSystem<ProjectileSpawnSystem>(ProjectileSpawnProfileId)
    .addCommitPoint(CommitPoint::Structural)
    .addPass<GameState>(GameState::Any)
    .addSystem<ScaleSystem>()
    .addSystem<SteeringSystem>()
    .addSystem<Move2DSystem>();

// Main-Phase: Collision and Bounds
gameLoop.phase(PhaseType::Main)
    .addPass<GameState>(GameState::Any)
    .addSystem<HierarchyPropagationSystem>()
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

## Command Dispatchers

The spawn system uses commands for deferred entity creation:

```cpp
gameLoop.commandBuffer()
    .addDispatcher<SpawnCommand>(std::make_unique<SpawnCommandDispatcher>())
    .addDispatcher<DespawnCommand>(std::make_unique<DespawnCommandDispatcher>());
```

## See Also

- [Spawn System](../../docs/core-concepts/spawn-system.md) - Spawn pipeline and DSL builder
- [Game Loop Architecture](../../docs/core-concepts/gameloop-architecture.md) - Phase-based game loop
- [Spaceship Control Example](../spaceship_control/README.md) - Basic movement controls
- [Collision Detection Example](../collision_detection/README.md) - Grid-based collision
- [Scoring Demo](../scoring_demo/README.md) - Full gameplay with scoring and menus
