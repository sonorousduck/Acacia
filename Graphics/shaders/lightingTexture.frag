#version 330 core

out vec4 outColor;
// in vec3 ourColor;
in vec2 vUvCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    vec4 textureColorMix = mix(texture(texture1, vUvCoord), texture(texture2, vUvCoord), 0.2);
    outColor = vec4(lightColor * objectColor, 1.0) * textureColorMix;
    // outColor = texture(textureSampler, vUvCoord) * vec4(ourColor, 1.0f);
}