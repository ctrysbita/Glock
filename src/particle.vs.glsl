#version 410 core
layout (location = 0) in vec2 vertex; // position

out vec4 ParticleColor;

uniform mat4 projection;
uniform vec3 offset;
uniform vec4 color;
uniform mat4 model;
uniform mat4 view;

void main()
{
    float scale = .002f;
    ParticleColor = color;
    gl_Position = projection * view * model * vec4((vertex * scale) + offset.xy, offset.z, 1.0);
}