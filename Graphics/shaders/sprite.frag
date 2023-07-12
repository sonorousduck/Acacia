#version 330 core
in vec2 vUv;
out vec4 outColor;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{
    outColor = texture(image, vUv) ;
    // * vec4(spriteColor, 1.0)
}