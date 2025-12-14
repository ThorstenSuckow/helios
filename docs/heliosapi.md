# helios - API Reference

## Introduction

`helios` is an ongoing, educational project and strives for providing a foundational and modular technical backbone for game development. Designed with a top-down, prototype-oriented approach, it emphasizes a clear separation of concerns and robust architectural patterns to help with creating maintainable games.

## Core Purpose

The primary objective of `helios` is to offer a flexible and extensible platform for interactive games. As a prototype, it provides the following systems:

- **Rendering Pipeline:** A rendering layer with customizable render passes.
- **Modular Subsystems:** Well-defined interfaces for core components such as Scene Management and Rendering.
- **Modern C++ Practices:** Leveraging C++(23) Modules for improved dependency management and compile-time efficiency.

## Architecture Overview

The framework's architecture follows a component-based philosophy, providing abstract interfaces that decouple high-level scene descriptions from underlying hardware implementations.

### Core Modules

```
helios
├── core              # Units and fundamental constants
├── rendering         # Rendering pipeline and render commands
│   ├── model         # Mesh, Material, and configuration
│   ├── shader        # Shader programs and uniform management
│   └── asset         # Geometric shapes and assets
├── scene             # Scene graph, camera, and culling
├── input             # Keyboard, mouse, and gamepad input
├── window            # Window management and events
├── engine            # Frame pacing and timing control
│   └── game          # Game objects and command pattern
├── math              # Vectors, matrices, transforms
├── util              # Logging, file I/O, utilities
├── tooling           # Performance metrics and profiling
└── app               # Application lifecycle and event management
```

### Extension Modules

```
helios.ext
├── imgui             # ImGui integration for debug overlays
│   ├── widgets       # Ready-to-use debug widgets
│   │   ├── LogWidget         # Log console with filtering
│   │   ├── GamepadWidget     # Gamepad state visualizer
│   │   ├── CameraWidget      # Camera parameter control
│   │   ├── FpsWidget         # Frame rate display
│   │   └── MainMenuWidget    # Application settings menu
│   ├── ImGuiOverlay          # Widget manager with docking support
│   ├── ImGuiLogSink          # Log sink for ImGui output
│   └── backends              # Platform-specific backends
├── glfw              # GLFW-based window and input implementation
│   ├── window
│   ├── input
│   └── app
└── opengl            # OpenGL rendering backend
    ├── rendering
    └── shader
```

## Core Concepts

### 1. Scene Graph

The scene graph organizes objects hierarchically, with automatic transform propagation from parent to child nodes. Transform inheritance can be selectively controlled per node.

**Key Classes:**
- `Scene` - Root container for all scene objects
- `SceneNode` - Node in the hierarchy with transform and optional renderable
- `CameraSceneNode` - Specialized node for cameras with view matrix computation
- `Camera` - Defines projection parameters (FOV, aspect ratio, near/far planes)
- `TransformType` - Bitmask for selective transform inheritance (Translation, Rotation, Scale)

**Example:**
```cpp
import helios.scene.Scene;
import helios.scene.SceneNode;
import helios.scene.CameraSceneNode;
import helios.math.transform;

auto scene = std::make_unique<Scene>(std::move(cullingStrategy));

// Add a game object
auto cubeNode = std::make_unique<SceneNode>(std::move(cubeRenderable));
auto* node = scene->addNode(std::move(cubeNode));
node->setTranslation(vec3f(0.0f, 2.0f, 0.0f));

// Add a camera as child of the cube (follows cube position)
auto camera = std::make_unique<Camera>();
auto cameraNode = std::make_unique<CameraSceneNode>(std::move(camera));
auto* camPtr = node->addNode(std::move(cameraNode));

// Camera inherits only translation (not rotation/scale)
camPtr->setInheritance(helios::math::TransformType::Translation);
camPtr->lookAt(vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f));
```

### 2. Rendering Pipeline

The rendering pipeline uses a deferred command pattern with render passes.

**Flow:**
```
Scene → Snapshot → RenderPass → RenderQueue → RenderCommand → GPU
```

