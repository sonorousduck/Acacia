#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include "shader.hpp"
#include <cmath>

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
Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));
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
    glUniform3f(glGetUniformLocation(s.ID, "textColor"), color.x, color.y, color.z);
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
    Ebony::Input input{};


    graphics.Initialize("Ebony", 800, 600);
    

    glEnable(GL_DEPTH_TEST);


    Shader textShader("shaders/font.vert", "shaders/font.frag");
    Shader shader("shaders/framebuffers.vert", "shaders/framebuffers.frag");
    Shader screenShader("shaders/screenTexture.vert", "shaders/screenTexture.frag");

    
    shader.use();
    shader.setInt("texture1", 0);

    screenShader.use();
    screenShader.setInt("screenTexture", 0);


    // Create a framebuffer object
// ============================================================================================
    unsigned int framebuffer = 0;

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);


    // Create texture attachment
    unsigned int textureColorBuffer = 0;
    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

    // Not going to be reading from the depth or stencil, so we can use an RBO (Render buffer object) which doesn't support reading
    unsigned int rbo = 0;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // This attaches the rbo to the fbo

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
            face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 textProjection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    textShader.use();
    glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(textProjection));

    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW); // It needs 6 vertices of 4 floats each, so 6 * 4 floats of memory (x, y, u, v)
    
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    Texture2D testTexture = graphics.loadTexture("textures/box.tx");

    Shader particleShader("shaders/particle.vert", "shaders/particle.frag");


    Ebony::Color clearColor = Ebony::Color::Color(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_DEPTH_TEST);

    graphics.defaultShader.use();
    graphics.defaultShader.setInt("image", 0);
    graphics.defaultShader.setMat4("projection", graphics.projection);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(graphics.window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        graphics.input.ProcessInput(graphics.window, camera, deltaTime);

        /* Render here */
       
            // bind to framebuffer and draw scene as we normally would to color texture 
        //glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
       // glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

        //Ebony::Color clearColor = Ebony::Color(0.1f, 0.1f, 0.1f, 1.0f);
        
        graphics.BeginDraw(clearColor);


        graphics.Draw(testTexture, glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));



        std::string fps = std::to_string(static_cast<int>(std::round(1000 / deltaTime))) + " fps";

        // Draw fonts
        RenderText(textShader, fps, 25.0f, 25.0f, 1.0f, glm::vec3(0.5f, 0.8f, 0.2f));
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
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &framebuffer);

    glfwTerminate();
    return 0;
}