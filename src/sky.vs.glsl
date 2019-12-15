#version 460 core
layout (location = 0) in vec3 Pos;

out vec3 TexCoords;

uniform mat4 View;
uniform mat4 Projection;

void main() {
    TexCoords = Pos;
    vec4 pos = Projection * View * vec4(Pos, 1.0);
    gl_Position = pos.xyww;
}  