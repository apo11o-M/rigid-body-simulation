#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// Include GLEW Library
#include <GL/glew.h>

// Include GLFW Library
#include <GLFW/glfw3.h>

// Include GLM Library
#include <glm/glm.hpp>

#include <shader.hpp>

#include "config.hpp"

GLFWwindow *window;
using namespace glm;
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
    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Create a Vertex Array Object
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile the GLSL program from the shaders
    GLuint programID = LoadShaders("./shader/VertexShader.vert", "./shader/FragmentShader.frag");
    
    // The vertices of our 2D triangle, lies on the z-plane
    static const GLfloat triangle_vertices[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f
    };

    GLuint vertexbuffer;
    // Generate 1 buffer and put the resulting identifer in the vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The follwoing commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

    do {
        // Clear the screen as it might cause flickering
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(programID);

        // Draw stuff below here
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
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

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    cout << "..Finished" << endl;
    return EXIT_SUCCESS;
}