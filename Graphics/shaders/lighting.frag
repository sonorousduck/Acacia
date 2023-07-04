#version 330 core

out vec4 outColor;
// in vec3 ourColor;


uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    vec3 result = ambient * objectColor;

    outColor = vec4(result, 1.0);
    // outColor = texture(textureSampler, vUvCoord) * vec4(ourColor, 1.0f);
}