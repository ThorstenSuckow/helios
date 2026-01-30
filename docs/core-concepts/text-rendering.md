# Text Rendering

The helios engine provides a flexible text rendering system for displaying dynamic text in your game or application. This guide covers the architecture, usage, and implementation details.

## Overview

Text rendering in helios is built on a layered architecture:

1. **TextRenderer** – Abstract interface defining the rendering contract.
2. **OpenGLGlyphTextRenderer** – OpenGL implementation using FreeType for glyph rasterization.
3. **TextRenderCommand** – Encapsulates all data needed to render a piece of text.
4. **DrawProperties** – Positioning and styling information for text.

## Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    TextRenderer                         │
│  (Abstract Interface)                                   │
├─────────────────────────────────────────────────────────┤
│  + addFontFamily(fontId, path, begin, end)             │
│  + render(command, frameUniforms)                       │
└─────────────────────────────────────────────────────────┘
                          ▲
                          │ implements
                          │
┌─────────────────────────────────────────────────────────┐
│              OpenGLGlyphTextRenderer                    │
│  (OpenGL + FreeType Implementation)                     │
├─────────────────────────────────────────────────────────┤
│  - fontCache_: map<FontId, FontCache>                  │
│  - vao_, vbo_: OpenGL buffers                          │
├─────────────────────────────────────────────────────────┤
│  + addFontFamily(...)                                   │
│  + render(...)                                          │
│  - init()                                               │
│  - loadMap(...)                                         │
│  - renderText(...)                                      │
└─────────────────────────────────────────────────────────┘
```

## Key Components

### TextRenderer

The abstract base class that defines the text rendering interface. All concrete implementations must provide:

- **addFontFamily()** – Load and cache a font for later use.
- **render()** – Render text based on a `TextRenderCommand`.

### OpenGLGlyphTextRenderer

The default OpenGL-based implementation that uses FreeType to:

1. Load font files (TTF, OTF).
2. Rasterize glyphs into GPU textures.
3. Render textured quads for each character.

#### Features

- **Per-Font Caching:** Glyphs are cached per font family for efficient reuse.
- **Dynamic Rendering:** VBO is updated per character for flexible positioning.
- **Shader Integration:** Works with custom shaders via `OpenGLShader`.

### Glyph

A structure containing all metrics for a single character:

| Field | Type | Description |
|-------|------|-------------|
| `textureId` | `unsigned int` | GPU texture containing the glyph bitmap |
| `size` | `vec2ui` | Width and height in pixels |
| `bearing` | `vec2i` | Offset from baseline to top-left of glyph |
| `advance` | `int` | Horizontal distance to next glyph (in 1/64 pixels) |

### DrawProperties

Positioning and styling data for text rendering:

| Field | Type | Description |
|-------|------|-------------|
| `fontId` | `FontId` | Which loaded font to use |
| `position` | `vec2f` | Screen-space origin (x, y) |
| `scale` | `float` | Size multiplier (default: 1.0) |

### TextRenderCommand

Encapsulates all data for a single text render operation:

- The text string to render.
- A pointer to the `TextRenderPrototype` (shader configuration).
- `DrawProperties` for positioning.
- Uniform values for the shader.

## Usage Example

### Basic Setup

```cpp
#include <helios/ext/opengl/rendering/OpenGLGlyphTextRenderer.ixx>

// Create and initialize the renderer
OpenGLGlyphTextRenderer textRenderer;
textRenderer.init();

// Load a font family (ASCII printable characters)
textRenderer.addFontFamily(
    FontId{1},
    "assets/fonts/arial.ttf",
    32,   // Start: space character
    127   // End: DEL (exclusive)
);
```

### Rendering Text

```cpp
// Create draw properties
DrawProperties drawProps{
    .fontId = FontId{1},
    .position = {100.0f, 200.0f},
    .scale = 1.0f
};

// Create the render command
TextRenderCommand command(
    "Hello, helios!",
    textPrototype,  // Pre-configured shader prototype
    drawProps,
    textUniformValues
);

// Render with frame uniforms (e.g., projection matrix)
textRenderer.render(command, frameUniforms);
```

### OpenGL State Requirements

Before rendering text, ensure the OpenGL state is configured correctly:

```cpp
// Enable blending for transparent glyph backgrounds
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

// Disable depth testing (optional, for UI text)
glDisable(GL_DEPTH_TEST);
```

## Text Shader

The text shader must:

1. Sample from texture unit 0 (the glyph texture).
2. Use the red channel as alpha (glyphs are stored as grayscale).
3. Apply the text color uniform.

### Example Fragment Shader

```glsl
#version 460 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D text;
uniform vec3 textColor;

void main() {
    float alpha = texture(text, TexCoords).r;
    FragColor = vec4(textColor, alpha);
}
```

### Example Vertex Shader

```glsl
#version 460 core

layout(location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

out vec2 TexCoords;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
```

## Performance Considerations

The current implementation renders **one draw call per character**. For applications with large amounts of text, consider:

1. **Batching:** Accumulate all glyph quads into a single VBO and render in one draw call.
2. **Texture Atlas:** Combine all glyphs into a single texture to reduce texture binds.
3. **Caching:** Pre-render static text to a texture and render as a single quad.

## Glyph Metrics

Understanding glyph metrics is essential for proper text layout:

```
                    │<--- advance --->│
    ┌───────────────┼─────────────────┤
    │               │                 │
    │    ▄▄▄▄▄      │                 │
    │   █     █     │    ← bearing.y  │
    │   █▄▄▄▄▄█     │       (top)     │
    │   █     █     │                 │
    │   █     █     │                 │
────┼───────────────┼─────────────────┼── baseline
    │   │           │                 │
    │   └─ bearing.x                  │
    │      (left)                     │
    │                                 │
    │<----- size.x ---->│             │
```

- **bearing.x:** Horizontal offset from cursor to left edge of glyph.
- **bearing.y:** Vertical offset from baseline to top of glyph.
- **size:** Bounding box of the glyph bitmap.
- **advance:** Distance to move cursor for the next character.

## Related Modules

| Module | Description |
|--------|-------------|
| `helios.rendering.text.TextRenderer` | Abstract text rendering interface |
| `helios.rendering.text.Glyph` | Glyph metrics structure |
| `helios.rendering.text.DrawProperties` | Text positioning data |
| `helios.rendering.text.TextRenderCommand` | Render command encapsulation |
| `helios.rendering.text.TextRenderPrototype` | Shader and font configuration |
| `helios.ext.opengl.rendering.OpenGLGlyphTextRenderer` | OpenGL implementation |

## See Also

- [LearnOpenGL: Text Rendering](https://learnopengl.com/In-Practice/Text-Rendering)
- [FreeType Documentation](https://freetype.org/freetype2/docs/)
- [Vri20, pp. 449-458] – Reference for the rendering technique
