#version 330 core
in vec2 vUv;
out vec4 outColor;

uniform sampler2D image;
uniform vec4 spriteColor;

void main()
{
    outColor = texture(image, vUv) * spriteColor;

    // if (outColor.a < 0.1)
    // {
    //     discard;
    // }
    // * vec4(spriteColor, 1.0)
}