#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture1;
uniform vec3 objectColor;
uniform bool useTexture;

// Ambient light
uniform bool enableAmbient;
uniform vec3 ambientColor;

// Directional light
uniform bool enableDirectional;
uniform vec3 dirLightDir;
uniform vec3 dirLightColor;

// Point light
uniform bool enablePoint;
uniform vec3 pointLightPos;
uniform vec3 pointLightColor;
uniform float pointLightConstant;
uniform float pointLightLinear;
uniform float pointLightQuadratic;

// Spot/Conical light
uniform bool enableSpot;
uniform vec3 spotLightPos;
uniform vec3 spotLightDir;
uniform vec3 spotLightColor;
uniform float spotLightCutoff;
uniform float spotLightOuterCutoff;
uniform float spotLightConstant;
uniform float spotLightLinear;
uniform float spotLightQuadratic;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(-FragPos); // Assuming camera at origin
    vec3 result = vec3(0.0);

    // Ambient
    if (enableAmbient)
        result += ambientColor * objectColor;

    // Directional
    if (enableDirectional) {
        vec3 lightDir = normalize(-dirLightDir);
        float diff = max(dot(norm, lightDir), 0.0);
        result += dirLightColor * diff * objectColor;
    }

    // Point
    if (enablePoint) {
        vec3 lightDir = normalize(pointLightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        float distance = length(pointLightPos - FragPos);
        float attenuation = 1.0 / (pointLightConstant + pointLightLinear * distance + pointLightQuadratic * (distance * distance));
        result += pointLightColor * diff * objectColor * attenuation;
    }

    // Spot/Conical
    if (enableSpot) {
        vec3 lightDir = normalize(spotLightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        float distance = length(spotLightPos - FragPos);
        float attenuation = 1.0 / (spotLightConstant + spotLightLinear * distance + spotLightQuadratic * (distance * distance));
        float theta = dot(lightDir, normalize(-spotLightDir));
        float epsilon = spotLightCutoff - spotLightOuterCutoff;
        float intensity = clamp((theta - spotLightOuterCutoff) / epsilon, 0.0, 1.0);
        result += spotLightColor * diff * objectColor * attenuation * intensity;
    }

    vec4 baseColor = useTexture ? texture(texture1, TexCoord) : vec4(objectColor, 1.0);
    FragColor = vec4(result, 1.0) * baseColor;
}
