#include <glad/glad.h> // be sure to include GLAD before other header requiring OpenGL
#include <GLFW/glfw3.h>
#include <iostream>

// Vertex shader
const char *vertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

int main()
{
    /* Send vertex data to GPU */
    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    // 1. Create memory on the GPU: where we want to store the vertex data
    // 2. Configure how OpenGL should interpret the memory
    // 3. Specify how to send the data to the graphics card
    // 'Memory' -> manage with Vertex Buffer Objects(VBO)
    unsigned int VBO;
    glGenBuffers(1, &VBO); // unique ID corresponding to that buffer, 

    // OpenGL allows us to bind several buffers at once -> as long as they have a different buffer type
    // buffer type of a vertex buffer object: GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // any buffer calls we make on the GL_ARRAY_BUFFER -> used to configure the currently bound buffer, VBO

    // Copies previously defined vertex data into the buffer's memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // function to copy user-defined data into the currently bound buffer

    // GL_STREAM_DRAW: data set only once, used at most a few times
    // GL_STATIC_DRAW: data set only once and used many times
    // GL_DYNAMIC_DRAW: data changed a lot and used many times -> graphics card will place the data in memory that allows for faster writes
    // ex. position data of the triangle does not change, used a lot, stays the same for every render call -> GL_STATIC_DRAW


    /* Compile vertex shader */
    // OpenGL dynamically compile shader at run-time from its source code
    unsigned int vertexShader;                                      // ID for shader object
    vertexShader = glCreateShader(GL_VERTEX_SHADER);                // provide type of shader we want to create
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);     // second argument: how many strings we are passing as source code, which is 1 for now
    glCompileShader(vertexShader);

    // * Checking if the shader compilation is successfull
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

