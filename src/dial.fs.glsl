#version 410 core

in vec3 FragmentPos;
in vec3 Normal;
in vec2 UV;
in vec4 FragmentPosLightSpace;

out vec4 FragColor;

uniform sampler2D Texture;
uniform vec3 ViewPos;
uniform sampler2D ShadowMap;

float ShadowCalculation() {
    // perform perspective divide
    vec3 projCoords = FragmentPosLightSpace.xyz / FragmentPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(ShadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main() {
    vec3 lightPos = vec3(3,3,3);
    vec3 textureColor = texture(Texture, vec2(UV.x, UV.y)).rgb;

    // ambient
    vec3 ambient = 0.1 * textureColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragmentPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * textureColor;

    // specular
    float strength = 0.65;
    vec3 viewDir = normalize(ViewPos - FragmentPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = strength * spec * vec3(1);

    // Calculate shadow
    float shadow = ShadowCalculation();
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * textureColor;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}