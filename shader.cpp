

/* Vertex Shader */
// Write a vertex shader in the shader language GLSL,
// Compile the shader to use in our application

/*
// Each shader begins with a declaration of its version
// + explicitly mention that we are using core profile functionality
#version 330 core

// Declare all the input vertex attributes: 'in' keyword
// in this case, we only care about position info. -> single vertex attribute
// ? GLSL has a vector datatype that contains 1 to 4 float based on its 'postfix' digit?
    // for vec4, w value needed for 'Perspective division'
// Set the location of the input variable
layout (location = 0) in vec3 aPos;

void main()
{
    // Set output of the shader
    // assign the position data to the predefined gl_Position (vec4)
    // whatever we set gl_Position to will be used as the output of the vertex shader
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

// -> no processing at all, the most simple vertex shader
*/

/* Fragment Shader */
// All about calculating the color output of the pixels!
// #version 330 core
// out vec4 FragColor;

// void main()
// {
//     FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
// }
// Fragment shader only one output -> color, vec4
