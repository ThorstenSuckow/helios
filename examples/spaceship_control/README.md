# Spaceship Control Example

This example demonstrates a complete game loop with input handling, logging, and ImGui debug overlay integration.

## Features

- **Gamepad Input** - Control a spaceship using analog sticks
- **Command Pattern** - Input mapped to reusable command objects
- **ImGui Debug Overlay** - Real-time logging, camera control, and settings via dockable widgets
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

| Input | Action                  |
|-------|-------------------------|
| Left Stick  | Move spaceship / Rotate |
| ESC         | Exit application        |


## Code Structure

| File | Purpose |
|------|---------|
| `main.cpp` | Application entry point and game loop |
| `Spaceship.ixx` | Spaceship entity with transform and physics |
| `TheGrid.ixx` | Grid game object for the play area |
| `InputHandler.ixx` | Maps input to commands |
| `commands/` | Command implementations (MoveCommand, etc.) |

## Camera System

The camera is attached to the spaceship in the scene graph and uses selective transform inheritance:

```cpp
// Create camera as child of spaceship
auto cameraNode = std::make_unique<CameraSceneNode>(std::move(camera));
auto* camPtr = spaceshipNode->addNode(std::move(cameraNode));

// Inherit only position, not rotation
camPtr->setInheritance(InheritTransform::Inherit::Translation);
camPtr->lookAtLocal(vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f));
```

## ImGui Integration

The example includes a complete ImGui setup with multiple widgets:

```cpp
// Backend and overlay
auto imguiBackend = ImGuiGlfwOpenGLBackend(window->nativeHandle());
auto imguiOverlay = ImGuiOverlay::forBackend(&imguiBackend);

// Add widgets
imguiOverlay.addWidget(new MainMenuWidget());      // Settings menu
imguiOverlay.addWidget(new FpsWidget(&metrics));   // FPS display
imguiOverlay.addWidget(new GamepadWidget(&input)); // Gamepad state
imguiOverlay.addWidget(new LogWidget());           // Log console
imguiOverlay.addWidget(new CameraWidget());        // Camera control
```

## Game System

The example uses helios's game framework:

```cpp
// Create game world and objects
auto gameWorld = GameWorld{};
auto* spaceship = gameWorld.addGameObject(
    std::make_unique<Spaceship>(spaceshipNode)
);

// Set size in meters
spaceship->setSize(5.0f, 5.0f, 0.0f, Unit::Meter);

// Game loop with command pattern
auto commandBuffer = CommandBuffer{};
inputHandler.handleInput(inputSnapshot, spaceship->guid(), commandBuffer);
commandBuffer.flush(gameWorld);
gameWorld.update(deltaTime);
```

## Logging

The spaceship logs its state updates:

```cpp
auto& logger = LogManager::loggerForScope("helios::examples::spaceshipControl::Spaceship");
logger.debug(std::format("Updating at {}", deltaTime));
```

Use the Log Console's scope filter to isolate messages from specific modules.

