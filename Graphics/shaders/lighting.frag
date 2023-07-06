#version 330 core
struct Material {
    // vec3 ambient; // You don't need these once you have a diffuse map
    // vec3 diffuse; // This was replaced by the sampler2D. Eventually, build out a system that has both
    sampler2D diffuse;
    //vec3 specular;
    sampler2D specular;
    //sampler2D emission;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
    float cutoff;
    float outerCutoff;
};



out vec4 outColor;


in vec3 vNormal;
in vec3 vFragPos;
in mat4 vView;
in vec2 vUv;

uniform Material material;
uniform DirectionalLight directionalLight;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform SpotLight spotLight;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDir = normalize(-light.direction);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDirection = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);

    // Combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, vUv));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, vUv));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, vUv));
    
    return (ambient + diffuse + specular);
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightPos = vec3(vView * vec4(light.position, 1.0));
    vec3 lightDir = normalize(lightPos - fragPos);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Attenuation
    float computedDistance = length(lightPos - vFragPos);
    float attenuation = 1.0 / (light.constant + light.linear * computedDistance + light.quadratic * (computedDistance * computedDistance));

    // Combine
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, vUv));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, vUv));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, vUv));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightPos = vec3(vView * vec4(light.position, 1.0));
    vec3 lightDir = normalize(lightPos - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, vUv));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vUv));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, vUv));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}




void main()
{
    vec3 norm = normalize(vNormal);
    vec3 viewDir = normalize(-vFragPos);

    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    
    // Phase 1: Directional Lighting
    vec3 result = calculateDirectionalLight(directionalLight, norm, viewDir);

    // Phase 2: Point lights
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += calculatePointLight(pointLights[i], norm, vFragPos, viewDir);
    }

    // Phase 3: Spot Light
    result += calculateSpotLight(spotLight, norm, vFragPos, viewDir);
    
    outColor = vec4(result, 1.0);    
}