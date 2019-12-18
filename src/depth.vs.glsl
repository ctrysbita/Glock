#version 410 core
layout (location = 0) in vec3 pos;

uniform mat4 LightSpace;
uniform mat4 Model;

// For position scaling.
uniform float PosFactor;

// For particle.
uniform bool IsParticle = false;
uniform vec3 Offset;

void main() {
    if (IsParticle)
        gl_Position = LightSpace * Model * vec4(pos.xy * .002f + Offset.xy, Offset.z, 1.0);
    else
        gl_Position = LightSpace * Model * vec4(pos * PosFactor, 1.0);
}