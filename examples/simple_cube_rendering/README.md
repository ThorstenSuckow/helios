# Simple Cube Rendering - Tutorial

This example demonstrates how to create a simple rotating wireframe cube using the helios game framework. It covers the fundamental concepts of 3D rendering, including shaders, materials, meshes, scene graphs, and the render loop.

## What You'll Learn

- Setting up a Helios application with OpenGL backend
- Creating and configuring shaders
- Building materials and meshes
- Working with the scene graph
- Implementing a basic render loop
- Handling user input

## Prerequisites

- helios framework properly built and installed
- Basic understanding of C++ and 3D graphics concepts
- Familiarity with OpenGL shader language (GLSL)

## Project Structure

```
simple_cube_rendering/
├── main.cpp           # Main application code
├── resources/
│   ├── cube.vert     # Vertex shader
│   └── cube.frag     # Fragment shader
└── README.md         # This file
```

## Step-by-Step Tutorial

### 1. Application and Window Setup

First, we create an OpenGL application using the `GLFWFactory`:

```cpp
const auto app = GLFWFactory::makeOpenGLApp(
    "helios - Simple Cube Renderer"
);

auto win = dynamic_cast<GLFWWindow*>(app->current());
helios::input::InputManager& inputManager = app->inputManager();
```

**What's happening:**
- `GLFWFactory::makeOpenGLApp()` creates a complete application with a window and OpenGL context
- We retrieve the current window and input manager for later use

### 2. Shader Creation

Shaders define how our geometry is transformed and colored. We load vertex and fragment shaders from files:

```cpp
const auto basicStringFileReader = BasicStringFileReader();

auto shader_ptr = std::make_shared<OpenGLShader>(
    "./resources/cube.vert",
    "./resources/cube.frag",
    basicStringFileReader
);
```

**Vertex Shader (`cube.vert`):**
```glsl
#version 450 core

layout (location=0) in vec3 aPos;
layout (location=1) uniform mat4 modelMatrix;

void main() {
    gl_Position = modelMatrix * vec4(aPos, 1.0f);
}
```

**Fragment Shader (`cube.frag`):**
```glsl
#version 450 core

out vec4 FragColor;

void main() {
    FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow color
}
```

**Mapping Uniforms:**

We tell the shader where to find the world transformation matrix:

```cpp
auto uniformLocationMap = std::make_unique<OpenGLUniformLocationMap>();
uniformLocationMap->set(UniformSemantics::ModelMatrix, 1);
shader_ptr->setUniformLocationMap(std::move(uniformLocationMap));
```

### 3. Material Creation

Materials combine shaders with properties like color and roughness:

```cpp
auto cubeMaterialProps = MaterialProperties(
    helios::math::vec4f(1.0f, 0.0f, 1.0f, 1.0f), // Magenta RGBA
    0.0f  // Roughness (0 = smooth, 1 = rough)
);

auto cubeMaterialProps_shared = std::make_shared<MaterialProperties>(cubeMaterialProps);
auto material_ptr = std::make_shared<Material>(
    shader_ptr, cubeMaterialProps_shared
);
```

**Note:** The material color is currently not used by the fragment shader (which hardcodes yellow). To use the material color, modify the fragment shader to accept a uniform.

### 4. Mesh (Geometry) Creation

A mesh defines the 3D shape. We use helios's built-in `Cube` shape:

```cpp
auto cube = Cube{};

auto meshConfig = std::make_shared<const MeshConfig>(
    PrimitiveType::LineLoop  // Render as wireframe
);

auto mesh_ptr = std::make_shared<OpenGLMesh>(cube, meshConfig);
```

**Primitive Types:**
- `PrimitiveType::Triangles` - Solid filled geometry
- `PrimitiveType::LineLoop` - Wireframe outline
- `PrimitiveType::Lines` - Individual line segments
- `PrimitiveType::Points` - Vertex points

### 5. Creating a Renderable

A `RenderPrototype` combines a material with a mesh, and a `Renderable` makes it instantiable:

```cpp
const auto renderPrototype = std::make_shared<RenderPrototype>(
    material_ptr, mesh_ptr
);

auto cubeRenderable = std::make_shared<Renderable>(renderPrototype);
```

### 6. Scene Graph Setup

The scene graph organizes objects in 3D space. We create a scene and add our cube:

```cpp
auto frustumCullingStrategy = std::make_unique<CullNoneStrategy>();
auto scene = std::make_unique<Scene>(std::move(frustumCullingStrategy));

auto cubeSceneNode = std::make_unique<SceneNode>(std::move(cubeRenderable));
auto* cubeNode = scene->addNode(std::move(cubeSceneNode));

// Scale the cube to half its original size
cubeNode->setScale(vec3f(0.5f, 0.5f, 0.5f));
```

**Scene Graph Concepts:**
- **Scene**: The root container for all objects
- **SceneNode**: An object in the scene with a transform (position, rotation, scale)
- **Culling Strategy**: Determines which objects are visible (here: render everything)

### 7. Camera and Viewport Setup

The camera defines the viewpoint from which we observe the scene. In helios, cameras are integrated into the scene graph via `CameraSceneNode`:

