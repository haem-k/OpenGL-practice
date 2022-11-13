#include <glad/glad.h> // be sure to include GLAD before other header requiring OpenGL
#include <GLFW/glfw3.h>
#include <iostream>

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
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
const char *fragmentShaderSource2 =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
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
    // We register the callback functions after we have created the window and before the render loop is initiated
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD manages function pointers for OpenGL
    // -> initialize GLAD before we call any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // pass the address of the OpenGL function pointers (given by OpenGL)
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ==================================================================================== //
    // * Compile vertex shader
    unsigned int vertexShader;                                  // ID for shader object
    vertexShader = glCreateShader(GL_VERTEX_SHADER);            // provide type of shader we want to create
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // second argument: how many strings we are passing as source code, which is 1 for now
    glCompileShader(vertexShader);

    // Check for any errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // * Compile fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // * Another fragment shader
    unsigned int fragmentShader2;
    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader2);
    // Check for errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT2::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // * Link the compiled shaders
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram); // -> result: program object that we can activate by calling 'glUseProgram'
    // Check for errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    // glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int shaderProgram2;
    shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2); // -> result: program object that we can activate by calling 'glUseProgram'
    // Check for errors
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader2);

    // ==================================================================================== //
    float vertices_1[] =
        {
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f};
    unsigned int indices_1[] =
        {
            0, 1, 2};

    float vertices_2[] =
        {
            0.7f, 0.6f, 0.0f,
            0.5f, 0.3f, 0.0f,
            0.1f, 0.6f, 0.0f};
    unsigned int indices_2[] =
        {
            0, 1, 2};

    // unsigned int VBO, VAO, EBO;
    GLuint vbo[2];
    GLuint vao[2];
    glGenVertexArrays(2, vao);
    glGenBuffers(2, vbo);
    // glGenBuffers(1, &EBO);

    // First
    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);                                         // any buffer calls we make on the GL_ARRAY_BUFFER -> used to configure the currently bound buffer, VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW); // function to copy user-defined data into the currently bound buffer
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0); // no need to unbind, since we bind another VAO in the next lines

    // Second
    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_2), vertices_2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0); // not really necessary, but beware of calls that could affect VAOs while this one is bound

    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram2);
        glBindVertexArray(vao[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // 1st: to the front and back of all triangles?
        // 2nd: draw them in lines
        // Any subsequent drawing calls will render the triangles in wireframe mode until,
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // We want to render triangles from an index buffer
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // 2nd argument: count for number of elements
        // 3rd argument: type of indices
        // 4th argument: offset in the EBO
        // Takes indices from the EBO currently bound to the GL_ELEMENT_ARRAY_BUFFER target
        // -> Bind corresponding EBO each time we want to render an object with indices
        // -> cumbersome! use VAO

        // Last element buffer object that gets bound while a VAO is bound,
        // is stored as the VAO's element buffer object
        // -> Binding to a VAO, also automatically binds that EBO

        // VAO also stores the unbind calls
        // -> make sure you don't unbind the element array buffer before unbinding your VAO
        // otherwise it doesn't have an EBO configured

        // Check and call events and swap the buffers
        glfwSwapBuffers(window); // Swap the color buffer used during this render iteration and show it as output to the screen
        glfwPollEvents();        // Checks if any events are triggered, update the window state, calls functions(registered callback functions)
    }

    // Optional: de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(2, vao);
    glDeleteBuffers(2, vbo);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderProgram2);

    // As soon as we exit the render loop,
    // properly clean / delete all of GLFW's resources that were allocated
    glfwTerminate();
    return 0;
}
