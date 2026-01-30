#version 430 core

// <vec2 pos, vec2 tex>
// position and texture coordinate into one vec4 combined
layout(location = 0) in vec4 vertex;
out vec2 TexCoords;

layout (location=1) uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}