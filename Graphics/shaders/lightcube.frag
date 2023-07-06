#version 330 core
out vec4 outColor;

uniform vec3 uLightColor;

void main()
{
    outColor = vec4(uLightColor, 1.0);
}