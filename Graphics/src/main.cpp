// #include "glad/glad.h"
// #include <GLFW/glfw3.h>

// #include <iostream>
// #include "shader.hpp"

// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <algorithm>
// #include "camera.hpp"
// #include <asset_load.hpp>
// #include <texture_asset.hpp>
// #include <filesystem>
// #include <chrono>
// #include <map>
// #include "model.hpp"

// #include <ft2build.h>
// #include FT_FREETYPE_H

// unsigned int WIDTH = 1600;
// unsigned int HEIGHT = 1200;

// // timing
// float deltaTime = 0.0f;	// time between current frame and last frame
// float lastFrame = 0.0f;
// Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));
// float lastX = WIDTH / 2.0f;
// float lastY = HEIGHT / 2.0f;

// bool firstMouse = true;

// unsigned int textVAO = 0, textVBO = 0;


// struct Character
// {
//     unsigned int TextureID;     // ID handle of the glyph texture
//     glm::ivec2   Size;          // Size of the glyph
//     glm::ivec2   Bearing;       // Offset from baseline to left/top of glyph
//     unsigned int Advance;       // Offset to advance the next glyph
// };

// std::map<char, Character> Characters{};


// void framebuffer_size_callback(GLFWwindow* window, int width, int height)
// {
//     glViewport(0, 0, width, height);
//     WIDTH = width;
//     HEIGHT = height;
//     std::cout << WIDTH << std::endl;
// }

// void processInput(GLFWwindow* window)
// {
//     float cameraSpeed = 2.50f;


//     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//     {
//         glfwSetWindowShouldClose(window, true);
//     }

//     if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//     {
//         camera.ProcessKeyboard(FORWARD, deltaTime);
//     }
//     if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//     {
//         camera.ProcessKeyboard(BACKWARD, deltaTime);
//     }
//     if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//     {
//         camera.ProcessKeyboard(LEFT, deltaTime);
//     }
//     if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//     {
//         camera.ProcessKeyboard(RIGHT, deltaTime);
//     }
// }

// void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn)
// {

//     float xPos = static_cast<float>(xPosIn);
//     float yPos = static_cast<float>(yPosIn);
//     // Orient yourself wherever the mouse starts
//     if (firstMouse)
//     {
//         lastX = xPos;
//         lastY = yPos;
//         firstMouse = false;
//     }


//     float xOffset = xPos - lastX;
//     float yOffset = lastY - yPos;
//     lastX = xPos;
//     lastY = yPos;

//     camera.ProcessMouseMovement(xOffset, yOffset);
// }

// void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
// {
//     camera.ProcessMouseScroll(static_cast<float>(yOffset));
// }

// unsigned int loadTexture(char const* path)
// {
//     unsigned int textureID = 0;
//     glGenTextures(1, &textureID);


//     assets::AssetFile file{};
//     bool loaded = assets::load_binaryfile(path, file);

//     if (!loaded)
//     {
//         std::cout << "Error when loading image: " << path << std::endl;
//         return -1;
//     }

//     assets::TextureInfo textureInfo = assets::read_texture_info(&file);

//     int nrComponents = textureInfo.textureFormat;
//     int width = textureInfo.pixelSize[0];
//     int height = textureInfo.pixelSize[1];

//     if (file.binaryBlob.data())
//     {
//         GLenum format{};

//         switch (nrComponents)
//         {
//         case 1:
//             format = GL_RED;
//             break;
//         case 3:
//             format = GL_RGB;
//             break;
//         case 4:
//             format = GL_RGBA;
//             break;
//         default:
//             break;
//         }

//         std::vector<char> data(textureInfo.textureSize);

//         assets::unpack_texture(&textureInfo, file.binaryBlob.data(), file.binaryBlob.size(), data.data());

//         glBindTexture(GL_TEXTURE_2D, textureID);
//         glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data.data());
//         glGenerateMipmap(GL_TEXTURE_2D);

//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//     }


//     /*int width = 0, height = 0, nrComponents = 0;
//     unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

//     if (data)
//     {
//         GLenum format{};
        
//         switch (nrComponents)
//         {
//         case 1:
//             format = GL_RED;
//             break;
//         case 3:
//             format = GL_RGB;
//             break;
//         case 4:
//             format = GL_RGBA;
//             break;
//         default:
//             break;
//         }

