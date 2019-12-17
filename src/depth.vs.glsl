#version 410 core
layout (location = 0) in vec3 pos;

uniform mat4 LightSpace;
uniform mat4 Model;
uniform float PosFactor;

void main() {
    gl_Position = lightSpace * Model * vec4(pos * PosFactor, 1.0);
}