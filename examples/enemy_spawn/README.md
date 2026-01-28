# Enemy Spawn Example

This example demonstrates the spawn system with timed enemy spawning, object pooling, and movement behaviors using the helios game loop architecture.

## Features

- **GameLoop Architecture** - Phase-based game loop with Pre/Main/Post phases and commit points
- **Spawn System** - Timed spawning with `SpawnScheduler`, `SpawnProfile`, and `SpawnRule`
- **Object Pooling** - Efficient enemy management via `GameObjectPoolManager`
- **Random Placement** - Enemies spawn at random positions within level bounds
- **MoveInitializer** - Random initial movement direction for spawned entities
- **SpinComponent** - Visual rotation effect on enemies
- **Level Bounds** - Enemies reflect off arena boundaries
- **GameObjectFactory** - Fluent builder pattern for creating GameObjects

## Building

```bash
cmake -S . -B build
cmake --build build --target enemy_spawn
```

## Running

```bash
./build/bin/enemy_spawn
```

## Controls

| Input | Action |
|-------|--------|
| Left Stick | Move spaceship / Rotate |
| ESC | Exit application |

## Code Structure

| File | Purpose |
|------|---------|
| `main.cpp` | Application entry point with spawn configuration |

### Components Used

| Component | Purpose |
|-----------|---------|
| `SceneNodeComponent` | Links GameObject to scene graph |
| `Move2DComponent` | 2D physics with velocity and acceleration |
| `DirectionComponent` | Movement direction vector |
| `SpinComponent` | Continuous rotation animation |
| `ComposeTransformComponent` | Local/world transform composition |
| `ScaleStateComponent` | Unit-based sizing (meters) |
| `LevelBoundsBehaviorComponent` | Reflect behavior at arena boundaries |
| `AabbColliderComponent` | World-space collision bounds |

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
| `SceneSyncSystem` | Syncs transforms to scene graph |
| `TransformClearSystem` | Clears dirty flags post-frame |

## Spawn System Architecture

The example demonstrates the complete spawn pipeline:

```cpp
// 1. Create prefab with GameObjectFactory
auto enemyPrefab = GameObjectFactory::instance()
    .gameObject()
    .withRendering([&](auto& rnb) { /* ... */ })
    .withMotion([](auto& mcb) {
        mcb.move2D().speed(5.0f).instantAcceleration(true);
    })
    .withEffects([](auto& eb) {
        eb.gfx().spin(270.0f, helios::math::Z_AXISf);
    })
    .withSpawn([](auto& sb) {
        sb.spawn().useSpawnProfile();
    })
    .make();

// 2. Configure pool manager
auto& poolManager = gameWorld.addManager<GameObjectPoolManager>();
poolManager.addPoolConfig(std::make_unique<GameObjectPoolConfig>(
    PoolId, std::move(enemyPrefab), 200
));

// 3. Create spawn profile with placement and initialization
auto spawnProfile = std::make_unique<SpawnProfile>(
    PoolId,
    std::make_unique<RandomSpawnPlacer>(),
    std::make_unique<MoveInitializer>(DirectionType::Random)
);

// 4. Configure spawn scheduler with rules
auto scheduler = std::make_unique<SpawnScheduler>();
auto spawnRule = std::make_unique<SpawnRule>(
    std::make_unique<TimerSpawnCondition>(5.0f),  // Every 5 seconds
    std::make_unique<FixedSpawnAmount>(1),        // Spawn 1 enemy
    RuleId
);
scheduler->addRule(ProfileId, std::move(spawnRule));

// 5. Register dispatchers and add spawn system
gameLoop.commandBuffer().addDispatcher<ScheduledSpawnPlanCommand>(/*...*/);
gameLoop.commandBuffer().addDispatcher<SpawnCommand>(/*...*/);
gameLoop.commandBuffer().addDispatcher<DespawnCommand>(/*...*/);

gameLoop.phase(PhaseType::Pre)
    .addPass()
    .addSystem<GameObjectSpawnSystem>(spawnSchedulers);
```

## GameLoop Phase Configuration

```cpp
// Pre-Phase: Input, Spawning, Physics
gameLoop.phase(PhaseType::Pre)
    .addPass()
    .addSystem<TwinStickInputSystem>(*shipGameObject)
    .addCommitPoint(CommitPoint::Structural)
    .addPass()
    .addSystem<GameObjectSpawnSystem>(spawnSchedulers)
    .addCommitPoint(CommitPoint::Structural)
    .addPass()
    .addSystem<ScaleSystem>()
    .addSystem<SteeringSystem>()
    .addSystem<SpinSystem>()
    .addSystem<Move2DSystem>();

// Main-Phase: Collision Detection
gameLoop.phase(PhaseType::Main)
    .addPass()
    .addSystem<BoundsUpdateSystem>()
    .addSystem<LevelBoundsBehaviorSystem>()
    .addCommitPoint();

// Post-Phase: Transform Composition and Scene Sync
gameLoop.phase(PhaseType::Post)
    .addPass()
    .addSystem<ComposeTransformSystem>()
    .addSystem<SceneSyncSystem>(scene.get())
    .addSystem<TransformClearSystem>();
```

## ImGui Integration

The example includes debug widgets for:
- FPS monitoring and frame pacing control
- Gamepad state visualization
- Log console with scope filtering
- Camera manipulation

```cpp
imguiOverlay.addWidget(new MainMenuWidget());
imguiOverlay.addWidget(new FpsWidget(&metrics, &framePacer));
imguiOverlay.addWidget(new GamepadWidget(&inputManager));
imguiOverlay.addWidget(new LogWidget());
imguiOverlay.addWidget(new CameraWidget());
```
