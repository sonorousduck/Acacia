#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 uv>
out vec2 vUv;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform float depth;



void main()
{
    gl_Position = projection * view * model * vec4(vertex.xy, depth, 1.0);
    vUv = vertex.zw;
}