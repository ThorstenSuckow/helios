#version 430 core

// <vec2 pos, vec2 tex>
// position and texture coordinate into one vec4 combined
layout(location = 0) in vec4 vertex;
out vec2 TexCoords;

layout (location=1) uniform mat4 projectionMatrix;

layout (location=8) uniform mat4 modelMatrix;
layout (location=9) uniform mat4 viewMatrix;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}