```cpp
// Create viewport
auto mainViewport = std::make_shared<Viewport>(0.0f, 0.0f, 1.0f, 1.0f);
win->addViewport(mainViewport);

// Create camera with projection settings
auto camera = std::make_unique<Camera>();
camera->setPerspective(
    helios::math::radians(90.0f),  // FOV in radians
    16.0f / 9.0f,                   // Aspect ratio
    0.1f,                           // Near plane
    1000.0f                         // Far plane
);

// Add camera to scene graph
auto cameraNode = std::make_unique<CameraSceneNode>(std::move(camera));
auto* camPtr = scene->addNode(std::move(cameraNode));

// Position the camera
camPtr->setTranslation(vec3f(0.0f, 0.0f, 3.0f));
camPtr->lookAt(vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f));

// Connect viewport to camera
mainViewport->setCameraSceneNode(camPtr);
```

**Camera Concepts:**
- **Camera**: Defines projection parameters (FOV, aspect ratio, near/far planes)
- **CameraSceneNode**: Scene graph node that owns the camera and computes the view matrix
- **Viewport**: Defines the render target area and references the active camera

### 8. Main Render Loop

The render loop updates the scene and draws each frame:

```cpp
float degrees = 0.0f;
constexpr float rotationSpeed = 2.25f;

while (!win->shouldClose()) {
    // 1. Process events
    app->eventManager().dispatchAll();
    inputManager.poll(0.0f);

    // 2. Handle input (ESC to quit)
    if (inputManager.isKeyPressed(Key::ESC)) {
        win->setShouldClose(true);
    }

    // 3. Update rotation
    degrees += rotationSpeed;
    if (degrees >= 360.0f) {
        degrees -= 360.0f;
    }

    // 4. Apply transformation
    float rad = helios::math::radians(degrees);
    cubeNode->setRotation(helios::math::rotate(
        helios::math::mat4f::identity(),
        rad,
        helios::math::vec3f(0.4f, 0.6f, 0.2f) // Rotation axis
    ));

    // 5. Render the scene
    const auto& snapshot = scene->createSnapshot(mainViewport);
    if (snapshot.has_value()) {
        auto renderPass = RenderPassFactory::getInstance().buildRenderPass(*snapshot);
        app->renderingDevice().render(renderPass);
    }

    // 6. Swap buffers (display the result)
    win->swapBuffers();
}
```

**Render Loop Breakdown:**
1. **Event Processing**: Handle window events (resize, close, etc.)
2. **Input Handling**: Check for keyboard/mouse input
3. **Update Logic**: Modify object transformations (rotation, position, etc.)
4. **Rendering**: Create snapshot from viewport's camera and render it
5. **Buffer Swap**: Display the rendered frame

## Building and Running

From the project root directory:

```bash
# Configure the build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build build --config Release

# Run the example
cd build/examples/simple_cube_rendering/Release
./simple_cube_rendering.exe
```

## Expected Output

You should see a window displaying a rotating yellow wireframe cube. Press `ESC` to exit.

## Customization Ideas

### Change the Cube Color

Modify `cube.frag`:

```glsl
void main() {
    FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f); // Green cube
}
```

### Render a Solid Cube

Change the primitive type in `main.cpp`:

```cpp
auto meshConfig = std::make_shared<const MeshConfig>(
    PrimitiveType::Triangles  // Changed from LineLoop
);
```

### Adjust Rotation Speed

Modify the rotation speed constant:

```cpp
constexpr float rotationSpeed = 5.0f; // Faster rotation
```

### Change Rotation Axis

Modify the rotation axis vector:

```cpp
helios::math::vec3f(1.0f, 0.0f, 0.0f) // Rotate around X-axis
```

## Key helios Concepts

### Rendering Pipeline

1. **Scene Graph** → Hierarchical organization of objects
2. **Snapshot** → Frozen state of the scene at a moment in time
3. **RenderPass** → Collection of render commands for the GPU
4. **RenderQueue** → Ordered list of objects to render
5. **RenderCommand** → Individual draw call with uniforms

### Resource Management

helios uses `std::shared_ptr` for shared resources (shaders, materials, meshes) and `std::unique_ptr` for owned resources (scene nodes, render passes).

### Module System

helios uses C++23 modules (`import` statements) instead of traditional `#include` headers for better compile times and cleaner dependencies.

## Troubleshooting

### Shader Compilation Errors

Check the console output for shader compilation errors. Common issues:
- Incorrect shader file paths
- GLSL syntax errors
- Unsupported GLSL version

### Black Screen

Ensure:
- Shaders are compiling successfully
- The camera is positioned correctly
- The cube is within the view frustum
- OpenGL context is properly initialized

### Crash on Startup

Verify:
- All shared resources are properly initialized
- Shader files exist in the `resources/` directory
- CMake build completed without errors

## Further Reading

- [helios API Documentation](../../docs/heliosapi.md)
- [helios Coding Style Guide](../../docs/styleguide.md)
- [OpenGL Tutorial](https://learnopengl.com/)
- [GLSL Reference](https://www.khronos.org/opengl/wiki/OpenGL_Shading_Language)

## License

This example is part of the helios framework and is distributed under the MIT License.

