#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//uniform vec3 uLightPos;

out vec3 vNormal;
out vec3 vFragPos;
//out vec3 vLightPos;
out mat4 vView;
out vec2 vUv;

void main()
{
    vFragPos = vec3(view * model * vec4(aPos, 1.0));

    // the transpose(inverse(view * model)) should be moved to come in as a uniform
    vNormal = mat3(transpose(inverse(view * model))) * aNormal;
    //vLightPos = vec3(view * vec4(uLightPos, 1.0));
    vUv = aUv;
    vView = view;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}