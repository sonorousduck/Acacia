#version 330 core

in vec2 vUv;
in vec3 vColor;
out vec4 outColor;

uniform int layer;
uniform sampler2DArray spritesheet;


void main()
{
    outColor = vec4(vec3(gl_FragCoord.z), 1.0);
    //outColor = texture(spritesheet, vec3(vUv, layer));
}