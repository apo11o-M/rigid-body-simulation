#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glfreetype/TextRenderer.hpp>

#include "util.hpp"
#include "config.hpp"
#include "fps.hpp"

GLFWwindow *window;
// glfreetype::font_data font;
using namespace glm;
using std::cout;
using std::endl;

int GLInitialization() {
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

    // font.init("/fonts/Arial.ttf", 25);

    cout << " Success" << endl;
    return EXIT_SUCCESS;
}

int main() {
    cout << "Program Start" << endl;

    if (GLInitialization() == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // set the clear color to be dark blue
    glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

    // Enable the z-buffer for the depth test
    glEnable(GL_DEPTH_TEST);
    // accept the fragment if it is close to the camera than the former one. This operation will be 
    // performed in every pixel drawn on the screen.
    glDepthFunc(GL_LESS);

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
                                ((float)config::windowWidth) / ((float)config::windowHeight), 
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

    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a 
    // triangle. A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 
    // vertices
    static const GLfloat cube_vertices[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };

    // One color for each vertex. They were generated randomly.
    static const GLfloat cube_colors[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };

    // Use the VBO(vertex buffer object) to send the triangle coordinate data to the gpu memory 
    GLuint vertBuffer;
    // Generate 1 vertex buffer object and put the result identifer in the vertBuffer
    glGenBuffers(1, &vertBuffer);
    // Specify the target of the buffer object. In this case vertBuffer is bound to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
    // Populate the GL_ARRAY_BUFFER with the vertices of our triangle
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);

    Fps fps;

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Specify the GLSL shader program that we want to run
        glUseProgram(programID);
        // Send the transformation matrix to the currently bound shader, in the type "mvp" uniform
        // This is done in the main loop because each model will have a difference mvp matrix (at 
        // least for the "m" part)
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(mvp));

        // 1st attribute buffer: vertices
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

        // 2nd attribute buffer: colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glVertexAttribPointer(
            1,          // attribute. match the layout in the shader
            3,          // size
            GL_FLOAT,   // type
            GL_FALSE,   // normalized?
            0,          // stride
            (void*)0    // array buffer offset
        );

        // Actually draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // startfrom vertex 0, 12 * 3 because 12 triangles
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        
        fps.update();
        cout << "fps: " << fps.getFps() << endl;
        // glPushMatrix();
        // glColor3ub(0, 0, 0xFF);
        // glfreetype::print(font, 20, 20, "Hello World");
        // glPopMatrix();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    // Cleanup the VBO and shader
    glDeleteBuffers(1, &vertBuffer);
    glDeleteBuffers(1, &colorBuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &vertArrayID);

    // font.clean();

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    cout << "..Finished" << endl;
    return EXIT_SUCCESS;
}