#version 330 core

in vec2 vUv;
in vec4 vParticleColor;

out vec4 outColor;

uniform sampler2D particleTexture;

void main()
{
    // outColor = (texture(particleTexture, vUv) * vParticleColor);
    outColor = (texture(particleTexture, vUv));
// 
    // outColor = vec4(1.0);
}