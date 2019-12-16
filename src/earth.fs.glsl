#version 330 core

in vec3 Normal;
in vec2 UV;

out vec4 FragColor;

uniform sampler2D tex;

void main() {    
    FragColor = texture(tex, vec2(UV.x, 1 - UV.y));
}