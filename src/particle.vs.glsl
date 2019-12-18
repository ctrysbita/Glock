#version 410 core
layout (location = 0) in vec2 pos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 Offset;

void main() {
    gl_Position = Projection * View * Model * vec4(pos * .002f + Offset.xy, Offset.z, 1.0);
}