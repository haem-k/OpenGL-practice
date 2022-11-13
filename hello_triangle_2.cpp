#include <glad/glad.h> // be sure to include GLAD before other header requiring OpenGL
#include <GLFW/glfw3.h>
#include <iostream>

// when user resizes the window -> viewport adjusted
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// input control in GLFW
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
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
    unsigned int vertexShader;                                      // ID for shader object
    vertexShader = glCreateShader(GL_VERTEX_SHADER);                // provide type of shader we want to create
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);     // second argument: how many strings we are passing as source code, which is 1 for now
    glCompileShader(vertexShader);
    
    // Check for any errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
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
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // * Link the compiled shaders
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);                       // -> result: program object that we can activate by calling 'glUseProgram'
    // Check for errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ==================================================================================== //
    /* Send vertex data to GPU */
    float vertices[] =
    {
        // First triangle
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
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
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // any buffer calls we make on the GL_ARRAY_BUFFER -> used to configure the currently bound buffer, VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // function to copy user-defined data into the currently bound buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

   
    while(!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);  
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // 1st: to the front and back of all triangles?
        // 2nd: draw them in lines
        // Any subsequent drawing calls will render the triangles in wireframe mode until,
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        // We want to render triangles from an index buffer
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
        glfwPollEvents(); // Checks if any events are triggered, update the window state, calls functions(registered callback functions)
    }

    // As soon as we exit the render loop,
    // properly clean / delete all of GLFW's resources that were allocated
    glfwTerminate();
    return 0;



}

