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
const char *fragmentShaderSource = 
    "#version 330 core\n"
    "out vec4 FragColor;"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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


    // ==================================================================================== //
    // * Compile vertex shader
    // OpenGL dynamically compile shader at run-time from its source code
    unsigned int vertexShader;                                      // ID for shader object
    vertexShader = glCreateShader(GL_VERTEX_SHADER);                // provide type of shader we want to create
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);     // second argument: how many strings we are passing as source code, which is 1 for now
    glCompileShader(vertexShader);

    // Checking if the shader compilation is successfull
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // * Compile fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // * Link the compiled shaders
    // To use the recently compiled shaders, LINK them and ACTIVATE the shader program when rendering objects
    // activated shader programs's shaders will be used when we issue render calls

    // Linking
    // - links the output of each shader to the inputs of the next shader
    // - errors if the output and input does not match
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);                       // -> result: program object that we can activate by calling 'glUseProgram'
    glUseProgram(shaderProgram);                        // Every shader and rendering call after this line will use this program object
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);                     // Have to delete after making a program. will not be used

    // ==================================================================================== //
    // ! OpenGL does not yet know 
    // - how it should INTERPRET the vertex data in memory
    // - how it should CONNECT the vertex data to the vertex shader's attributes

    // Tell OpenGL how it should interpret the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);   // enable the vertex attribute, giving the vertex attribute location as argument. it is disabled by default.

    // * glVertexAttribPointer
    // 1 - which vertex attribute we want to configure, specified the location 'layout (location=0)'
    // 2 - size of the attribute, vec3
    // 3 - data type
    // 4 - do we want our data to be normalized?, when the data is INT this normalized data to 0 / 1 if converted to float
    // 5 - stride: the space between consecutive vertex attributes
    // 6 - offset: where the position data begins in the buffer

    // ==================================================================================== //
    // Bind buffer, set attibute, enable them, use program, draw the object
    // have to repeat everytime we draw something..
    // -> Store all states into an object & simply bind this object to restore its state?

    // * Vertex Array Object (VAO)
    // VBO + attribute pointers 
    // Once bound, 
    // Any subsequent vertex attribute calls from that point on will be stored inside the VAO
    // when configuring vertex attrib pointers, make calls once & just bind the corresponding VAO
    //  - refer to the image in the website WOW

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // 1 - primitive type
    // 2 - starting index of vertex array
    // 3 - number of vertices

}

