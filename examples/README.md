# helios Examples

This directory contains example applications demonstrating various features of the helios game framework.

## Available Examples

### 1. Simple Cube Rendering

**Location:** `simple_cube_rendering/`

An example showing the fundamentals of 3D rendering with helios:
- Creating and configuring shaders
- Building materials and meshes
- Working with the scene graph
- Implementing a basic render loop

**Controls:**
- `ESC` - Exit application

---

### 2. Game Controller Input

**Location:** `game_controller_input/`

Demonstrates gamepad/controller input handling with helios.

---

### 3. Spaceship Control

**Location:** `spaceship_control/`

A complete game loop example featuring:
- GameLoop architecture with Pre/Main/Post phases
- GameObjectFactory fluent builder pattern
- Gamepad input with TwinStickInputSystem
- ImGui debug overlay with dockable widgets
- Real-time logging with scope filtering
- Frame pacing and performance metrics
- Scene graph camera with transform inheritance

**Controls:**
- Left Stick - Move/Rotate spaceship
- ESC - Exit application

---

### 4. Spaceship Shooting

**Location:** `spaceship_shooting/`

A twin-stick shooter example extending Spaceship Control with:
- Projectile spawning via ProjectileSpawnSystem
- Object pooling with GameObjectPoolManager
- ShootComponent with configurable fire rate
- Aim2DComponent for direction tracking
- Level bounds with bounce/despawn behavior

**Controls:**
- Left Stick - Move/Rotate spaceship
- Right Stick - Aim and fire projectiles
- ESC - Exit application

---

### 5. Enemy Spawn

**Location:** `enemy_spawn/`

Demonstrates the spawn system with:
- Timed spawning via SpawnScheduler and SpawnRule
- Object pooling with GameObjectPoolManager
- RandomSpawnPlacer for random positioning
- MoveInitializer for random movement
- SpinComponent for visual rotation
- Level bounds with reflect behavior

**Controls:**
- Left Stick - Move/Rotate spaceship
- ESC - Exit application

---

### 6. Collision Detection

**Location:** `collision_detection/`

A comprehensive collision detection example featuring:
- Grid-based spatial partitioning with GridCollisionDetectionSystem
- Collision layers and masks for filtering
- TriggerCollisionEvent and SolidCollisionEvent
- Multiple collision behaviors (Bounce, Reflect, Despawn)
- Enemy spawning and projectile systems combined

**Controls:**
- Left Stick - Move/Rotate spaceship
- Right Stick - Aim and fire projectiles
- ESC - Exit application

---

### 7. Text Rendering Demo

**Location:** `render_text_demo/`

Demonstrates FreeType-based text rendering with:
- Font loading with TextRenderer::addFontFamily()
- TextRenderPrototype for shared shader configuration
- TextRenderable for dynamic text display
- Orthographic projection for screen-space positioning
- Real-time text updates each frame

**Controls:**
- ESC - Exit application

---

## Building the Examples

All examples are automatically built when you build the helios project.

### Configure and Build

From the project root directory:

```bash
# Configure with CMake
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Build all targets (including examples)
cmake --build build --config Release
```

### Running Examples

Example executables are placed in:
```
build/examples/<example_name>/Release/
```

For example, to run the simple cube renderer:

```bash
# Windows
cd build/examples/simple_cube_rendering/Release
./main.exe

# Linux/macOS
cd build/examples/simple_cube_rendering/Release
./simple_cube_rendering
```

**Note:** Resources (shaders, textures, etc.) are automatically copied to the output directory during build.

## Creating Your Own Example

To add a new example:

1. Create a new directory under `examples/`:
   ```
   examples/my_example/
   ```

2. Add your `main.cpp`:
   ```cpp
   #include <cstdlib>
   
   import helios.ext.glfw.app.GLFWFactory;
   
   int main() {
       const auto app = GLFWFactory::makeOpenGLApp("My Example");
       // Your code here...
       return EXIT_SUCCESS;
   }
   ```

3. (Optional) Add a `resources/` directory for shaders, textures, etc.:
   ```
   examples/my_example/resources/
   ```

4. The CMake build system will automatically:
   - Detect your new example
   - Create an executable target
   - Copy resources to the build directory

5. Rebuild the project:
   ```bash
   cmake --build build --config Release
   ```

## Example Structure

Each example should follow this structure:

```
example_name/
├── main.cpp              # Main application code
├── resources/            # Optional: shaders, textures, models, etc.
│   ├── shader.vert
│   ├── shader.frag
│   └── ...
└── README.md            # Optional: Tutorial/documentation
```

## Common Patterns

### Application Setup

```cpp
const auto app = GLFWFactory::makeOpenGLApp("Window Title");
auto* win = dynamic_cast<GLFWWindow*>(app->current());
```

### Main Loop

```cpp
while (!win->shouldClose()) {
    app->eventManager().dispatchAll();
    inputManager.poll(0.0f);
    
    // Update logic here...
    
    const auto& snapshot = scene->createSnapshot(*camera);
    auto renderPass = RenderPassFactory::getInstance().buildRenderPass(snapshot);
    app->renderingDevice().render(renderPass);
    
    win->swapBuffers();
}
```

### Input Handling

```cpp
helios::input::InputManager& inputManager = app->inputManager();

if (inputManager.isKeyPressed(Key::SPACE)) {
    // Handle space key press
}
```

## Dependencies

Examples depend on:
- **helios Core** - Main framework
- **GLFW** - Windowing and input (via `helios.ext.glfw`)
- **OpenGL** - Rendering backend (via `helios.ext.opengl`)

## Troubleshooting

### "Module not found" errors

Ensure you've built the entire helios project before building examples:
```bash
cmake --build build --config Release --target helios
```

### Shader/Resource loading errors

Check that:
- Resources are in the `resources/` subdirectory
- Paths in code are relative (`./resources/shader.vert`)
- CMake successfully copied resources to the build directory

### Black screen / No output

Common causes:
- Shaders failed to compile (check console output)
- Camera or objects positioned incorrectly
- Rendering backend not properly initialized

## Learning Path

We recommend exploring examples in this order:

1. **simple_cube_rendering** - Core rendering concepts
2. **game_controller_input** - Input handling
3. **spaceship_control** - Game loop, command pattern, ImGui overlay
4. **spaceship_shooting** - Component system, projectile pooling, collision

## Further Resources

- [helios API Documentation](../docs/heliosapi.md)
- [helios Coding Style Guide](../docs/styleguide.md)
- [Doxygen Documentation](../docs/api/)
- [Main README](../README.md)

## Contributing

Found a bug in an example? Have an idea for a new example? Please open an issue or submit a pull request!

## License

All examples are part of the helios framework and are distributed under the MIT License.

