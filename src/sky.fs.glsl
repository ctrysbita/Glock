#version 410 core
in vec3 TexCoords;

out vec4 FragmentColor;

uniform samplerCube Skybox;

void main() {
    FragmentColor = texture(Skybox, TexCoords);
}