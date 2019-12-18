#version 410 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 FragmentPos;
out vec3 Normal;
out vec2 UV;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() {
    // Calculate fragment position and normal in word space.
    FragmentPos = vec3(Model * vec4(pos / 60, 1.0));
    Normal = mat3(transpose(inverse(Model))) * normal;
    UV = uv;

    gl_Position = Projection * View * Model * vec4(pos / 60, 1.0);
}