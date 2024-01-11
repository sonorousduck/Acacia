#version 330 core
in vec2 vUv;
in vec3 vColor;
out vec4 outColor;

uniform sampler2D image;
uniform vec4 spriteColor;

void main()
{
    //outColor = texture(image, vUv) * spriteColor;
    outColor = vec4(vec3(gl_FragCoord.z), 1.0);

    // outColor = vec4(vColor, 1.0);
    // if (outColor.a < 0.1)
    // {
    //     discard;
    // }
    // * vec4(spriteColor, 1.0)
}