//         glBindTexture(GL_TEXTURE_2D, textureID);
//         glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//         glGenerateMipmap(GL_TEXTURE_2D);

//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//         stbi_image_free(data);
//     }
//     else 
//     {
//         std::cout << "Texture failed to load at path: " << path << std::endl;
//         stbi_image_free(data);
//     }*/

//     return textureID;
// }

// void RenderText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color)
// {
//     s.use();
//     glUniform3f(glGetUniformLocation(s.ID, "textColor"), color.x, color.y, color.z);
//     glActiveTexture(GL_TEXTURE0);
//     glBindVertexArray(textVAO);

//     // Iterate through all characters
//     std::string::const_iterator c;
//     for (c = text.begin(); c != text.end(); c++)
//     {
//         Character ch = Characters[*c];

//         float xpos = x + ch.Bearing.x * scale;
//         float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

//         float w = ch.Size.x * scale;
//         float h = ch.Size.y * scale;

//         // Update VBO for each character
//         float vertices[6][4] = {
//             { xpos,         ypos + h,   0.0f, 0.0f  },
//             { xpos,         ypos,       0.0f, 1.0f  },
//             { xpos + w,     ypos,       1.0f, 1.0f  },
//             { xpos,         ypos + h,   0.0f, 0.0f  },
//             { xpos + w,     ypos,       1.0f, 1.0f  },
//             { xpos + w,     ypos + h,   1.0f, 0.0f  }
//         };
    
//         // Render glyph texture over quad
//         glBindTexture(GL_TEXTURE_2D, ch.TextureID);
//         // Update content of VBO memory
//         glBindBuffer(GL_ARRAY_BUFFER, textVBO);
//         glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
//         glBindBuffer(GL_ARRAY_BUFFER, 0);

//         // Render Quad
//         glDrawArrays(GL_TRIANGLES, 0, 6);
        
//         // Now advance cursors for next glyph (Note that advance number is 1/64 pixels)
//         x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
//     }

//     glBindVertexArray(0);
//     glBindTexture(GL_TEXTURE_2D, 0);
// }





// int main()
// {
//     GLFWwindow* window{};
//     glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

//     /* Initialize the library */
//     if (!glfwInit())
//         return -1;

//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

// #ifdef __APPLE__
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)
// #endif

//         /* Create a windowed mode window and its OpenGL context */
//     window = glfwCreateWindow(WIDTH, HEIGHT, "Ebony", NULL, NULL);
//     if (!window)
//     {
//         std::cout << "Failed to create GLFW window" << std::endl;
//         glfwTerminate();
//         return -1;
//     }

//     // Capture the cursor and hide it
//     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//     glfwSetCursorPosCallback(window, mouse_callback);
//     glfwSetScrollCallback(window, scroll_callback);

//     // This sets the callback to change the viewport. This is also how you would record input from glfw as well
//     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

//     /* Make the window's context current */
//     glfwMakeContextCurrent(window);



//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//     {
//         std::cout << "Failed to initialize GLAD" << std::endl;
//         return -1;
//     }

//     Shader lightingShader("shaders/lighting.vert", "shaders/lighting.frag");
//     Shader lightcubeShader("shaders/lightcube.vert", "shaders/lightcube.frag");
//     Shader textShader("shaders/font.vert", "shaders/font.frag");
//     //Shader ourShader("shaders/modelLoading.vert", "shaders/modelLoading.frag");


//     // set up vertex data (and buffer(s)) and configure vertex attributes
//     // ------------------------------------------------------------------
//     //float vertices[] = {
//     //    // positions         // colors
//     //     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
//     //     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
//     //    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
//     //    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
//     //};

//     float vertices[] = {
//         // positions          // normals           // texture coords
//         -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
//          0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
//          0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
//          0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
//         -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
//         -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

//         -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
//          0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
//          0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
//          0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
//         -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
//         -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

//         -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
//         -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
//         -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
//         -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
//         -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
//         -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

//          0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
//          0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
//          0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
//          0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
//          0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
//          0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

//         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
//          0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
//          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
//          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
//         -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
//         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

