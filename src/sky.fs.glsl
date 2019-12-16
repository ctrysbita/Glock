#version 410 core
in vec3 TexCoords;

out vec4 FragColor;

uniform samplerCube Skybox;

void main() {
    FragColor = texture(Skybox, TexCoords);
}