**Key Classes:**
- `RenderPrototype` - Immutable combination of Mesh + Material
- `Renderable` - Instance of a RenderPrototype with optional overrides
- `RenderCommand` - Low-level GPU command
- `RenderQueue` - Collection of commands for a pass
- `RenderPass` - Execution unit with frame-specific uniforms

**Example:**
```cpp
import helios.rendering.RenderPrototype;
import helios.rendering.Renderable;

// Create prototype (shared)
auto prototype = std::make_shared<RenderPrototype>(material, mesh);

// Create instance
auto renderable = std::make_shared<Renderable>(prototype);

// Optional per-instance material override
renderable->materialOverride() = MaterialPropertiesOverride{
    .baseColor = vec4f(1.0f, 0.0f, 0.0f, 1.0f)
};
```

### 3. Material System

Materials define surface appearance with shaders and properties.

**Key Classes:**
- `Material` - Shader + default MaterialProperties
- `MaterialProperties` - Immutable value object (baseColor, roughness, etc.)
- `MaterialPropertiesOverride` - Per-instance overrides

**Example:**
```cpp
import helios.rendering.model.Material;
import helios.rendering.model.config.MaterialProperties;

auto materialProps = MaterialProperties(
    vec4f(1.0f, 0.0f, 1.0f, 1.0f), // Base color (magenta)
    0.5f                             // Roughness
);

auto material = std::make_shared<Material>(shader, 
    std::make_shared<MaterialProperties>(materialProps));
```

### 4. Input System

Unified input handling for keyboard, mouse, and gamepad.

**Key Classes:**
- `InputManager` - Central input coordinator
- `InputAdapter` - Platform-specific input implementation
- `GamepadState` - Current state of a gamepad

**Example:**
```cpp
import helios.input.InputManager;
import helios.input.types.Key;
import helios.input.types.Gamepad;

auto& inputManager = app->inputManager();
inputManager.poll(deltaTime);

if (inputManager.isKeyPressed(Key::SPACE)) {
    // Handle space key
}

const auto& state = inputManager.gamepadState(Gamepad::ONE);
if (state.buttonA()) {
    // Handle A button
}

// Analog sticks
auto leftStick = state.left();   // vec2f
auto rightStick = state.right(); // vec2f
```

### 5. Logging System

Flexible logging with configurable output destinations (sinks).

**Key Classes:**
- `Logger` - Scoped logger for categorized output
- `LogManager` - Central manager for loggers and sinks
- `LogSink` - Abstract interface for output destinations
- `ConsoleSink` - Outputs to stdout
- `ImGuiLogSink` - Outputs to ImGui LogWidget

**Example:**
```cpp
import helios.util.log;

// Get a scoped logger
auto& logger = LogManager::loggerForScope("MyModule");
logger.info("Application started");
logger.debug("Processing frame");
logger.warn("Resource not found");
logger.error("Critical failure");

// Configure sinks
auto& mgr = LogManager::getInstance();
mgr.registerSink(std::make_shared<ConsoleSink>());

// Enable/disable sinks by type identifier
mgr.enableSink("console");
mgr.disableSink("imgui");
```

### 6. ImGui Debug Overlay

Integrated debug UI with docking support and persistent settings.

**Key Classes:**
- `ImGuiOverlay` - Widget manager with DockSpace support
- `LogWidget` - Scrollable log console with scope filtering
- `GamepadWidget` - Real-time gamepad state visualizer
- `MainMenuWidget` - Application settings (transparency, docking, style)

**Example:**
```cpp
import helios.ext.imgui.ImGuiOverlay;
import helios.ext.imgui.ImGuiGlfwOpenGLBackend;
import helios.ext.imgui.widgets.LogWidget;
import helios.ext.imgui.widgets.MainMenuWidget;
import helios.ext.imgui.ImGuiLogSink;

// Setup backend and overlay
auto backend = std::make_unique<ImGuiGlfwOpenGLBackend>(window);
auto& overlay = ImGuiOverlay::forBackend(backend.get());

// Add main menu with settings persistence (saves to imgui.ini)
auto menu = std::make_shared<MainMenuWidget>();
menu->setDockingCallback([&overlay](bool enabled) {
    overlay.setDockSpaceEnabled(enabled);
});
overlay.addWidget(menu.get());

// Add log console with ImGui sink
auto logWidget = std::make_shared<LogWidget>();
overlay.addWidget(logWidget.get());

auto imguiSink = std::make_shared<ImGuiLogSink>(logWidget.get());
LogManager::getInstance().registerSink(imguiSink);

// In render loop
overlay.render();
```

