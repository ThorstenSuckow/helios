#version 450 core

out vec4 FragColor;

layout (location=4) uniform vec4 color;


void main() {

    FragColor = color;

}