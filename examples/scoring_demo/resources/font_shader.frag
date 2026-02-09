#version 430 core
in vec2 TexCoords;
out vec4 color;

// mono colored bitmap image of the glyph
layout (location=3) uniform sampler2D text;
// color uniform for final color
layout (location=4) uniform vec4 textColor;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = textColor * sampled;
}