**LogWidget Features:**
- Per-scope message buffers (1000 entries each)
- Filter by log level (Debug, Info, Warn, Error)
- Filter by scope via dropdown
- Text search filter
- Auto-scroll with pause on manual scroll
- Dockable to window edges

**MainMenuWidget Features:**
- Window transparency control (slider + presets)
- Docking toggle
- Style themes (Dark, Light, Classic)
- Style Editor access
- Settings persistence via imgui.ini

### 8. Game System

A lightweight framework for game object management and input handling.

**Key Classes:**
- `GameObject` - Base class for game entities with GUID identification
- `GameWorld` - Container for game objects with update loop
- `CommandBuffer` - Deferred command execution pattern
- `InputSnapshot` - Captures input state for a single frame
- `InputHandler` - Interface for translating input to commands

**Example:**
```cpp
import helios.engine.game.GameWorld;
import helios.engine.game.GameObject;
import helios.engine.game.CommandBuffer;
import helios.engine.game.InputSnapshot;

// Create game world
auto gameWorld = GameWorld{};

// Add game objects
auto spaceship = std::make_unique<Spaceship>(sceneNode);
auto* spaceshipPtr = gameWorld.addGameObject(std::move(spaceship));

// Set size using engine units (1 hu = 1 meter)
spaceshipPtr->setSize(5.0f, 5.0f, 0.0f, helios::core::units::Unit::Meter);

// Game loop
auto commandBuffer = CommandBuffer{};
while (running) {
    // Capture input
    auto snapshot = InputSnapshot(gamepadState);
    
    // Translate input to commands
    inputHandler.handleInput(snapshot, spaceshipPtr->guid(), commandBuffer);
    
    // Execute commands
    commandBuffer.flush(gameWorld);
    
    // Update all game objects
    gameWorld.update(deltaTime);
}
```

### 9. Units System

Standardized measurement units for consistent object sizing across the engine.

**Key Constants:**
- Standard unit: **Meter** (1 hu = 1 m)
- Time standard: **Seconds**

**Example:**
```cpp
import helios.core.units;

using namespace helios::core::units;

// Set object size in meters (1 hu = 1 meter)
gameObject->setSize(2.0f, 1.0f, 0.5f, Unit::Meter);

// A spaceship 5 meters wide
constexpr float SHIP_WIDTH = 5.0f;  // 5 hu = 5 meters
```

### 7. Frame Pacing and Performance Metrics

Control frame rate and monitor performance with built-in timing tools.

**Key Classes:**
- `FramePacer` - Maintains consistent frame rate through precise timing
- `FrameStats` - Timing statistics for a single frame
- `FpsMetrics` - Aggregates frame data for performance analysis

**Example:**
```cpp
import helios.engine.FramePacer;
import helios.tooling.FpsMetrics;

// Setup frame pacing
FramePacer pacer;
pacer.setTargetFps(60.0f);

// Setup metrics tracking
FpsMetrics metrics;
metrics.setHistorySize(120); // 2 seconds at 60 FPS

// Main loop
while (!window->shouldClose()) {
    pacer.beginFrame();
    
    // Game logic and rendering
    // ...
    
    auto stats = pacer.sync();
    metrics.addFrame(stats);
    
    // Display metrics
    float currentFps = metrics.getFps();
    float avgFrameTime = metrics.getFrameTimeMs();
}
```

**Frame Statistics:**
- `totalFrameTime` - Complete frame duration (work + wait)
- `workTime` - Actual CPU processing time
- `waitTime` - Time spent sleeping to maintain target FPS

**Performance Monitoring:**
```cpp
// Get detailed metrics
float fps = metrics.getFps();              // Average FPS
float frameTime = metrics.getFrameTimeMs(); // Average frame time
float workTime = metrics.getWorkTimeMs();   // Last frame work time
float idleTime = metrics.getIdleTimeMs();   // Last frame idle time

// Access history for graphs
const auto& history = metrics.getHistory();
```

