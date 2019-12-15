#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 Normal;
out vec2 UV;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() {
    Normal = normal;
    UV = uv;
    gl_Position = Projection * View * Model * vec4(pos / 150, 1.0);
}