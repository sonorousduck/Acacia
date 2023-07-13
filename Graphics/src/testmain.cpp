#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include "camera.hpp"
#include <asset_load.hpp>
#include <texture_asset.hpp>
#include <filesystem>
#include <chrono>
#include <map>
#include "model.hpp"
#include "graphics2d.hpp"
#include "resourceManager.hpp"

#include "particleGenerator.hpp"
#include "colors.hpp"

#include <ft2build.h>
#include "input.hpp"
#include FT_FREETYPE_H

unsigned int WIDTH = 800;
unsigned int HEIGHT = 600;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float fpsUpdateDeltaTime = 0.0f;
std::string fps = "";
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;

bool firstMouse = true;

unsigned int textVAO = 0, textVBO = 0;


struct Character
{
    unsigned int TextureID;     // ID handle of the glyph texture
    glm::ivec2   Size;          // Size of the glyph
    glm::ivec2   Bearing;       // Offset from baseline to left/top of glyph
    unsigned int Advance;       // Offset to advance the next glyph
};

std::map<char, Character> Characters{};


void RenderText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color)
{
    s.use();
    s.setVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVAO);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        // Update VBO for each character
        float vertices[6][4] = {
            { xpos,         ypos + h,   0.0f, 0.0f  },
            { xpos,         ypos,       0.0f, 1.0f  },
            { xpos + w,     ypos,       1.0f, 1.0f  },
            { xpos,         ypos + h,   0.0f, 0.0f  },
            { xpos + w,     ypos,       1.0f, 1.0f  },
            { xpos + w,     ypos + h,   1.0f, 0.0f  }
        };
    
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render Quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        // Now advance cursors for next glyph (Note that advance number is 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}



int main()
{

    Ebony::Graphics2d graphics{};


    graphics.Initialize("Ebony", 800, 600);
    graphics.SetMainCamera(camera);

    glEnable(GL_DEPTH_TEST);


    Shader& textShader = Ebony::ResourceManager::LoadShader("shaders/font.vert", "shaders/font.frag", "text");
    

    // Load font
    // =============================================================================================
    FT_Library ft{};
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "Error: Freetype: Could not init FreeType Library" << std::endl;
        return -1;
    }

    FT_Face face{};
    if (FT_New_Face(ft, "fonts/super-indie-font/SuperIndie.ttf", 0, &face))
    {
        std::cout << "Error: Freetype: Failed to load the font" << std::endl;
        return -1;
    }

    FT_Set_Pixel_Sizes(face, 0, 24);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

    // We are going to loop through 128 characters of the ASCII set
    for (unsigned char c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "Error: Freetype: Failed to load Glyph (In particular, glyph: " << c << std::endl;
            continue;
        }

        // Generate texture
        unsigned int texture = 0;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 textProjection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    textShader.use();
    textShader.setMat4("projection", textProjection);

    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW); // It needs 6 vertices of 4 floats each, so 6 * 4 floats of memory (x, y, u, v)
    
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Texture2D& faceTexture = Ebony::ResourceManager::LoadTexture("textures/awesomeface.tx", "face");
    Shader& s = Ebony::ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", "default");

    //Texture2D testTexture = graphics.loadTexture("textures/awesomeface.tx");

    //Shader particleShader("shaders/particle.vert", "shaders/particle.frag");


    Ebony::Color clearColor = Ebony::Color::Color(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    s.use();
    s.setInt("image", 0);
    s.setMat4("projection", graphics.projection);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(graphics.window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        fpsUpdateDeltaTime -= deltaTime;

        graphics.input.ProcessInput(graphics.window, graphics.mainCamera, deltaTime);

        graphics.BeginDraw(clearColor);


        graphics.Draw(faceTexture, glm::vec2(200.0f, 0.0f), glm::vec2(300.0f, 400.0f), 45.0f, Ebony::Colors::Red);



        if (fpsUpdateDeltaTime <= 0.0f) 
        {
            fps = std::to_string(static_cast<int>(std::round(1000 / deltaTime))) + " fps";
            fpsUpdateDeltaTime += 1.0f;
            // Draw fonts
        }

        RenderText(textShader, fps, 25.0f, 25.0f, 1.0f, Ebony::Colors::Green.GetRGB());
        RenderText(textShader, "Beep beep!", 25.0f, 100.0f, 1.0f, glm::vec3(0.5f, 0.8f, 0.2f));

       

        graphics.EndDraw();
        


        /* Poll for and process events */
        glfwPollEvents();
    }

    //glDeleteVertexArrays(1, &cubeVAO);
    //glDeleteVertexArrays(1, &planeVAO);
    //glDeleteVertexArrays(1, &quadVAO);
   // glDeleteBuffers(1, &cubeVBO);
    //glDeleteFramebuffers(1, &quadVBO);
    //glDeleteFramebuffers(1, &planeVBO);
    graphics.Cleanup();
    //glDeleteRenderbuffers(1, &rbo);
    //glDeleteFramebuffers(1, &framebuffer);

    glfwTerminate();
    return 0;
}