## Quick Start

### Creating a Simple Application

```cpp
import helios.ext.glfw.app.GLFWFactory;
import helios.scene.Scene;
import helios.scene.CameraSceneNode;
import helios.rendering.Viewport;

int main() {
    // 1. Create application
    auto app = GLFWFactory::makeOpenGLApp("My helios App", 1920, 1080);
    auto* window = app->current();
    
    // 2. Setup viewport
    auto viewport = std::make_shared<Viewport>(0.0f, 0.0f, 1.0f, 1.0f);
    window->addViewport(viewport);
    
    // 3. Create scene
    auto scene = std::make_unique<Scene>(
        std::make_unique<CullNoneStrategy>()
    );
    
    // 4. Add camera to scene graph
    auto camera = std::make_unique<Camera>();
    camera->setPerspective(radians(90.0f), 16.0f/9.0f, 0.1f, 1000.0f);
    
    auto cameraNode = std::make_unique<CameraSceneNode>(std::move(camera));
    auto* camPtr = scene->addNode(std::move(cameraNode));
    camPtr->setTranslation(vec3f(0.0f, 0.0f, 5.0f));
    camPtr->lookAt(vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f));
    
    // Connect viewport to camera
    viewport->setCameraSceneNode(camPtr);
    
    // 5. Create and add renderable
    auto prototype = std::make_shared<RenderPrototype>(material, mesh);
    auto renderable = std::make_shared<Renderable>(prototype);
    auto node = std::make_unique<SceneNode>(std::move(renderable));
    scene->addNode(std::move(node));
    
    // 6. Main loop
    while (!window->shouldClose()) {
        app->eventManager().dispatchAll();
        
        auto snapshot = scene->createSnapshot(viewport);
        if (snapshot.has_value()) {
            auto renderPass = RenderPassFactory::getInstance()
                .buildRenderPass(*snapshot);
            app->renderingDevice().render(renderPass);
        }
        
        window->swapBuffers();
    }
    
    return 0;
}
```

## Module Details

For detailed information about each module, see:

- **[Core](../include/helios/core/README.md)** - Units and fundamental constants
- **[Rendering](../include/helios/rendering/README.md)** - Rendering system and pipeline
- **[Scene](../include/helios/scene/README.md)** - Scene graph and camera
- **[Game](../include/helios/engine/game/README.md)** - Game objects and command pattern
- **[Input](../include/helios/input/README.md)** - Input handling
- **[Engine](../include/helios/engine/README.md)** - Frame pacing and timing control
- **[Tooling](../include/helios/tooling/README.md)** - Performance metrics and profiling
- **[Math](../include/helios/math/README.md)** - Mathematical operations
- **[Window](../include/helios/window/README.md)** - Window management
- **[Utilities](../include/helios/util/README.md)** - Logging, file I/O
- **[ImGui Extension](../include/ext/imgui/README.md)** - Debug overlay and widgets

## Advanced Topics

### Custom Rendering Backend

To implement a custom rendering backend:

1. Implement `RenderingDevice` interface
2. Extend `Mesh` for your platform (e.g., `VulkanMesh`)
3. Extend `Shader` for your platform (e.g., `VulkanShader`)
4. Create a factory (e.g., `VulkanFactory`)

### Custom Input Adapter

To implement a custom input adapter:

1. Implement `InputAdapter` interface
2. Map platform-specific events to helios types
3. Update `GamepadState` on polling

## API Documentation

For detailed class-by-class documentation, see the [Doxygen-generated API reference](https://helios.garagecraft.games/docs/helios/).

## Examples

Check the [examples/](../examples/) directory for complete working examples:

- **[Simple Cube Rendering](../examples/simple_cube_rendering/README.md)** - Basic rendering tutorial
- **[Game Controller Input](../examples/game_controller_input/README.md)** - Gamepad input handling
- **[Spaceship Control](../examples/spaceship_control/README.md)** - Complete game loop with input, logging, and ImGui overlay

## License

`helios` is distributed under the MIT-license.