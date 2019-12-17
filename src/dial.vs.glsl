#version 410 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 FragmentPos;
out vec3 Normal;
out vec2 UV;
out vec4 FragmentPosLightSpace;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 LightSpace;

void main() {
    FragmentPos = vec3(Model * vec4(pos * 1.5, 1.0));
    Normal = mat3(transpose(inverse(Model))) * normal;
    UV = uv;
    FragmentPosLightSpace = LightSpace * vec4(FragmentPos, 1.0);

    gl_Position = Projection * View * Model * vec4(pos * 1.5, 1.0);
}