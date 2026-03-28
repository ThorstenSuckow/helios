#version 450 core

layout (location=0) in vec3 aPos;

layout (location=1) uniform mat4 modelMatrix;
layout (location=2) uniform mat4 viewMatrix;
layout (location=3) uniform mat4 projectionMatrix;


void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);
}