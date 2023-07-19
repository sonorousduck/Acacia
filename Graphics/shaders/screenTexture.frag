#version 330 core

in vec2 vUv;

uniform sampler2D screenTexture;

out vec4 outColor;


const float offset = 1.0 / 300.0;

void main()
{
    outColor = vec4(vec3(texture(screenTexture, vUv)), 1.0); 


    // Invert all colors
    // outColor = vec4(vec3(1.0 - texture(screenTexture, vUv)), 1.0); 

    // Grayscale
    // outColor = texture(screenTexture, vUv);
    // float average = 0.2126 * outColor.r + 0.7152 * outColor.g + 0.0722 * outColor.b;
    // outColor = vec4(average, average, average, 1.0);

    // Sharpen, Blur + 
   //  vec2 offsets[9] = vec2[](
   //      vec2(-offset, offset), // Top-left
   //      vec2(0, offset), // Top-Center
   //      vec2(offset, offset), // Top-Right
   //      vec2(-offset, 0), // Center-Left
   //      vec2(0.0, 0.0), // Center-Center
   //      vec2(offset, 0.0), // Center-Right
   //      vec2(-offset, -offset), // Bottom-Left
   //      vec2(0.0, -offset), // Bottom-Center
   //      vec2(offset, -offset) // Bottom-Right
   //  );

   //  // Sharpen Kernel
   //   float kernel[9] = float[](
   //      -1, -1, -1,
   //      -1, 9, -1,
   //      -1, -1, -1
   //   );

    // Blur kernel
    //  float kernel[9] = float[](
    //     1.0 / 16, 2.0 / 16, 1.0 / 16,
    //     2.0 / 16, 4.0 / 16, 2.0 / 16,
    //     1.0 / 16, 2.0 / 16, 1.0 / 16
    //  );

    // Edge Detection
    // float kernel[9] = float[](
    //     1, 1, 1,
    //     1, -8, 1,
    //     1, 1, 1
    //  );

   //   vec3 sampleUv[9];
   //   for (int i = 0; i < 9; i++)
   //   {
   //      sampleUv[i] = vec3(texture(screenTexture, vUv.st + offsets[i]));
   //   }
   //   vec3 col = vec3(0.0);
   //   for (int i = 0; i < 9; i++)
   //   {
   //      col += sampleUv[i] * kernel[i];
   //   }

   //  outColor = vec4(col, 1.0);
}