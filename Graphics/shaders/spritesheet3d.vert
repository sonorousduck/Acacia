#version 330 core
layout (location = 0) in vec2 aVertex;
layout (location = 1) in vec2 aUv;

out vec2 vUv;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aVertex, 0.0, 1.0);
    vUv = aUv;
}