//         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
//          0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
//          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
//          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
//         -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
//         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
//     };

//     // Create a framebuffer object
//     // ============================================================================================
//     unsigned int fbo = 0;

//     glGenFramebuffers(1, &fbo);


//     // ============================================
//     // Create a VBO - Vertex Buffer Object - which holds a large number of vertices in the GPU's memory

//     unsigned int VBO = 0;
//     unsigned int cubeVAO = 0;

//     glGenVertexArrays(1, &cubeVAO);
//     glGenBuffers(1, &VBO);

//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//     // Bind to the vertex array object before binding buffer and data
//     glBindVertexArray(cubeVAO);

    
//     // ============================================
//     // Create EBO - Element Buffer Object
//     //unsigned int EBO = 0;
//     //glGenBuffers(1, &EBO);

//     //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//     //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

//     // ============================================



//     // 1st parameter: Specifies the location (i.e. in the vertex shader we said layout(position = 0)
//     // 2nd parameter: size of vertex attribute. Since it is a vec3, 3 goes here
//     // 5th parameter: Stride. So this has a stride of 3 vertices (since 1 vertex is 1 float)
//     // 6th parameter: This is the offset of where the position data begins in the buffer
//     // Position Attribute
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(0);

//     // Normal Attribute
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//     glEnableVertexAttribArray(1);

//     // UVs
//     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//     glEnableVertexAttribArray(2);

//     // color attribute
//     //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//     //glEnableVertexAttribArray(1);


//     // ============================================
//     // To draw in wireframe mode
//     //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//     // ============================================
    
//     // Configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
//     // =============================================================================================
//     unsigned int lightVAO = 0;
//     glGenVertexArrays(1, &lightVAO);
//     glBindVertexArray(lightVAO);

//     // We only need to bind to the VBO, the container's VBO data already contains the data
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);

//     // This becomes a 6 since we are reusing the vertices. This is much more efficient than creating its own VBO, even though it is just not using the normals
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(0);

//     // =============================================================================================

//     glm::vec3 cubePositions[] = {
//      glm::vec3(0.0f,  0.0f,  0.0f),
//      glm::vec3(2.0f,  5.0f, -15.0f),
//      glm::vec3(-1.5f, -2.2f, -2.5f),
//      glm::vec3(-3.8f, -2.0f, -12.3f),
//      glm::vec3(2.4f, -0.4f, -3.5f),
//      glm::vec3(-1.7f,  3.0f, -7.5f),
//      glm::vec3(1.3f, -2.0f, -2.5f),
//      glm::vec3(1.5f,  2.0f, -2.5f),
//      glm::vec3(1.5f,  0.2f, -1.5f),
//      glm::vec3(-1.3f,  1.0f, -1.5f)
//     };
    
//     glm::vec3 pointLightPositions[] = {
//         glm::vec3(0.7f,  0.2f,  2.0f),
//         glm::vec3(2.3f, -3.3f, -4.0f),
//         glm::vec3(-4.0f,  2.0f, -12.0f),
//         glm::vec3(0.0f,  0.0f, -3.0f)
//     };


//     unsigned int diffuseMap = loadTexture("textures/box.tx");
//     unsigned int specularMap = loadTexture("textures/box_specular.tx");
    
//     //Model ourModel("models/backpack.obj");



//     //unsigned int emissionMap = loadTexture("textures/box_emission.jpg");
//     lightingShader.use();
//     lightingShader.setInt("material.diffuse", 0); // Only have to do this once
//     lightingShader.setInt("material.specular", 1);
//     //lightingShader.setInt("material.emission", 2);


//     // Load font
//     // =============================================================================================
//     FT_Library ft{};
//     if (FT_Init_FreeType(&ft))
//     {
//         std::cout << "Error: Freetype: Could not init FreeType Library" << std::endl;
//         return -1;
//     }

//     FT_Face face{};
//     if (FT_New_Face(ft, "fonts/super-indie-font/SuperIndie.ttf", 0, &face))
//     {
//         std::cout << "Error: Freetype: Failed to load the font" << std::endl;
//         return -1;
//     }

//     FT_Set_Pixel_Sizes(face, 0, 24);

//     glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

