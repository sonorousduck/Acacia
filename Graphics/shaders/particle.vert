#version 330 core
layout (location = 0) in vec2 aVertex;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec4 aPositionSize;
layout (location = 3) in vec4 aColor;

out vec2 vUv;
out vec4 vParticleColor;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vUv = aUv;
    vParticleColor = aColor;

    mat4 translation = mat4(1.0);
    mat4 scale = mat4(1.0);
    mat4 rotation = mat4(1.0);
    
    translation[0][0] = aPositionSize.x;
    translation[1][1] = aPositionSize.y;

    scale[0][0] = aPositionSize.z;
    scale[1][1] = aPositionSize.w;

    mat4 model = translation * rotation * scale;

    gl_Position = projection * view * model * vec4(aVertex * 10.0, 0.0, 1.0);
    // gl_Position = projection * view * instanceMatrix * vec4(aVertex * 10.0, 0.0, 1.0);

}