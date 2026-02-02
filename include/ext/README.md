## helios::ext

This namespace contains **platform-specific implementations** and **backend adapters** for the helios framework. It bridges the abstract helios core APIs with concrete third-party libraries.

## Overview

The `ext` namespace follows a **plugin architecture**, allowing helios to support multiple windowing systems and rendering backends without coupling the core framework to specific implementations.

### Available Extensions

```
ext/
├── glfw/          # GLFW-based windowing and input
│   ├── window/   # GLFWWindow implementation
│   ├── input/    # GLFWInputAdapter implementation
│   └── app/      # GLFWApplication and factory
└── opengl/        # OpenGL rendering backend
    ├── rendering/ # OpenGL RenderingDevice
    ├── shader/    # OpenGLShader implementation
    └── model/     # OpenGLMesh implementation
```

## Architecture

### Extension Responsibilities

| Extension | Responsibility |
|-----------|----------------|
| **`glfw`** | Window creation, event handling, input polling (keyboard, mouse, gamepad) |
| **`opengl`** | GPU resource management, shader compilation, draw call submission |

### Separation of Concerns

- **helios core** defines the **interfaces** (e.g., `Window`, `RenderingDevice`, `Shader`)
- **`ext` modules** provide the **implementations** (e.g., `GLFWWindow`, `OpenGLShader`)
- Applications use **factories** to instantiate concrete implementations

## Using Extensions

### Quick Start with GLFW + OpenGL

```cpp
import helios.ext.glfw.app.GLFWFactory;

int main() {
    // Factory creates a complete GLFW+OpenGL application
    auto app = GLFWFactory::makeOpenGLApp("My App");
    
    auto* window = app->current();        // GLFWWindow
    auto& device = app->renderingDevice(); // OpenGLRenderingDevice
    auto& input = app->inputManager();     // GLFWInputAdapter
    
    while (!window->shouldClose()) {
        // Your game loop
    }
}
```

## Extension Modules

### `helios.ext.glfw`

GLFW-based implementation for windowing and input.

**Key Classes:**
- `GLFWWindow` - Window management (resize, close, swap buffers)
- `GLFWInputAdapter` - Input polling (keyboard, mouse, gamepad)
- `GLFWApplication` - Application lifecycle with event loop
- `GLFWFactory` - Factory for creating GLFW-based apps

**Dependencies:**
- [GLFW 3.x](https://www.glfw.org/)

**See:** [glfw/README.md](./glfw/README.md)

### `helios.ext.opengl`

OpenGL 4.5+ rendering backend.

**Key Classes:**
- `OpenGLRenderingDevice` - Main rendering interface
- `OpenGLShader` - Shader compilation and linking
- `OpenGLMesh` - VBO/IBO management
- `OpenGLUniformLocationMap` - Uniform binding

**Dependencies:**
- OpenGL 4.5+
- [GLAD](https://glad.dav1d.de/) (loader)

**See:** [opengl/README.md](./opengl/README.md)

## Creating a New Backend

To add support for a new platform or rendering API:

### 1. Implement Core Interfaces

Create implementations for:
- `helios::window::Window` (if new windowing system)
- `helios::rendering::RenderingDevice` (if new rendering API)
- `helios::rendering::Shader`
- `helios::rendering::mesh::Mesh`
- `helios::input::InputAdapter` (if new input system)

### 2. Create Factory

```cpp
namespace helios::ext::vulkan {
    class VulkanFactory {
    public:
        static std::unique_ptr<Application> makeVulkanApp(
            const std::string& title
        ) {
            auto window = std::make_unique<VulkanWindow>(...);
            auto device = std::make_unique<VulkanRenderingDevice>(...);
            auto input = std::make_unique<VulkanInputAdapter>(...);
            
            return std::make_unique<Application>(
                std::move(window),
                std::move(device),
                std::move(input)
            );
        }
    };
}
```

### 3. Follow Module Structure

```
include/ext/yourbackend/
├── _module.ixx         # Module facade
├── rendering/
│   ├── YourRenderingDevice.ixx
│   └── ...
├── shader/
│   └── YourShader.ixx
└── ...

src/ext/yourbackend/
├── rendering/
│   ├── YourRenderingDevice.cpp
│   └── ...
└── ...
```

### 4. Update CMake

Add your backend to the build system:
```cmake
# In src/ext/CMakeLists.txt
add_subdirectory(yourbackend)
```

## Design Principles

1. **Abstract Interfaces**: The core helios framework defines interfaces, not implementations.
2. **Dependency Injection**: Applications receive concrete implementations via factories.
3. **Modularity**: Each extension is an independent C++23 module.
4. **Interchangeability**: Backends can be swapped without changing application code (only the factory call).

## Examples

See [examples/](../../examples/) for complete applications using the extensions:
- `simple_cube_rendering` - GLFW + OpenGL
- `game_controller_input` - GLFW input handling

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::ext
@brief Platform-specific extensions and backend implementations.
@details This namespace contains concrete implementations of helios core interfaces for various platforms, including GLFW-based windowing/input and OpenGL rendering.
</p></details>

