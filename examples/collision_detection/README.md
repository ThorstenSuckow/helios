# Collision Detection Example

This example demonstrates the grid-based collision detection system with multiple entity types, collision responses, and visual feedback for debugging.

## Features

- **Grid-Based Collision Detection** - Spatial partitioning with `GridCollisionDetectionSystem`
- **Multiple Collision Behaviors** - Bounce, Reflect, Despawn based on entity type
- **Collision Events** - `TriggerCollisionEvent` and `SolidCollisionEvent` for game logic
- **Entity Spawning** - Timed enemy spawning with configurable spawn rules
- **Projectile System** - Twin-stick shooting with bullet pooling
- **Visual Collision Feedback** - Color changes on collision for debugging
- **Complete GameLoop** - Full Pre/Main/Post phase pipeline with commit points

## Building

```bash
cmake -S . -B build
cmake --build build --target collision_detection
```

## Running

```bash
./build/bin/collision_detection
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
| `main.cpp` | Application entry point with collision setup |
| `CollisionId.ixx` | Collision layer identifiers |
| `_module.ixx` | Module aggregation |

### Components Used

| Component | Purpose |
|-----------|---------|
| `CollisionComponent` | Collision layer and mask configuration |
| `AabbColliderComponent` | World-space bounding box |
| `LevelBoundsBehaviorComponent` | Arena boundary response |
| `Move2DComponent` | 2D physics simulation |
| `ShootComponent` | Projectile firing with cooldown |
| `Aim2DComponent` | Aiming direction and fire frequency |
| `SpinComponent` | Continuous rotation animation |

### Systems Used

| System | Purpose |
|--------|---------|
| `GridCollisionDetectionSystem` | Spatial grid broadphase + AABB narrowphase |
| `CollisionStateResponseSystem` | Processes collision events |
| `CollisionStateClearSystem` | Clears collision flags per frame |
| `BoundsUpdateSystem` | Updates AABB from transforms |
| `LevelBoundsBehaviorSystem` | Handles arena boundary collisions |
| `ProjectileSpawnSystem` | Spawns projectiles from ShootComponent |
| `GameObjectSpawnSystem` | Timed enemy spawning |

## Collision Layer System

Entities are assigned collision layers and masks to control which entities can collide:

```cpp
// Define collision layers
enum class CollisionId : uint32_t {
    Player      = 1 << 0,
    Enemy       = 1 << 1,
    Projectile  = 1 << 2,
    Boundary    = 1 << 3
};

// Configure player collision
shipGameObject.get<CollisionComponent>()
    ->setLayer(CollisionId::Player)
    ->setMask(CollisionId::Enemy | CollisionId::Boundary);

// Configure enemy collision
enemyPrefab->get<CollisionComponent>()
    ->setLayer(CollisionId::Enemy)
    ->setMask(CollisionId::Player | CollisionId::Projectile);
```

## Grid-Based Collision Detection

The `GridCollisionDetectionSystem` uses spatial partitioning for efficient broadphase:

```cpp
// Add collision detection system with grid cell size
gameLoop.phase(PhaseType::Main)
    .addPass()
    .addSystem<GridCollisionDetectionSystem>(cellSize)
    .addSystem<CollisionStateResponseSystem>()
    .addCommitPoint()
    .addPass()
    .addSystem<CollisionStateClearSystem>();
```

## Collision Behaviors

Different entities respond differently to collisions:

| Entity | Level Bounds | Entity Collision |
|--------|--------------|------------------|
| Player | Bounce | Push back |
| Enemy | Reflect | Continue |
| Projectile | Despawn | Despawn on hit |

```cpp
// Player bounces off walls
shipBuilder.withCollision([](auto& cb) {
    cb.collision().useBoundingBox();
    cb.levelBoundsCollision()
      .onCollision(CollisionBehavior::Bounce);
});

// Projectiles despawn at boundaries
projectileBuilder.withCollision([](auto& cb) {
    cb.collision().useBoundingBox();
    cb.levelBoundsCollision()
      .onCollision(CollisionBehavior::Despawn);
});
```

## GameLoop Phase Configuration

```cpp
// Pre-Phase: Input, Spawning, Physics
gameLoop.phase(PhaseType::Pre)
    .addPass()
    .addSystem<TwinStickInputSystem>(shipGameObject)
    .addCommitPoint(CommitPoint::Structural)
    .addPass()
    .addSystem<ProjectileSpawnSystem>(ProjectileSpawnProfileId)
    .addSystem<GameObjectSpawnSystem>(spawnSchedulers)
    .addCommitPoint(CommitPoint::Structural)
    .addPass()
    .addSystem<ScaleSystem>()
    .addSystem<SteeringSystem>()
    .addSystem<SpinSystem>()
    .addSystem<Move2DSystem>();

// Main-Phase: Collision Detection and Response
gameLoop.phase(PhaseType::Main)
    .addPass()
    .addSystem<BoundsUpdateSystem>()
    .addSystem<GridCollisionDetectionSystem>(cellSize)
    .addSystem<CollisionStateResponseSystem>()
    .addSystem<LevelBoundsBehaviorSystem>()
    .addCommitPoint()
    .addPass()
    .addSystem<CollisionStateClearSystem>();

// Post-Phase: Transform and Rendering
gameLoop.phase(PhaseType::Post)
    .addPass()
    .addSystem<ComposeTransformSystem>()
    .addSystem<SceneSyncSystem>(scene.get())
    .addSystem<TransformClearSystem>();
```

## Collision Events

The collision system emits events that can be consumed by game logic:

```cpp
// Read collision events in a system
void update(UpdateContext& ctx) {
    for (const auto& event : ctx.readPhaseEvents<TriggerCollisionEvent>()) {
        // Handle trigger collision (e.g., pickup, damage zone)
    }
    
    for (const auto& event : ctx.readPhaseEvents<SolidCollisionEvent>()) {
        // Handle solid collision (e.g., physics response)
    }
}
```

## ImGui Integration

Full debug overlay with:
- FPS monitoring and frame pacing
- Gamepad state visualization
- Log console with scope filtering
- Camera manipulation
- Physics parameter tuning

```cpp
imguiOverlay.addWidget(new MainMenuWidget());
imguiOverlay.addWidget(new FpsWidget(&metrics, &framePacer));
imguiOverlay.addWidget(new GamepadWidget(&inputManager));
imguiOverlay.addWidget(new LogWidget());
imguiOverlay.addWidget(new CameraWidget());
```