//     // We are going to loop through 128 characters of the ASCII set
//     for (unsigned char c = 0; c < 128; c++)
//     {
//         // Load character glyph
//         if (FT_Load_Char(face, c, FT_LOAD_RENDER))
//         {
//             std::cout << "Error: Freetype: Failed to load Glyph (In particular, glyph: " << c << std::endl;
//             continue;
//         }

//         // Generate texture
//         unsigned int texture = 0;
//         glGenTextures(1, &texture);
//         glBindTexture(GL_TEXTURE_2D, texture);
//         glTexImage2D(
//             GL_TEXTURE_2D,
//             0,
//             GL_RED,
//             face->glyph->bitmap.width,
//             face->glyph->bitmap.rows,
//             0,
//             GL_RED,
//             GL_UNSIGNED_BYTE,
//             face->glyph->bitmap.buffer
//         );

//         // Set texture options
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//         Character character = {
//             texture,
//             glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
//             glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
//             face->glyph->advance.x
//         };
//         Characters.insert(std::pair<char, Character>(c, character));
//     }

//     FT_Done_Face(face);
//     FT_Done_FreeType(ft);


//     glEnable(GL_BLEND);
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//     glm::mat4 textProjection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
//     textShader.use();
//     glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(textProjection));

//     glGenVertexArrays(1, &textVAO);
//     glGenBuffers(1, &textVBO);
//     glBindVertexArray(textVAO);
//     glBindBuffer(GL_ARRAY_BUFFER, textVBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW); // It needs 6 vertices of 4 floats each, so 6 * 4 floats of memory (x, y, u, v)
    
//     glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
//     glEnableVertexAttribArray(0);

//     glBindBuffer(GL_ARRAY_BUFFER, 0);
//     glBindVertexArray(0);



//     glEnable(GL_DEPTH_TEST);
//     /* Loop until the user closes the window */
//     while (!glfwWindowShouldClose(window))
//     {
//         float currentFrame = static_cast<float>(glfwGetTime());
//         deltaTime = currentFrame - lastFrame;
//         lastFrame = currentFrame;

//         processInput(window);

//         /* Render here */
//         //glClearColor(0.4f, 0.58f, 0.93f, 1.0f);
//         glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


//         lightingShader.use();
//         //lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
//         //lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
//         //lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
//         lightingShader.setFloat("material.shininess", 64.0f);

        
//         /*
//            Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
//            the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
//            by defining light types as classes and set their values in there, or by using a more efficient uniform approach
//            by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
//         */
//         // directional light
//         lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
//         lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
//         lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
//         lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
//         // point light 1
//         lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
//         lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
//         lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
//         lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
//         lightingShader.setFloat("pointLights[0].constant", 1.0f);
//         lightingShader.setFloat("pointLights[0].linear", 0.09f);
//         lightingShader.setFloat("pointLights[0].quadratic", 0.032f);
//         // point light 2
//         lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
//         lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
//         lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
//         lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
//         lightingShader.setFloat("pointLights[1].constant", 1.0f);
//         lightingShader.setFloat("pointLights[1].linear", 0.09f);
//         lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
//         // point light 3
//         lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
//         lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
//         lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
//         lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
//         lightingShader.setFloat("pointLights[2].constant", 1.0f);
//         lightingShader.setFloat("pointLights[2].linear", 0.09f);
//         lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
//         // point light 4
//         lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
//         lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
//         lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
//         lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
//         lightingShader.setFloat("pointLights[3].constant", 1.0f);
//         lightingShader.setFloat("pointLights[3].linear", 0.09f);
//         lightingShader.setFloat("pointLights[3].quadratic", 0.032f);
//         // spotLight
//         lightingShader.setVec3("spotLight.position", camera.Position);
//         lightingShader.setVec3("spotLight.direction", camera.Front);
//         lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
//         lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
//         lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
//         lightingShader.setFloat("spotLight.constant", 1.0f);
//         lightingShader.setFloat("spotLight.linear", 0.09f);
//         lightingShader.setFloat("spotLight.quadratic", 0.032f);
//         lightingShader.setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
//         lightingShader.setFloat("spotLight.outerCutoff", glm::cos(glm::radians(15.0f)));



