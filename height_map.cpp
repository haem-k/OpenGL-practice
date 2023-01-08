/*

* Rendering Terrain using Height Maps
- Height map: 
    grayscale image with the texel value 
    corresponding to the distance a vertex should be moved along its normal.

* CPU Implementation
- 


*/

#include <glad/glad.h> // be sure to include GLAD before other header requiring OpenGL
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shaders.h"
#include "camera.h"

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

// camera - give pretty starting point
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
Camera camera(glm::vec3(67.0f, 627.5f, 169.9f),
              glm::vec3(0.0f, 1.0f, 0.0f),
              -128.1f, -42.4f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

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
    // Height map
    int width, height, nChannels;
    unsigned char *data = stbi_load("./img/iceland_heightmap.png", &width, &height, &nChannels, 0);
    if(data)
    {
        // check if properly loaded?
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    // Generate a mesh that matched the resolution of our image
    // Populate each mesh vertex as follows
    std::vector<float> vertices;

    float yScale = 64.0f / 256.0f, yShift = 16.0f; // apply a scale+shift to the height data -> why???
    for(unsigned int i = 0; i < height ; i++)
    {
        for(unsigned int j=0; j<width ; j++)
        {
            // retrieve texel for (i, j) coord
            unsigned char* texel = data + (j + width * i) * nChannels;
            // raw height at coordinate
            unsigned char y = texel[0]; // grayscale, all values in the channels are same

            // store all coordinate in one array -> size of width * height * 3
            vertices.push_back(-(height/2.0f) + i);
            vertices.push_back((int)y * yScale - yShift);
            vertices.push_back(-(width/2.0f) + j);
        }
    }
    stbi_image_free(data); // good practice to free memory after reading information
    std::cout << "Loaded " << vertices.size() / 3 << " vertices" << std::endl;
    std::cout << vertices.size() << std::endl;

    // ! Why scale & shift y value?
    // y value from image -> within range of [0, 256]
    // Scale?
    //      - normalize the height map data to be within the range [0.0f, 1.0f]
    //      - scale it to the desired height we wish to work with
    // Shift?
    //      - translate the elevations to our final desired range, [-16.0f, 48.0f]


    // Element Buffer Object (EBO) to connect the vertices into triangles
    // alternate between row i and i+1 as we sweep across all columns j
    std::vector<unsigned int> indices;
    for(unsigned int i=0; i<height-1; i++)
    {
        for(unsigned int j=0; j<width ; j++)
        {
            for(unsigned int k=0; k<2; k++)
            {
                indices.push_back(j + width * (i + k));
            }
        }
    }

    // Two values need to know when rendering
    const unsigned int NUM_STRIPS = height - 1;
    const unsigned int NUM_VERTS_PER_STRIP = width * 2;
    // Each strip will be comprised of NUM_VERTS_PER_STRIP - 2 triangles
    // Full mesh will contain NUM_STRIPS * (NUM_VERTS_PER_STRIP - 2)

    GLuint terrainVAO, terrainVBO, terrainEBO;
    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO);

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float), // size of vertices buffer
                 &vertices[0],                    // pointer to first element 
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &terrainEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int), // size of indices buffer
                 &indices[0],                           // pointer to first element
                 GL_STATIC_DRAW);

    glBindVertexArray(terrainVAO);

    // Simple shader
    Shader ourShader("./height_shader.vs", "./height_shader.fs");

    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering commands here
        ourShader.use();
        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        ourShader.setMat4("model", model);
        for(unsigned int strip = 0; strip < NUM_STRIPS; ++strip)
        {
            // draw strip by strip
            glDrawElements(GL_TRIANGLE_STRIP,
                           NUM_VERTS_PER_STRIP,
                           GL_UNSIGNED_INT,
                           (void*)(sizeof(unsigned int)
                                        * NUM_VERTS_PER_STRIP
                                        * strip));
        }
        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // As soon as we exit the render loop,
    // properly clean / delete all of GLFW's resources that were allocated
    glfwTerminate();
    return 0;
}
