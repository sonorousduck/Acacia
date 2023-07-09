#version 330 core

in vec2 vUv;
in vec4 vParticleColor;

out vec4 outColor;

uniform sampler2D sprite;

void main()
{
    outColor = (texture(sprite, vUv) * vParticleColor);
}