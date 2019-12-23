#version 410 core

in vec3 FragmentPos;
in vec4 FragmentPosLightSpace;
in vec3 Normal;
in vec2 UV;

out vec4 FragmentColor;

uniform sampler2D Texture;
uniform sampler2D ShadowMap;
uniform vec3 ViewPos;
uniform vec3 LightPos = vec3(2, 2, 2);

vec3 lightColor = vec3(1);

float ShadowCalculation() {
    // Perform perspective divide.
    vec3 fragmentPosLightSpace = FragmentPosLightSpace.xyz / FragmentPosLightSpace.w;
    // Transform to [0,1] range.
    fragmentPosLightSpace = fragmentPosLightSpace * 0.5 + 0.5;

    // Calculate bias (based on depth map resolution and slope).
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragmentPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // PCF. (Feathering the edge of shadow.)
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(ShadowMap, fragmentPosLightSpace.xy + vec2(x, y) * texelSize).r;
            shadow += fragmentPosLightSpace.z - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // Remove shadow when outside the region of the light's frustum.
    if(fragmentPosLightSpace.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main() {
    vec3 textureColor = texture(Texture, vec2(UV.x, UV.y)).rgb;
    vec3 normal = normalize(Normal);

    // Ambient
    vec3 ambient = 0.1 * textureColor;

    // Diffuse
    vec3 lightDir = normalize(LightPos - FragmentPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * textureColor;

    // specular
    vec3 viewDir = normalize(ViewPos - FragmentPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * lightColor * 0.60;

    // Calculate shadow
    float shadow = ShadowCalculation();
    vec3 result = ambient + (1.0 - shadow) * (diffuse + specular);

    FragmentColor = vec4(result, 1.0);
}