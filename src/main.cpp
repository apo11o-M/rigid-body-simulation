#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// Include GLEW Library
#include <GL/glew.h>

// Include GLFW Library
#include <GLFW/glfw3.h>

// Include GLM Library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <util.hpp>
#include "config.hpp"

GLFWwindow *window;
using namespace glm;
using namespace config;
using std::cout;
using std::endl;

int main() {
    cout << "Program Start" << endl;

    // Initialize GLFW
    cout << "Initializing GLFW...";
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return EXIT_FAILURE;
    }
    cout << " Success" << endl;

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // OpenGL 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // said to make MacOS happy, not sure how
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(config::windowWidth, config::windowHeight, "OpenGL Test", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.");
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    // Note the OpenGL context need to be created before initializing glew or else it will segfault
    cout << "Initializing GLEW...";
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initizliae GLEW");
        glfwTerminate();
        return EXIT_FAILURE;
    }
    cout << " Success" << endl;

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // set the clear color to be dark blue
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Use the VAO(vertex array objects) to send the data from VBO to the shaders. VAO also 
    // describes what type of data is contained within a VBO, and which shader variables the data 
    // should be sent to.
    GLuint vertArrayID;
    // Generate 1 vertex array object and store the result identifier in vertexArrayID
    glGenVertexArrays(1, &vertArrayID);
    // Bind the vertex array object with vertArrayID
    glBindVertexArray(vertArrayID);

    // Create and compile the GLSL program from the shaders
    GLuint programID = LoadShaders("./shader/VertexShader.vert", "./shader/FragmentShader.frag");

    // The projection matrix with 45˚ fov; window ratio, display range 0.1 unit <-> 100 units
    glm::mat4 projMatrix = glm::perspective(
                                glm::radians(45.0f),
                                ((float)windowWidth) / ((float)windowHeight), 
                                0.1f, 
                                100.0f
                            );

    // The camera matrix that we want to rotate the entire world
    glm::mat4 camMatrix = glm::lookAt( glm::vec3(10, 5, 9),  // Camera position, in world space
                                       glm::vec3(0, 0, 0),  // Camera's direction, looking at origin
                                       glm::vec3(0, 1, 0)   // Head is up, (0,-1,0) for upside-down
                          );

    // This means our model will be at the origin and no transformations
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    // Our final ModelViewProjection matrix is the multiplication of the three matrices
    // Note that for matrices multiplication we first perform the right most operation then work our
    // way towards the left
    glm::mat4 mvp = projMatrix * camMatrix * modelMatrix;

    // Get a handle for our "mvp" uniform. The variable "mvp" refers to the mvp variable in the 
    // vertex shader
    GLuint matrixID = glGetUniformLocation(programID, "mvp");

    // The vertices of our 2D triangle, lies on the z-plane
    static const GLfloat triangle_vertices[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f
    };

    // Use the VBO(vertex buffer object) to send the triangle coordinate data to the gpu memory 
    GLuint vertBuffer;
    // Generate 1 vertex buffer object and put the result identifer in the vertBuffer
    glGenBuffers(1, &vertBuffer);
    // Specify the target of the buffer object. In this case vertBuffer is bound to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
    // Populate the GL_ARRAY_BUFFER with the vertices of our triangle
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

    do {
        glClear(GL_COLOR_BUFFER_BIT);
        // Specify the GLSL shader program that we want to run
        glUseProgram(programID);
        // Send the transformation matrix to the currently bound shader, in the type "mvp" uniform
        // This is done in the main loop because each model will have a difference mvp matrix (at 
        // least for the "m" part)
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(mvp));
        // Draw stuff below here
        glEnableVertexAttribArray(0);
        // Specify which array we want to draw. In this case we want to draw the vertex buffer obj 
        glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
        glVertexAttribPointer(
            0,          // attribute 0, match the layout in the shader
            3,          // size
            GL_FLOAT,   // type
            GL_FALSE,   // normalized?
            0,          // stride
            (void*)0    // array buffer offset
        );
        // Actually draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3); // startfrom vertex 0; 3 vertices in total -> 1 triangle
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    // Cleanup the VBO and shader
    glDeleteBuffers(1, &vertBuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &vertArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    cout << "..Finished" << endl;
    return EXIT_SUCCESS;
}