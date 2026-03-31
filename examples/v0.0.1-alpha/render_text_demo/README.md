# Text Rendering Demo

This example demonstrates how to render dynamic text using the helios text rendering system. It showcases FreeType-based glyph rendering with OpenGL, including font loading, text positioning, and real-time text updates.

## What You'll Learn

- Setting up the text rendering pipeline
- Loading fonts via `FontResourceProvider`
- Creating `TextRenderPrototype` for shared shader configuration
- Using `TextRenderable` for dynamic text display
- Integrating text into the scene graph
- Configuring orthographic projection for 2D UI text

## Prerequisites

- helios framework properly built and installed
- Basic understanding of C++ and OpenGL
- FreeType library (automatically included via helios)

## Project Structure

```
render_text_demo/
├── main.cpp                           # Main application code
├── resources/
│   ├── font_shader.vert              # Vertex shader for text
│   ├── font_shader.frag              # Fragment shader for text
│   ├── Roboto-SemiBoldItalic.ttf     # Font file
│   ├── OFL.txt                       # Font license
│   └── README.txt                    # Font attribution
└── README.md                         # This file
```

## Step-by-Step Tutorial

### 1. Application and Window Setup

Create an OpenGL application with a configured viewport:

```cpp
constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;

const auto app = helios::ext::glfw::app::GLFWFactory::makeOpenGLApp(
    "helios - Text Rendering Demo",
    SCREEN_WIDTH, SCREEN_HEIGHT,
    16.0f, 9.0f  // Aspect ratio
);

auto* win = dynamic_cast<helios::ext::glfw::window::GLFWWindow*>(app->current());

auto mainViewport = std::make_shared<helios::rendering::Viewport>(0.0f, 0.0f, 1.0f, 1.0f);
mainViewport->setClearFlags(std::to_underlying(helios::rendering::ClearFlags::Color))
            .setClearColor(helios::math::vec4f(0.051f, 0.051f, 0.153f, 1.0f));
win->addViewport(mainViewport);
```

### 2. UI Scene and Orthographic Camera

Text rendering uses a dedicated scene with orthographic projection:

```cpp
// Create UI scene with no culling (all text is always visible).
auto uiScene = std::make_unique<helios::scene::Scene>(
    std::make_unique<helios::scene::CullNoneStrategy>()
);

// Create UI viewport.
auto uiViewport = std::make_shared<helios::rendering::Viewport>(0.0f, 0.0f, 1.0f, 1.0f);
win->addViewport(uiViewport);

// Configure orthographic camera.
// Origin (0,0) is at the bottom-left corner of the screen.
auto uiCamera = std::make_unique<helios::scene::Camera>();
auto uiCameraSceneNode = std::make_unique<helios::scene::CameraSceneNode>(std::move(uiCamera));
auto* uiCameraSceneNode_ptr = uiCameraSceneNode.get();

uiViewport->setCameraSceneNode(uiCameraSceneNode_ptr);
uiCameraSceneNode_ptr->camera().setOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
uiCameraSceneNode_ptr->setTranslation(helios::math::vec3f(0.0f, 0.0f, -100.0f));
uiCameraSceneNode_ptr->lookAtLocal(
    helios::math::vec3f(0.0f, 0.0f, 0.0f),
    helios::math::vec3f(0.0f, 1.0f, 0.0f)
);

std::ignore = uiScene->addNode(std::move(uiCameraSceneNode));
```

### 3. Text Shader Setup

The text shader requires specific uniform locations:

