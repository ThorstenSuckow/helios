# Spaceship Control Example

This example demonstrates a complete game loop using the component-based game system, input handling, logging, and ImGui debug overlay integration.

## Features

- **Component System** - GameObject with attachable components (Move2DComponent, SceneNodeComponent)
- **Gamepad Input** - Control a spaceship using analog sticks via TwinStickInputComponent
- **Command Pattern** - Input mapped to reusable command objects (Move2DCommand, Aim2DCommand)
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

The spaceship behavior is implemented via engine components:

| Component | Purpose |
|-----------|---------|
| `SceneNodeComponent` | Links GameObject to scene graph |
| `Move2DComponent` | 2D physics with rotation and dampening |
| `TwinStickInputComponent` | Translates gamepad input to commands |

## Camera System

The camera is attached to the spaceship in the scene graph and uses selective transform inheritance:

```cpp
// Create camera as child of spaceship
auto cameraNode = std::make_unique<CameraSceneNode>(std::move(camera));
auto* camPtr = spaceshipNode->addNode(std::move(cameraNode));

// Inherit only position, not rotation
camPtr->setInheritance(helios::math::TransformType::Translation);
camPtr->lookAtLocal(vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f));
```

## ImGui Integration

The example includes a complete ImGui setup with multiple widgets:

```cpp
// Backend and overlay
auto imguiBackend = ImGuiGlfwOpenGLBackend(window->nativeHandle());
auto imguiOverlay = ImGuiOverlay::forBackend(&imguiBackend);

// Add widgets
imguiOverlay.addWidget(new MainMenuWidget());       // Settings menu
imguiOverlay.addWidget(new FpsWidget(&metrics));    // FPS display
imguiOverlay.addWidget(new GamepadWidget(&input));  // Gamepad state
imguiOverlay.addWidget(new LogWidget());            // Log console
imguiOverlay.addWidget(new CameraWidget());         // Camera control
imguiOverlay.addWidget(new SpaceshipWidget());      // Physics tuning
```

## Component-Based Game System

The example uses helios's component-based game framework:

```cpp
// Create game world and command buffer
auto gameWorld = GameWorld{};
auto commandBuffer = CommandBuffer{};
auto updateContext = UpdateContext{&commandBuffer, &gameWorld};

// Create spaceship as GameObject with components
auto shipGameObject = std::make_unique<GameObject>();
shipGameObject->add<SceneNodeComponent>(spaceshipSceneNode);
shipGameObject->add<Move2DComponent>();
shipGameObject->add<TwinStickInputComponent>();

auto* ship = gameWorld.addGameObject(std::move(shipGameObject));

// Set size in meters
ship->get<SceneNodeComponent>()->setSize(5.0f, 5.0f, 0.0f, Unit::Meter);

// Game loop
updateContext.setDeltaTime(deltaTime);
updateContext.setInputSnapshot(&inputSnapshot);
gameWorld.update(updateContext);
commandBuffer.flush(gameWorld);
```

## Physics Tuning

Use the Spaceship Physics widget to adjust movement parameters at runtime:

- **Movement**: Max speed, acceleration, dampening
- **Rotation**: Max rotation speed, dampening
- **Reset**: Restore default physics values

