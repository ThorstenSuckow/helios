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
├── rendering          # Rendering pipeline and render commands
│   ├── model         # Mesh, Material, and configuration
│   ├── shader        # Shader programs and uniform management
│   └── asset         # Geometric shapes and assets
├── scene             # Scene graph, camera, and culling
├── input             # Keyboard, mouse, and gamepad input
├── window            # Window management and events
├── math              # Vectors, matrices, transforms
├── util              # Logging, file I/O, utilities
└── app               # Application lifecycle and event management
```

### Extension Modules

```
helios.ext
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

The scene graph organizes objects hierarchically, with automatic transform propagation from parent to child nodes.

**Key Classes:**
- `Scene` - Root container for all scene objects
- `SceneNode` - Node in the hierarchy with transform and optional renderable
- `Camera` - Defines the viewpoint and projection

**Example:**
```cpp
import helios.scene.Scene;
import helios.scene.SceneNode;

auto scene = std::make_unique<Scene>(std::move(cullingStrategy));

auto cubeNode = std::make_unique<SceneNode>(std::move(cubeRenderable));
auto* node = scene->addNode(std::move(cubeNode));

node->translate(vec3f(0.0f, 2.0f, 0.0f));
node->rotate(rotationMatrix);
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

auto& inputManager = app->inputManager();
inputManager.poll(deltaTime);

if (inputManager.isKeyPressed(Key::SPACE)) {
    // Handle space key
}

const auto& gamepadState = inputManager.gamepadState(Gamepad::Gamepad1);
if (gamepadState.buttons.A) {
    // Handle A button
}
```

## Quick Start

### Creating a Simple Application

```cpp
import helios.ext.glfw.app.GLFWFactory;
import helios.scene.Scene;
import helios.scene.Camera;

int main() {
    // 1. Create application
    auto app = GLFWFactory::makeOpenGLApp("My helios App");
    auto* window = app->current();
    
    // 2. Create scene
    auto scene = std::make_unique<Scene>(
        std::make_unique<CullNoneStrategy>()
    );
    
    // 3. Add camera
    auto camera = std::make_unique<Camera>();
    scene->addNode(std::move(camera));
    
    // 4. Create and add renderable
    auto prototype = std::make_shared<RenderPrototype>(material, mesh);
    auto renderable = std::make_shared<Renderable>(prototype);
    auto node = std::make_unique<SceneNode>(std::move(renderable));
    scene->addNode(std::move(node));
    
    // 5. Main loop
    while (!window->shouldClose()) {
        app->eventManager().dispatchAll();
        
        auto snapshot = scene->createSnapshot(*camera);
        auto renderPass = RenderPassFactory::getInstance()
            .buildRenderPass(snapshot);
        
        app->renderingDevice().render(renderPass);
        window->swapBuffers();
    }
    
    return 0;
}
```

## Module Details

For detailed information about each module, see:

- **[Rendering](../include/helios/rendering/README.md)** - Rendering system and pipeline
- **[Scene](../include/helios/scene/README.md)** - Scene graph and camera
- **[Input](../include/helios/input/README.md)** - Input handling
- **[Math](../include/helios/math/README.md)** - Mathematical operations
- **[Window](../include/helios/window/README.md)** - Window management
- **[Utilities](../include/helios/util/README.md)** - Logging, file I/O

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

For detailed class-by-class documentation, see the [Doxygen-generated API reference](./api/html/index.html).

## Examples

Check the [examples/](../examples/) directory for complete working examples:

- **[Simple Cube Rendering](../examples/simple_cube_rendering/README.md)** - Basic rendering tutorial
- **Game Controller Input** - Input handling demo

## License

`helios` is distributed under the MIT-license.