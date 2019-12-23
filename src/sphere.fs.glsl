#version 410 core

in vec3 FragmentPos;
in vec3 Normal;
in vec2 UV;

out vec4 FragmentColor;

uniform sampler2D Texture;
uniform vec3 ViewPos;
uniform vec3 LightPos = vec3(2, 2, 2);

vec3 lightColor = vec3(1);

void main() {
    vec3 textureColor = texture(Texture, vec2(UV.x, 1 - UV.y)).rgb;
    vec3 normal = normalize(Normal);

    // Ambient
    vec3 ambient = 0.1 * textureColor;

    // Diffuse
    vec3 lightDir = normalize(LightPos - FragmentPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * textureColor;

    // Specular
    vec3 viewDir = normalize(ViewPos - FragmentPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * lightColor * 0.60;

    vec3 result = ambient + diffuse + specular;
    FragmentColor = vec4(result, 1.0);
}