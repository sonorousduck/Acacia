#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 uv>

out vec2 vUv;
out vec4 vParticleColor;

uniform mat4 projection;
// uniform mat4 view; // Don't need this for a 2D renderer if camera isn't moving
uniform vec2 offset;
uniform vec4 color;

void main()
{
    float scale = 10.0f;
    vUv = vertex.zw;
    vParticleColor = color;
    gl_Position = projection * vec4((vertex.xy * scale) + offset, 0.0, 1.0);
}