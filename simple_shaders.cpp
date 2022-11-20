/*
* Shaders
- little program rests on GPU
- isolated programs, they are not allowed to communicate with each other (only input and output)

* GLSL
- C-like language
- specifically targeted at vector and matrix manipulation

in vertex shader,
each input variable "vertex attribute"

Define how the vertex data is organized
- specify the input variables with location metadata
-> configure the vertex attributes on the CPU

in fragment shader,
requires vec4 color output variable
(need to generate a final output color)

Send data from one shader to another
sending shader output
receiveing shader input -> similar
- types & names equal,
- OpenGL will link variables (when linking a program object)

* Uniforms
- another way to pass data CPU (application) -> GPU (shader)
- GLOBAL !
    - unique per shader program object
    - can be accessed from any shader at any stage in the shader program
    - keep the value until they're reset or updated
! Caution
    - if you declare a uniform that isnt used anywhere in your GLSL code,
    - the compiler will silently remove the variable from the compiled version
    -> the cause for frustrating errors
- useful tool for setting attributes that may change every frame,
- or interchanging data between your application and your shaders
? How uniform values are sent to GPU? If it is updated every frame, does it degrade the performance?

OpenGL - C library in its core
- no native support for function overloading
- wherever a function can be called with different types -> defines new functions for each type required
- ex) glUniform: requires a specific postfix for the type of the uniform you want to set

* glVertexArribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)))
- index
- number of components in a single attr. (1/2/3/4)
- type
- normalize? : clamp to [-1, 1] / [0, 1]
- stride: byte offset of consecutive attribute
- pointer: offset of the first component in the first attribute in the array

Why colors are interpolated?
- result of fragment interpolation! (in the fragment shader)

* During rasterization
- results in a lot more fragments than vertices originally specified
? determines the positions of each of those fragments based on where they reside on the triangle shape
- based on the positions, interpolates all the fragment shader's input variables
*/

#include <glad/glad.h> // be sure to include GLAD before other header requiring OpenGL
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "shaders.h"

// when user resizes the window -> viewport adjusted
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// input control in GLFW
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Vertex shader
const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";
// const char *fragmentShaderSource =
//     "#version 330 core\n"
//     "out vec4 FragColor;\n"
//     "in vec4 vertexColor;\n"
//     "void main()\n"
//     "{\n"
//     "   FragColor = vertexColor;\n"
//     "}\n\0";
const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0);\n"
    "}\n\0";

int main()
{
    // ==================================================================================== //
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // after window creation, before render function

    // GLAD manages function pointers for OpenGL
    // -> initialize GLAD before we call any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // pass the address of the OpenGL function pointers (given by OpenGL)
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ==================================================================================== //
    // * Use Shader class
    Shader ourShader("./shader.vs", "./shader.fs");
    
    // // * Compile vertex shader
    // unsigned int vertexShader;                                  // ID for shader object
    // vertexShader = glCreateShader(GL_VERTEX_SHADER);            // provide type of shader we want to create
    // glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // second argument: how many strings we are passing as source code, which is 1 for now
    // glCompileShader(vertexShader);

    // // Check for any errors
    // int success;
    // char infoLog[512];
    // glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    // if (!success)
    // {
    //     glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    //     std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
    //               << infoLog << std::endl;
    // }

    // // * Compile fragment shader
    // unsigned int fragmentShader;
    // fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // glCompileShader(fragmentShader);
    // // Check for errors
    // glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    // if (!success)
    // {
    //     glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    //     std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
    //               << infoLog << std::endl;
    // }

    // // * Link the compiled shaders
    // unsigned int shaderProgram;
    // shaderProgram = glCreateProgram();
    // glAttachShader(shaderProgram, vertexShader);
    // glAttachShader(shaderProgram, fragmentShader);
    // glLinkProgram(shaderProgram); // -> result: program object that we can activate by calling 'glUseProgram'
    // // Check for errors
    // glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    // if (!success)
    // {
    //     glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    //     std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
    //               << infoLog << std::endl;
    // }
    // glDeleteShader(vertexShader);
    // glDeleteShader(fragmentShader);

    // ==================================================================================== //
    /* Send vertex data to GPU */
    float vertices[] =
    {
        // position             // colors
        0.0f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,
    };

    unsigned int indices[] =
    {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                        // any buffer calls we make on the GL_ARRAY_BUFFER -> used to configure the currently bound buffer, VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // function to copy user-defined data into the currently bound buffer
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0); // enable two different attributes

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // * use uniform
        ourShader.use();
        float timeValue = glfwGetTime();
        float offsetValue = sin(timeValue);
        int vertexOffsetLocation = glGetUniformLocation(ourShader.ID, "offset"); // does not require glUseProgram
        // glUseProgram(shaderProgram);
        glUniform3f(vertexOffsetLocation, offsetValue, 0.0f, 0.0f); // require glUseProgram, set uniform to currently active program
        // ourShader.setFloat("offset", offsetValue);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // We want to render triangles from an index buffer
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // As soon as we exit the render loop,
    // properly clean / delete all of GLFW's resources that were allocated
    glfwTerminate();
    return 0;
}
