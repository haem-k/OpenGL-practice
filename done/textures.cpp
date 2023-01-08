/*
* Texture
- Each vertex should have a texture coordinate
: what part of the texture image to sample from

- For triangle, 
- 3 texture coordinates -> vertex shader -> fragment shader

- fragment shader neatly interpolates all the texture coordinates for each fragment

* Texture Wrapping
- What happens if we specify coordinates outside this range? (0, 0) ~ (1, 1)
- Default: repeat, but more options -> can be set per coordinate axis
- Texture coordinate axis (s, t) + r if using 3D textures

* Texture Filtering
- Texture coordinates do not depend on resolution, can be any floating point value
-> OpenGL has to figure out which texture pixel to map the texture coordinate into
-> Texture filtering!
- GL_NEAREST:   select the texel that center is closest to the texture coordinate.
- GL_LINEAR:    takes interpolated values from the texture coordinate's neighboring texels.

- Used for magnifying & minifying operations
- Set filtering methods via glTexParameter (GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER)

* Mipmaps
- Object far away, smaller resolution
- OpenGL function: glGenerateMipMap
- Possible to filter between mipmap levels using NEAREST and LINEAR filtering for switching between mipmap levels
- Separate option names for filtering between the mipmaps

! Common mistake !
- To set one of the mipmap filtering options as magnification filter.
-> no effect since mipmaps are primarily used for when textures get DOWNSCALED
- Texture magnification doesn't use mipmaps 
*/

#include <glad/glad.h> // be sure to include GLAD before other header requiring OpenGL
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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
    
    /* Send vertex data to GPU */
    float vertices[] =
    {
        // position             // colors               // texture coords
        0.5f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f,       1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,       1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,       0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f,      1.0f, 1.0f, 0.0f,       0.0f, 1.0f  // top left
    };

    unsigned int indices[] =
    {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // * Texture
    unsigned int texture;
    glGenTextures(1, &texture);
        // - how many textures we want to generate
        // - store in unsigned int array (in this case, single int)
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("container.jpeg", &width, &height, &nrChannels, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // Now currently bound texture object new has the texture image attached to it.
        // only base-level of the texture image
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data); // good practice to free memory after generating texture

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO); // multiple triangles with indices -> need Element Buffer Objects!!

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                        // any buffer calls we make on the GL_ARRAY_BUFFER -> used to configure the currently bound buffer, VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // function to copy user-defined data into the currently bound buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // vertex pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0); // enable two different attributes
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // As soon as we exit the render loop,
    // properly clean / delete all of GLFW's resources that were allocated
    glfwTerminate();
    return 0;
}