//         //glm::vec3 lightColor{};
//         //lightColor.x = static_cast<float>(sin(glfwGetTime() * 2.0f));
//         //lightColor.y = static_cast<float>(sin(glfwGetTime() * 0.7f));
//         //lightColor.z = static_cast<float>(sin(glfwGetTime() * 1.3f));

//         /*glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
//         glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);


//         lightingShader.setVec3("light.ambient", ambientColor);
//         lightingShader.setVec3("light.diffuse", diffuseColor);*/
//         //lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
//         //lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
//         //lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
//         //lightingShader.setVec3("light.position", camera.Position);
//         //lightingShader.setVec3("light.direction", camera.Front);
//         //lightingShader.setFloat("light.constant", 1.0f);
//         //lightingShader.setFloat("light.linear", 0.09f);
//         //lightingShader.setFloat("light.quadratic", 0.032f);
//         //lightingShader.setFloat("light.cutoff", glm::cos(glm::radians(12.5f)));
//         //lightingShader.setFloat("light.outerCutoff", glm::cos(glm::radians(17.5f)));

//         //lightingShader.setVec3("uLightPos", lightPos);

//         glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);
//         glm::mat4 view = camera.GetViewMatrix();
//         glm::mat4 model = glm::mat4(1.0f);

//         lightingShader.setMat4("projection", projection);
//         lightingShader.setMat4("view", view);
//         //lightingShader.setMat4("model", model);

//         // Bind to the diffuse map
//         glActiveTexture(GL_TEXTURE0);
//         glBindTexture(GL_TEXTURE_2D, diffuseMap);

//         // Bind Specular map
//         glActiveTexture(GL_TEXTURE1);
//         glBindTexture(GL_TEXTURE_2D, specularMap);
        
//         // Bind Emission Map
//         //glActiveTexture(GL_TEXTURE2);
//         //glBindTexture(GL_TEXTURE_2D, emissionMap);

//         glBindVertexArray(cubeVAO);

//         for (unsigned int i = 0; i < 10; i++)
//          {
//             glm::mat4 model = glm::mat4(1.0f);
//             model = glm::translate(model, cubePositions[i]);
//             float angle = 20.0f * i;
//             model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
//             lightingShader.setMat4("model", model);

//              glDrawArrays(GL_TRIANGLES, 0, 36);
//          }

//         //glBindVertexArray(cubeVAO);
//         //glDrawArrays(GL_TRIANGLES, 0, 36);


//         // Draw the lamp now
//         lightcubeShader.use();
//         lightcubeShader.setMat4("projection", projection);
//         lightcubeShader.setMat4("view", view);


//         // We now draw as many light bulbs as we have point lights
//         glBindVertexArray(lightVAO);
//         for (int i = 0; i < pointLightPositions->length() + 1; i++)
//         {
//             model = glm::mat4(1.0f);
//             model = glm::translate(model, pointLightPositions[i]);
//             model = glm::scale(model, glm::vec3(0.2f));
//             lightcubeShader.setMat4("model", model);
//             lightcubeShader.setVec3("uLightColor", glm::vec3(1.0f));
//             glDrawArrays(GL_TRIANGLES, 0, 36);
//         }

//         // Draw fonts
//         RenderText(textShader, "This is a sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5f, 0.8f, 0.2f));
//         RenderText(textShader, "Beep beep!", 25.0f, 100.0f, 1.0f, glm::vec3(0.5f, 0.8f, 0.2f));


//         //ourShader.use();

//         //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
//         //model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
//         //ourShader.setMat4("model", model);
//         //ourModel.Draw(ourShader);


//         /*float timeValue = glfwGetTime();
//         float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
//         int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
//         glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);*/


//         //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//         // Unbind from vertex array at the end

//         /* Swap front and back buffers */
//         glfwSwapBuffers(window);

//         /* Poll for and process events */
//         glfwPollEvents();
//     }

//     glDeleteVertexArrays(1, &cubeVAO);
//     glDeleteVertexArrays(1, &lightVAO);
//     glDeleteBuffers(1, &VBO);
//     //glDeleteBuffers(1, &EBO);

//     glfwTerminate();
//     return 0;
// }