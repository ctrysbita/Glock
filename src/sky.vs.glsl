#version 410 core
layout (location = 0) in vec3 pos;

out vec3 TexCoords;

uniform mat4 View;
uniform mat4 Projection;

void main() {
    TexCoords = pos;
    vec4 pos = Projection * View * vec4(pos, 1.0);
    gl_Position = pos.xyww;
}