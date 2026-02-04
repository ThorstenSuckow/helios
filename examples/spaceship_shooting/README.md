# Spaceship Shooting Example

This example extends the spaceship control demo with a complete twin-stick shooting system, demonstrating projectile spawning via the spawn system, aiming components, and level bounds collision.

## Features

- **Twin-Stick Shooter Controls** - Left stick for movement, right stick for aiming and shooting
- **GameLoop Architecture** - Phase-based execution with Pre/Main/Post phases and commit points
- **GameObjectFactory** - Fluent builder pattern for creating player, projectiles, and grid
- **Projectile Spawning** - `ProjectileSpawnSystem` with spawn profiles and pooling
- **Level Bounds System** - Arena boundaries with bounce/despawn behavior
- **ShootComponent** - Configurable fire rate and projectile speed
- **Aim2DComponent** - Direction tracking and firing frequency management
- **Object Pooling** - Efficient projectile management via `GameObjectPoolManager`

## Building

```bash
cmake -S . -B build
cmake --build build --target spaceship_shooting
```

## Running

```bash
./build/bin/spaceship_shooting
```

## Controls

| Input | Action |
|-------|--------|
| Left Stick | Move spaceship / Rotate |
| Right Stick | Aim direction / Fire projectiles |
| ESC | Exit application |

## Code Structure

| File | Purpose |
|------|---------|
| `main.cpp` | Application entry point and game loop |
| `SpaceshipWidget.ixx` | ImGui widget for physics parameter tuning |

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
| `SpawnProfileComponent` | Links entity to spawn profile |

### Systems Used

| System | Purpose |
|--------|---------|
| `TwinStickInputSystem` | Dual analog stick input handling |
| `ProjectileSpawnSystem` | Spawns projectiles from ShootComponent |
| `ScaleSystem` | Applies ScaleStateComponent sizing |
| `SteeringSystem` | Rotation and direction updates |
| `Move2DSystem` | Physics simulation (velocity integration) |
| `BoundsUpdateSystem` | Updates AABB colliders from transforms |
| `LevelBoundsBehaviorSystem` | Handles boundary collisions |
| `ComposeTransformSystem` | Computes final transforms |
| `SceneSyncSystem` | Syncs transforms to scene graph |
| `TransformClearSystem` | Clears dirty flags post-frame |

## Projectile Spawn System

The example demonstrates the spawn pipeline for projectiles:

```cpp
// 1. Create projectile prefab
auto projectilePrefab = GameObjectFactory::instance()
    .gameObject()
    .withRendering([&](auto& rnb) {
        rnb.meshRenderable()
           .shader(shader)
           .color(Colors::Yellow)
           .primitiveType(PrimitiveType::LineLoop)
           .shape(std::make_shared<Ellipse>(0.5f, 0.2f, 8))
           .attachTo(&root);
    })
    .withCollision([](auto& cb) {
        cb.collision().useBoundingBox();
        cb.levelBoundsCollision()
          .onCollision(CollisionBehavior::Despawn);
    })
    .withMotion([](auto& mcb) {
        mcb.move2D()
           .speed(80.0f)
           .instantAcceleration(true);
    })
    .withSpawn([](auto& sb) {
        sb.spawn().useSpawnProfile();
    })
    .make();

// 2. Configure pool manager
auto& poolManager = gameWorld.addManager<GameObjectPoolManager>();
poolManager.addPoolConfig(std::make_unique<GameObjectPoolConfig>(
    ProjectilePoolId, std::move(projectilePrefab), 50
));

// 3. Create spawn profile with emitter placement
auto spawnProfile = std::make_unique<SpawnProfile>(
    ProjectilePoolId,
    std::make_unique<EmitterSpawnPlacer>(),
    std::make_unique<EmitterInitializer>()
);
spawnManager.addSpawnProfile(ProfileId, std::move(spawnProfile));

// 4. Add ProjectileSpawnSystem
gameLoop.phase(PhaseType::Pre)
    .addPass()
    .addSystem<ProjectileSpawnSystem>(ProfileId);
```

## Ship with Shooting Components

```cpp
auto shipGameObject = GameObjectFactory::instance()
    .gameObject()
    .withRendering([&](auto& rnb) { /* ... */ })
    .withCollision([](auto& cb) {
        cb.collision().useBoundingBox();
        cb.levelBoundsCollision()
          .onCollision(CollisionBehavior::Bounce);
    })
    .withShooting([](auto& ccb) {
        ccb.weapon().fireRate(5.0f);  // 5 shots per second
    })
    .withMotion([](auto& mcb) {
        mcb.move2D().speed(30.0f).instantAcceleration(false);
        mcb.steering().steeringSetsDirection(true);
    })
    .make();
```

## GameLoop Phase Configuration

```cpp
// Pre-Phase: Input, Spawning, Physics
gameLoop.phase(PhaseType::Pre)
    .addPass()
    .addSystem<TwinStickInputSystem>(*shipGameObject)
    .addCommitPoint(CommitPoint::Structural)
    .addPass()
    .addSystem<ProjectileSpawnSystem>(ProjectileSpawnProfileId)
    .addCommitPoint(CommitPoint::Structural)
    .addPass()
    .addSystem<ScaleSystem>()
    .addSystem<SteeringSystem>()
    .addSystem<Move2DSystem>();

// Main-Phase: Collision and Bounds
gameLoop.phase(PhaseType::Main)
    .addPass()
    .addSystem<BoundsUpdateSystem>()
    .addSystem<LevelBoundsBehaviorSystem>()
    .addCommitPoint();

// Post-Phase: Transform and Rendering
gameLoop.phase(PhaseType::Post)
    .addPass()
    .addSystem<ComposeTransformSystem>()
    .addSystem<SceneSyncSystem>(scene.get())
    .addSystem<TransformClearSystem>();
```

## Command Dispatchers

The spawn system uses commands for deferred entity creation:

```cpp
gameLoop.commandBuffer().addDispatcher<SpawnCommand>(
    std::make_unique<SpawnCommandDispatcher>()
);
gameLoop.commandBuffer().addDispatcher<DespawnCommand>(
    std::make_unique<DespawnCommandDispatcher>()
);
```

## ImGui Integration

The example includes debug widgets for physics tuning:

```cpp
imguiOverlay.addWidget(new MainMenuWidget());
imguiOverlay.addWidget(new FpsWidget(&metrics, &framePacer));
imguiOverlay.addWidget(new GamepadWidget(&inputManager));
imguiOverlay.addWidget(new LogWidget());
imguiOverlay.addWidget(new CameraWidget());
imguiOverlay.addWidget(new SpaceshipWidget());
```
