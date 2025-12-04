# Spaceship Control Example

This example demonstrates a complete game loop with input handling, logging, and ImGui debug overlay integration.

## Features

- **Gamepad and Keyboard Input** - Control a spaceship using either input method
- **Command Pattern** - Input mapped to reusable command objects
- **ImGui Debug Overlay** - Real-time logging and settings via dockable widgets
- **Frame Pacing** - Consistent 60 FPS with performance metrics

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


## Code Structure

| File | Purpose |
|------|---------|
| `main.cpp` | Application entry point and game loop |
| `Spaceship.ixx` | Spaceship entity with transform and physics |
| `InputHandler.ixx` | Maps input to commands |
| `commands/` | Command implementations (MoveCommand, etc.) |

## ImGui Integration

The example includes a complete ImGui setup:

```cpp
// Backend and overlay
auto backend = std::make_unique<ImGuiGlfwOpenGLBackend>(window);
auto& overlay = ImGuiOverlay::forBackend(backend.get());

// Main menu with settings
auto menu = std::make_shared<MainMenuWidget>();
overlay.addWidget(menu.get());

// Log console with scope filtering
auto logWidget = std::make_shared<LogWidget>();
overlay.addWidget(logWidget.get());

// Connect logging to widget
auto sink = std::make_shared<ImGuiLogSink>(logWidget.get());
LogManager::getInstance().registerSink(sink);
```

## Logging

The spaceship logs its state updates:

```cpp
auto& logger = LogManager::loggerForScope("helios::examples::spaceshipControl::Spaceship");
logger.debug(std::format("Updating at {}", deltaTime));
```

Use the Log Console's scope filter to isolate messages from specific modules.

