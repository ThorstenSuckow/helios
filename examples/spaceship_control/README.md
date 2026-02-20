# Spaceship Control Example

This example demonstrates the complete GameLoop architecture with phase-based execution, the GameObjectFactory builder pattern, component-based game systems, state management, and ImGui debug overlay integration.

## Features

- **GameLoop Architecture** - Phase-based game loop (Pre/Post) with typed passes and commit points
- **GameObjectFactory** - Fluent builder pattern for creating GameObjects with components
- **Component System** - Composition-based entity design with Move2DComponent, SteeringComponent
- **Gamepad Input** - Control a spaceship using analog sticks via TwinStickInputSystem
- **State Management** - Game state tracking via `Session` with typed `GameState` transitions
- **Viewport Management** - State-driven viewport activation with `StateToViewportPolicyUpdateSystem`
- **Automated Rendering** - `SceneRenderingSystem` handles viewport-based rendering automatically
- **ImGui Debug Overlay** - Real-time logging, camera control, physics/fire rate tuning via dockable widgets
- **Frame Pacing** - Configurable target FPS with performance metrics
- **Scene Graph Camera** - Camera follows spaceship using transform inheritance
- **Units System** - Object sizes defined in meters (1 hu = 1 m)

## Building

```bash
cmake -S . -B build
cmake --build build --target spaceship_controlmain
```

## Running

```bash
./build/examples/spaceship_control/main
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
| `main.cpp` | Application entry point and game loop |
| `SpaceshipWidget.ixx` | ImGui widget for physics and fire rate parameter tuning |

### Components Used

| Component | Purpose |
|-----------|---------|
| `SceneNodeComponent` | Links GameObject to scene graph |
| `Move2DComponent` | 2D physics with velocity, acceleration, dampening |
| `SteeringComponent` | Rotation control and direction management |
| `ComposeTransformComponent` | Local/world transform composition |
| `ScaleStateComponent` | Unit-based sizing (meters) |
| `Active` / `Inactive` | Entity activation state tags |

### Systems Used

| System | Purpose |
|--------|---------|
| `TwinStickInputSystem` | Translates gamepad input to movement commands |
| `ScaleSystem` | Applies ScaleStateComponent sizing |
| `SteeringSystem` | Rotation and direction updates |
| `Move2DSystem` | Physics simulation (velocity integration) |
| `ComposeTransformSystem` | Computes final transforms |
| `StateToViewportPolicyUpdateSystem` | Activates viewports based on game state |
| `SceneSyncSystem` | Syncs transforms to scene graph |
| `SceneRenderingSystem` | Renders scenes to associated viewports |
| `TransformClearSystem` | Clears dirty flags post-frame |

## GameObjectFactory Builder

```cpp
auto shipGameObject = GameObjectFactory::instance()
    .gameObject(gameWorld)
    .withRendering([&](auto& rnb) {
        rnb.meshRenderable()
           .shader(shader)
           .color(Colors::Yellow)
           .primitiveType(PrimitiveType::LineLoop)
           .shape(std::make_shared<Triangle>())
           .attachTo(&root);
    })
    .withTransform([](auto& tb) {
        tb.transform()
          .scale(vec3f(SPACESHIP_SIZE, SPACESHIP_SIZE, 0.0f));
    })
    .withMotion([](auto& mcb) {
        mcb.move2D()
           .speed(30.0f)
           .instantAcceleration(false);
        mcb.steering()
           .steeringSetsDirection(true)
           .instantSteering(false);
    })
    .make();
```

## GameLoop Phase Configuration

```cpp
// Pre-Phase: Input and Physics
gameLoop.phase(PhaseType::Pre)
    .addPass<GameState>(GameState::Any)
    .addSystem<TwinStickInputSystem>(shipGameObject)
    .addCommitPoint(CommitPoint::Structural)
    .addPass<GameState>(GameState::Any)
    .addSystem<ScaleSystem>()
    .addSystem<SteeringSystem>()
    .addSystem<Move2DSystem>();

// Post-Phase: Transform and Rendering
gameLoop.phase(PhaseType::Post)
    .addPass<GameState>(GameState::Any)
    .addSystem<ComposeTransformSystem>()
    .addSystem<StateToViewportPolicyUpdateSystem<GameState, MatchState>>(stateToViewportMap)
    .addSystem<SceneSyncSystem>(sceneToViewportMap)
    .addSystem<SceneRenderingSystem>(renderingDevice, sceneToViewportMap)
    .addSystem<TransformClearSystem>();
```

## Camera System

The camera is attached to the spaceship in the scene graph and uses selective transform inheritance:

```cpp
// Create camera as child of spaceship
auto cameraNode = std::make_unique<CameraSceneNode>(std::move(camera));
shipSceneNode->addNode(std::move(cameraNode));

// Inherit only position, not rotation
cameraNode->setInheritance(TransformType::Translation);
cameraNode->lookAtLocal(vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f));
```

## See Also

- [Game Loop Architecture](../../docs/core-concepts/gameloop-architecture.md) - Phase-based game loop
- [State Management](../../docs/core-concepts/state-management.md) - State machine architecture
- [Spaceship Shooting Example](../spaceship_shooting/README.md) - Adds projectile spawning
- [Scoring Demo](../scoring_demo/README.md) - Full gameplay with scoring and menus
