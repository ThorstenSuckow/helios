# Spaceship Control Example

This example demonstrates the complete GameLoop architecture with phase-based execution, the GameObjectFactory builder pattern, component-based game systems, and ImGui debug overlay integration.

## Features

- **GameLoop Architecture** - Phase-based game loop (Pre/Main/Post) with commit points
- **GameObjectFactory** - Fluent builder pattern for creating GameObjects with components
- **Component System** - Composition-based entity design with Move2DComponent, SteeringComponent
- **Gamepad Input** - Control a spaceship using analog sticks via TwinStickInputSystem
- **ImGui Debug Overlay** - Real-time logging, camera control, physics tuning via dockable widgets
- **Frame Pacing** - Configurable target FPS with performance metrics
- **Scene Graph Camera** - Camera follows spaceship using transform inheritance
- **Units System** - Object sizes defined in meters (1 hu = 1 m)

## Building

```bash
cmake -S . -B build
cmake --build build --target spaceship_control
```

## Running

```bash
./build/bin/spaceship_control
```

## Controls

| Input | Action |
|-------|--------|
| Left Stick | Move spaceship / Rotate |
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
| `Move2DComponent` | 2D physics with velocity, acceleration, dampening |
| `SteeringComponent` | Rotation control and direction management |
| `DirectionComponent` | Movement direction vector |
| `ComposeTransformComponent` | Local/world transform composition |
| `TranslationStateComponent` | Position state tracking |
| `RotationStateComponent` | Rotation state tracking |
| `ScaleStateComponent` | Unit-based sizing (meters) |

### Systems Used

| System | Purpose |
|--------|---------|
| `TwinStickInputSystem` | Translates gamepad input to movement commands |
| `ScaleSystem` | Applies ScaleStateComponent sizing |
| `SteeringSystem` | Rotation and direction updates |
| `Move2DSystem` | Physics simulation (velocity integration) |
| `ComposeTransformSystem` | Computes final transforms |
| `SceneSyncSystem` | Syncs transforms to scene graph |
| `TransformClearSystem` | Clears dirty flags post-frame |

## GameObjectFactory Builder

The example uses the fluent builder pattern for GameObject creation:

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
    .addPass()
    .addSystem<TwinStickInputSystem>(shipGameObject)
    .addCommitPoint(CommitPoint::Structural)
    .addPass()
    .addSystem<ScaleSystem>()
    .addSystem<SteeringSystem>()
    .addSystem<Move2DSystem>();

// Post-Phase: Transform and Rendering
gameLoop.phase(PhaseType::Post)
    .addPass()
    .addSystem<ComposeTransformSystem>()
    .addSystem<SceneSyncSystem>(scene.get())
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

## ImGui Integration

The example includes a complete ImGui setup with multiple widgets:

```cpp
auto imguiBackend = ImGuiGlfwOpenGLBackend(window->nativeHandle());
auto imguiOverlay = ImGuiOverlay::forBackend(&imguiBackend);

imguiOverlay.addWidget(new MainMenuWidget());
imguiOverlay.addWidget(new FpsWidget(&metrics, &framePacer));
imguiOverlay.addWidget(new GamepadWidget(&inputManager));
imguiOverlay.addWidget(new LogWidget());
imguiOverlay.addWidget(new CameraWidget());
imguiOverlay.addWidget(new SpaceshipWidget());
```

## Main Game Loop

```cpp
while (!win->shouldClose()) {
    framePacer.beginFrame();
    
    // Process events and input
    app->eventManager().dispatchAll();
    inputManager.poll(0.0f);
    
    // Update game logic via GameLoop
    const auto inputSnapshot = InputSnapshot(gamepadState);
    gameLoop.update(gameWorld, deltaTime, inputSnapshot);
    
    // Render scene
    const auto& snapshot = scene->createSnapshot(*mainViewport);
    if (snapshot.has_value()) {
        auto renderPass = RenderPassFactory::getInstance().buildRenderPass(*snapshot);
        app->renderingDevice().render(renderPass);
    }
    
    // Render ImGui
    imguiOverlay.render();
    
    // Swap and sync
    win->swapBuffers();
    frameStats = framePacer.sync();
}
```
