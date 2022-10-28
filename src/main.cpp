#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "util.hpp"
#include "config.hpp"
#include "fps.hpp"
#include "controls.hpp"
#include "obj_file_parser.hpp"
#include "block.hpp"

GLFWwindow *window;
// The basic configuration for the program. 
Config config;

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
    window = glfwCreateWindow(config.windowWidth, config.windowHeight, "OpenGL Test", NULL, NULL);
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // set the clear color to be dark blue
    glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

    // Enable the z-buffer for the depth test
    glEnable(GL_DEPTH_TEST);
    // accept the fragment if it is close to the camera than the former one. This operation will be 
    // performed in every pixel drawn on the screen.
    glDepthFunc(GL_LESS);
    // Enable back face culling -> stop render the triangles that are behind the camera
    glEnable(GL_CULL_FACE);
    
    return EXIT_SUCCESS;
}

int main() {
    cout << "Program Start" << endl;

    if (GLInitialization() == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    // Use the VAO(vertex array objects) to send the data from VBO to the shaders. VAO also 
    // describes what type of data is contained within a VBO, and which shader variables the data 
    // should be sent to.
    GLuint vertArrayID;
    // Generate 1 vertex array object and store the result identifier in vertexArrayID
    glGenVertexArrays(1, &vertArrayID);
    // Bind the vertex array object with vertArrayID
    glBindVertexArray(vertArrayID);

    Fps fps;
    ObjFileParser parser;

    vector<glm::vec3> cubeVertices, cubeNormals;
    vector<glm::vec2> cubeUVs;
    parser.parseObjFile("./model/default_cube.obj", cubeVertices, cubeUVs, cubeNormals);

    vector<glm::vec3> teapotVertices, teapotNormals;
    vector<glm::vec2> teapotUVs;
    parser.parseObjFile("./model/teapot.obj", teapotVertices, teapotUVs, teapotNormals);

    // randomly generate the colors for the triangles
    vector<GLfloat> teapotColors(teapotVertices.size() * 3);
    for (int i = 0; i < teapotColors.size(); i++) {
        teapotColors[i] = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX);
    }
    vector<GLfloat> cubeColors(cubeVertices.size() * 3);
    for (int i = 0; i < cubeColors.size(); i++) {
        cubeColors[i] = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX);
    }

    // Use the VBO(vertex buffer object) to send the triangle coordinate data to the gpu memory 
    GLuint teapotVertBuffer = 0, teapotColorBuffer = 0;
    vboInit(teapotVertBuffer, teapotColorBuffer, teapotVertices, teapotColors);

    GLuint cubeVertBuffer = 0, cubeColorBuffer = 0;
    vboInit(cubeVertBuffer, cubeColorBuffer, cubeVertices, cubeColors);


    // Create and compile the GLSL program from the shaders
    GLuint programID = shaderLoader("./shader/VertexShader.vert", "./shader/FragmentShader.frag");
    // Get a handle for our "mvp" uniform. The variable "mvp" refers to the mvp variable in the 
    // vertex shader
    GLuint matrixID = glGetUniformLocation(programID, "mvp");
    glm::mat4 projMatrix, camMatrix, modelMatrix, prevMvp, currMvp, interpolatedMvp;
    
    // The physics simulation update rate, 30 times per second cuz I have a potato computer
    const double dt = 1.0 / 30.0;
    // The current elapsed time of the program, in seconds.
    double currTime = glfwGetTime();
    // Record how much time has elapsed since last update.
    double accumulator = 0;
    // The time of the previous logic update
    double prevLogicTime = currTime;
    // Keeps track the number of updates of the render and logic
    unsigned int fpsUpdateCount = 0, logicUpdateCount = 0;

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Specify the GLSL shader program that we want to run
        glUseProgram(programID);

        // -----------------------------------------------------------------------------------------
        // Time step magic, ensure physics works consistent across different systems.
        // -----------------------------------------------------------------------------------------
        double newTime = glfwGetTime();
        double frameTime = newTime - currTime;
        currTime = newTime;
        accumulator += frameTime;
        // Once we go over the threashold, we shall update the logic
        while (accumulator >= dt) {
            computeMatricesFromInputs(window, (float)dt, config.inputMode);
            // The projection matrix with 45˚ fov; window ratio, display range 0.1 unit to 100 units
            projMatrix = getProjMatrix();
            // The camera matrix that we want to rotate the entire world
            camMatrix = getViewMatrix();
            // This means our model will be at the origin and no transformations
            modelMatrix = glm::mat4(1.0f);
            // Our final ModelViewProjection matrix is the multiplication of the three matrices
            // Note that for matrices multiplication we first perform the right most operation then 
            // work our way towards the left
            prevMvp = currMvp;
            currMvp = projMatrix * camMatrix * modelMatrix;
            accumulator -= dt;
            logicUpdateCount++;
            prevLogicTime = glfwGetTime();
        }
        // calculate the interpolated mvp between the previous and the next frame to avoid jitter
        interpolatedMvp = prevMvp + (currMvp - prevMvp) * float((newTime - prevLogicTime) / dt);
        
        // Send the transformation matrix to the currently bound shader, in the type "mvp" uniform
        // This is done in the main loop because each model will have a difference mvp matrix (at 
        // least for the "m" part)
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(interpolatedMvp));

        // -----------------------------------------------------------------------------------------
        // Draws the model
        // -----------------------------------------------------------------------------------------
        renderModel(cubeVertBuffer, cubeColorBuffer, cubeVertices);
        renderModel(teapotVertBuffer, teapotColorBuffer, teapotVertices);

        fps.update();
        // if (prevFps != fps.getFps()) {
            // cout << "fps: " << fps.getFps() << endl;
        //     prevFps = fps.getFps();
        // }
        fpsUpdateCount++;
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    // Cleanup the VBO and shader
    glDeleteBuffers(1, &teapotVertBuffer);
    glDeleteBuffers(1, &teapotColorBuffer);
    glDeleteBuffers(1, &cubeVertBuffer);
    glDeleteBuffers(1, &cubeColorBuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &vertArrayID);
    
    cout << "..Finished" << endl;
    cout << "Average updates per second: " << 
            "\n    Fps: " << ((double)fpsUpdateCount) / glfwGetTime() << 
            "\n    Logic: " << ((double)logicUpdateCount) / glfwGetTime() << 
            endl;

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return EXIT_SUCCESS;
}