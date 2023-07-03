#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void checkSuccessfulShaderCompilation(const unsigned int& shader)
{
    int success = 0;
    char infoLog[512]{};

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR! Shader compilation failed\n" << infoLog << std::endl;
    }
}

void checkSuccessfulShaderProgramLinking(const unsigned int& program)
{
    int success = 0;
    char infoLog[512]{};

    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR! Shader program linking failed\n" << infoLog << std::endl;
    }
}


const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// For now, include this here
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";;


int main()
{
    GLFWwindow* window{};


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Ebony", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // This sets the callback to change the viewport. This is also how you would record input from glfw as well
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    

    // ============================================
    // Compile shaders

    unsigned int vertexShader = 0;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    checkSuccessfulShaderCompilation(vertexShader);

    unsigned int fragmentShader = 0;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    checkSuccessfulShaderCompilation(fragmentShader);

    // ============================================

    // ============================================
    // Create shader program and attach compiled shaders

    unsigned int shaderProgram = 0;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    checkSuccessfulShaderProgramLinking(shaderProgram);

    // ============================================

    glUseProgram(shaderProgram);

    // Since the shaders have been linked into a program, you don't need them anymore. (Maybe keep these in a smart queue for reuse? Free them after a certain
    // number of shaders have entered and it hasn't been used in a while?
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };

    //unsigned int indices[] = {  // note that we start from 0!
    //    0, 1, 3,   // first triangle
    //    1, 2, 3    // second triangle
    //};

    // ============================================
    // Create a VBO - Vertex Buffer Object - which holds a large number of vertices in the GPU's memory

    unsigned int VBO = 0;
    glGenBuffers(1, &VBO);

    // ============================================

    // ============================================
    // Create a VAO - Vertex Array Object - which holds configurations
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);

    // Bind to the vertex array object before binding buffer and data
    glBindVertexArray(VAO);
    // ============================================

    // ============================================
    // Create EBO - Element Buffer Object
    //unsigned int EBO = 0;
    //glGenBuffers(1, &EBO);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // ============================================

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // 1st parameter: Specifies the location (i.e. in the vertex shader we said layout(position = 0)
    // 2nd parameter: size of vertex attribute. Since it is a vec3, 3 goes here
    // 5th parameter: Stride. So this has a stride of 3 vertices (since 1 vertex is 1 float)
    // 6th parameter: This is the offset of where the position data begins in the buffer
    // Position Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ============================================
    // To draw in wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // ============================================
    
    glUseProgram(shaderProgram);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        /* Render here */
        glClearColor(0.4f, 0.58f, 0.93f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        /*float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);*/

        // You don't need to bind to the EBO either because the VAO stores that as well.
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);       
        // Unbind from vertex array at the end
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}