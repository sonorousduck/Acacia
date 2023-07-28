#version 330 core
layout (location = 0) in vec2 aVertex;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec4 aPositionSize;
layout (location = 3) in vec4 aColor;

out vec2 vUv;
out vec4 vParticleColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vUv = aUv;
    vParticleColor = aColor;
    gl_Position = projection * view * aPositionSize * vec4(aVertex * 10.0, 0.0, 1.0);
    // gl_Position = projection * view * instanceMatrix * vec4(aVertex * 10.0, 0.0, 1.0);

}