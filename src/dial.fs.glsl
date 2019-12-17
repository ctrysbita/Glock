#version 410 core

in vec3 FragmentPos;
in vec3 Normal;
in vec2 UV;

out vec4 FragColor;

uniform sampler2D Texture;
uniform vec3 ViewPos;

void main() {
    vec3 lightPos = vec3(3,3,3);
    vec3 lightColor = vec3(1,1,1);

    // ambient
    vec3 ambient = 0.1 * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragmentPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * texture(Texture, vec2(UV.x, UV.y)).rgb;

    // specular
    float strength = 0.65;
    vec3 viewDir = normalize(ViewPos - FragmentPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = strength * spec * lightColor;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}