```cpp
auto glyphUniformLocationMap = std::make_unique<
    helios::ext::opengl::rendering::shader::OpenGLUniformLocationMap
>();
glyphUniformLocationMap->set(helios::rendering::shader::UniformSemantics::ProjectionMatrix, 1);
glyphUniformLocationMap->set(helios::rendering::shader::UniformSemantics::TextTexture, 3);
glyphUniformLocationMap->set(helios::rendering::shader::UniformSemantics::TextColor, 4);
glyphUniformLocationMap->set(helios::rendering::shader::UniformSemantics::ModelMatrix, 8);
glyphUniformLocationMap->set(helios::rendering::shader::UniformSemantics::ViewMatrix, 9);

auto glyphShader = std::make_shared<helios::ext::opengl::rendering::shader::OpenGLShader>(
    "resources/font_shader.vert",
    "resources/font_shader.frag",
    helios::util::io::BasicStringFileReader()
);
glyphShader->setUniformLocationMap(std::move(glyphUniformLocationMap));
```

### 4. Font Loading

Load fonts via the rendering device's font resource provider:

```cpp
auto fontId = helios::engine::core::data::FontId{"roboto"};
auto& fontResourceProvider = app->renderingDevice().fontResourceProvider();
fontResourceProvider.loadFont(fontId, "resources/Roboto-SemiBoldItalic.ttf");
```

### 5. Text Prototype and Renderable

Create a reusable prototype and text instance:

```cpp
// Shared prototype with shader and font configuration.
auto uiTextPrototype = std::make_shared<helios::rendering::text::TextRenderPrototype>(
    glyphShader,
    std::make_shared<helios::rendering::text::TextShaderProperties>(),
    &fontResourceProvider
);

// Create a TextRenderable for dynamic text display.
auto textRenderable = std::make_shared<helios::rendering::text::TextRenderable>(
    std::make_unique<helios::rendering::text::TextMesh>("Hello World!", fontId),
    uiTextPrototype
);

// Add to the scene graph.
auto textSceneNode = std::make_unique<helios::scene::SceneNode>(textRenderable);
auto* textNode = uiScene->addNode(std::move(textSceneNode));
textNode->setScale({1.0f, 1.0f, 1.0f});
textNode->setTranslation({100.0f, 100.0f, 0.0f});
```

### 6. Render Loop

Update and render text each frame:

```cpp
float deltaTime = 0.0f;

while (!win->shouldClose()) {
    framePacer.beginFrame();

    app->eventManager().dispatchAll();
    inputManager.poll(0.0f);

    if (inputManager.isKeyPressed(helios::input::types::Key::ESC)) {
        win->setShouldClose(true);
    }

    // Update text content dynamically.
    textRenderable->setText(std::format("Frame Time: {:.4f} ms", deltaTime * 1000.0f));

    // Render the UI scene.
    const auto& uiSnapshot = uiScene->createSnapshot(*uiViewport);
    if (uiSnapshot.has_value()) {
        auto uiRenderPass = helios::rendering::RenderPassFactory::getInstance()
            .buildRenderPass(*uiSnapshot);
        app->renderingDevice().render(uiRenderPass);
    }

    win->swapBuffers();

    frameStats = framePacer.sync();
    deltaTime = frameStats.totalFrameTime;
}
```

## Key Concepts

### Text Rendering Pipeline

```
TextRenderable (high-level, scene graph)
       │
       ▼
TextRenderCommand (low-level, per-frame)
       │
       ▼
RenderQueue
       │
       ▼
OpenGLGlyphTextRenderer (FreeType + OpenGL)
```

### Coordinate System

Text uses screen-space coordinates with orthographic projection:

```
(0, 1080) ─────────────────── (1920, 1080)
    │                              │
    │      Text appears here       │
    │      at (100, 100)           │
    │           ●                  │
    │                              │
(0, 0) ─────────────────────── (1920, 0)
```

## Controls

- **ESC** - Exit application

## Running the Example

```bash
cd build/bin
./render_text_demo
```

## Expected Output

A window displaying "Frame Time: X.XXXX ms" where X updates each frame, rendered in white text on a dark blue background.

## See Also

- [Simple Cube Rendering](../simple_cube_rendering/README.md) - Basic 3D rendering
- [Spaceship Control](../spaceship_control/README.md) - Complete game loop example
- [Spaceship Shooting](../spaceship_shooting/README.md) - Advanced gameplay mechanics
