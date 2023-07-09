#version 330 core
out vec4 outColor;

in vec2 vUv;

uniform sampler2D texture1;

void main()
{    
    outColor = texture(texture1, vUv);
}