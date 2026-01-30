# Text Rendering Demo

This example demonstrates how to render dynamic text using the helios text rendering system. It showcases FreeType-based glyph rendering with OpenGL, including font loading, text positioning, and real-time text updates.

## What You'll Learn

- Setting up the text rendering pipeline
- Loading fonts with `TextRenderer::addFontFamily()`
- Creating `TextRenderPrototype` for shared shader configuration
- Using `TextRenderable` for dynamic text display
- Building `TextRenderCommand` objects for the render queue
- Configuring orthographic projection for 2D text

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
const auto app = helios::ext::glfw::app::GLFWFactory::makeOpenGLApp(
    "helios - Text Rendering Demo", 800, 600, 4, 3
);

auto win = dynamic_cast<helios::ext::glfw::window::GLFWWindow*>(app->current());
auto mainViewport = std::make_shared<helios::rendering::Viewport>(0.0f, 0.0f, 1.0f, 1.0f);

mainViewport->setClearFlags(std::to_underlying(helios::rendering::ClearFlags::Color))
              .setClearColor(helios::math::vec4f(0.051f, 0.051f, 0.153f, 1.0f));
win->addViewport(mainViewport);
```

**What's happening:**
- Creates a window with 4:3 aspect ratio
- Configures a viewport covering the entire window
- Sets a dark blue clear color

### 2. Orthographic Projection

Text rendering uses orthographic projection where screen coordinates map directly to pixels:

```cpp
helios::math::mat4f projection = helios::math::ortho(
    0.0f, 800.0f,   // left, right
    0.0f, 600.0f,   // bottom, top
    -1.0f, 100.0f   // near, far
);
```

**What's happening:**
- `(0, 0)` is the bottom-left corner of the screen
- `(800, 600)` is the top-right corner
- Text position is specified in screen pixels

### 3. Shader Setup

The text shader requires specific uniform locations for projection, texture, and color:

```cpp
auto uniformLocationMap = std::make_unique<
    helios::ext::opengl::rendering::shader::OpenGLUniformLocationMap
>();
uniformLocationMap->set(UniformSemantics::ProjectionMatrix, 1);
uniformLocationMap->set(UniformSemantics::TextTexture, 3);
uniformLocationMap->set(UniformSemantics::TextColor, 4);

auto shader = std::make_shared<helios::ext::opengl::rendering::shader::OpenGLShader>(
    "resources/font_shader.vert",
    "resources/font_shader.frag",
    helios::util::io::BasicStringFileReader()
);
shader->setUniformLocationMap(std::move(uniformLocationMap));
```

**Vertex Shader (`font_shader.vert`):**
```glsl
#version 430 core

layout(location = 0) in vec4 vertex;  // <vec2 pos, vec2 tex>
out vec2 TexCoords;

layout (location=1) uniform mat4 projectionMatrix;

void main() {
    gl_Position = projectionMatrix * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
```

**Fragment Shader (`font_shader.frag`):**
```glsl
#version 430 core

in vec2 TexCoords;
out vec4 color;

layout (location=3) uniform sampler2D text;
layout (location=4) uniform vec4 textColor;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = textColor * sampled;
}
```

**Key Points:**
- The vertex shader combines position and texture coordinates in a single `vec4`
- The fragment shader samples the red channel as alpha (FreeType grayscale glyphs)
- Text color is applied as a uniform

### 4. Font Loading

Register a font family with the text renderer:

```cpp
auto WorkbenchFont = helios::engine::core::data::FontId{"workbench"};

auto& openGlDevice = app->renderingDevice();
openGlDevice.textRenderer().addFontFamily(
    WorkbenchFont, 
    "resources/Roboto-SemiBoldItalic.ttf"
);
```

**What's happening:**
- `FontId` creates a unique identifier from the string "workbench"
- `addFontFamily()` loads the TTF file and caches glyph textures
- Default character range is ASCII 0-127

### 5. Text Prototype and Renderable

Create a reusable prototype and a text instance:

```cpp
// Shared prototype with shader configuration
auto textPrototype = std::make_shared<helios::rendering::text::TextRenderPrototype>(
    shader, 
    std::make_shared<helios::rendering::text::config::TextShaderProperties>()
);

// Instance-specific text renderable
auto textRenderable = std::make_unique<helios::rendering::text::TextRenderable>(
    "Hello World",
    textPrototype,
    helios::rendering::text::DrawProperties{
        .fontId = WorkbenchFont,
        .position = helios::math::vec2f{50.0f, 50.0f}
    },
    helios::rendering::text::config::TextShaderPropertiesOverride{
        .baseColor = helios::util::Colors::Pink
    }
);
```

**What's happening:**
- `TextRenderPrototype` holds shared shader and text properties
- `TextRenderable` combines the prototype with instance-specific data
- `DrawProperties` specifies font, position, and scale
- `TextShaderPropertiesOverride` allows per-instance color customization

### 6. Render Loop

Update and render text each frame:

```cpp
unsigned int i = 0;
while (!win->shouldClose()) {
    inputManager.poll(0.0f);

    if (inputManager.isKeyPressed(helios::input::types::Key::ESC)) {
        win->setShouldClose(true);
    }

    // Frame uniforms
    auto frameUniformValues = std::make_unique<helios::rendering::shader::UniformValueMap>();
    frameUniformValues->set(UniformSemantics::ProjectionMatrix, projection);
    frameUniformValues->set(UniformSemantics::TextTexture, 
        helios::ext::opengl::rendering::OpenGLGlyphTextRenderer::textTextureUnit());

    // Update text dynamically
    textRenderable->setText(std::format("Hello World {0}", i++));

    // Build uniform values from renderable
    auto uniformValues = helios::rendering::shader::UniformValueMap();
    textRenderable->writeUniformValues(uniformValues);

    // Create render command
    auto textRenderCommand = helios::rendering::text::TextRenderCommand(
        std::string{textRenderable->text()},
        textPrototype.get(),
        textRenderable->drawProperties(),
        uniformValues
    );

    // Build render queue and pass
    auto uiRenderQueue = std::make_unique<helios::rendering::RenderQueue>();
    uiRenderQueue->add(std::move(textRenderCommand));

    auto uiRenderPass = helios::rendering::RenderPass(mainViewport, std::move(uiRenderQueue));
    uiRenderPass.setFrameUniformValues(std::move(frameUniformValues));

    openGlDevice.render(uiRenderPass);
    win->swapBuffers();
}
```

**What's happening:**
1. Poll input and check for ESC key
2. Set up frame-level uniforms (projection, texture unit)
3. Update the text string dynamically
4. Write uniform values from the renderable
5. Create a `TextRenderCommand` with all rendering data
6. Add to render queue and execute the render pass

## Key Concepts

### Text Rendering Pipeline

```
TextRenderable (high-level)
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
(0, 600) ────────────────── (800, 600)
    │                            │
    │     Text appears here      │
    │     at (50, 50)            │
    │         ●                  │
    │                            │
(0, 0) ────────────────────── (800, 0)
```

## Running the Example

```bash
cd build/examples/render_text_demo
./main
```

Press **ESC** to close the application.

## Expected Output

A window displaying "Hello World X" where X increments each frame, rendered in pink text on a dark blue background.

## See Also

- [Text Rendering Guide](/docs/core-concepts/text-rendering)
- [Simple Cube Rendering](/docs/examples/simple-cube) - Basic 3D rendering
- [Spaceship Control](/docs/examples/spaceship-control) - Complete game loop example
