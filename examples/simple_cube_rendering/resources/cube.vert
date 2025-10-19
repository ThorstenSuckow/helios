#version 450 core

layout (location=0) in vec3 aPos;

layout (location=1) uniform mat4 worldMatrix;

void main() {
    gl_Position = worldMatrix * vec4(aPos, 1.0f);
}