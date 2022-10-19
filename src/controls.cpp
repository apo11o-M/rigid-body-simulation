
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace glm;

#include "controls.hpp"
#include "config.hpp"

extern Config config;
glm::mat4 viewMartix;
glm::mat4 projMatrix;

glm::mat4 getViewMatrix() {
    return viewMartix;
}

glm::mat4 getProjMatrix() {
    return projMatrix;
}

// Initial camera position, which is on the z+ axis
glm::vec3 camPos = glm::vec3(0, 0, 12);
// Initial horizontal angle in radians, which points towards the z- axis
float horizAng = 3.14f;
// Initial vertical angle in radians, which points towards the horizon
float vertAng = 0.0f;
// Initial field of view, in degrees
float fov = 70.0f;

// The camera moving speed
float camSpeed = 4.0f;
// The mouse moving speed
float mouseSpeed = 0.001f;

void computeMatricesFromInputs(GLFWwindow *window, float deltaTime, int flag) {
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    // std::cout << "xPos: " << xPos << " yPos: " << yPos << std::endl;
    // reset the mosue position
    glfwSetCursorPos(window, ((double)config.windowWidth) / 2, ((double)config.windowHeight) / 2);

    // Calculate the angle depression and elevation of the camera
    horizAng += mouseSpeed * float(config.windowWidth / 2 - xPos);
    vertAng += mouseSpeed * float(config.windowHeight / 2 - yPos);

    glm::vec3 direction, frontVec, rightVec, upVec;
    // The direction vector, where we convert the spherical coord from the cursor into xyz coord
    direction = glm::vec3(
                cos(vertAng) * sin(horizAng),
                sin(vertAng),
                cos(vertAng) * cos(horizAng)
    );
    rightVec = glm::vec3(sin(horizAng - 3.14f/2.0f), 0, cos(horizAng - 3.14f/2.0f));
    if (flag == 0) {
        frontVec = direction;
        // Use the cross product to calculate the vector that is perpendicular to both the front-facing
        // vector and the right vector. Giving us the vector that points towards the top of our head
        upVec = glm::cross(rightVec, direction);
    } else {
        frontVec = direction * glm::vec3(1.0f, 0, 1.0f);
        upVec = glm::vec3(0, 1, 0);
    }

    // Key inputs
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camPos += glm::normalize(frontVec) * deltaTime * camSpeed;  
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camPos -= glm::normalize(frontVec) * deltaTime * camSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camPos += glm::normalize(rightVec) * deltaTime * camSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camPos -= glm::normalize(rightVec) * deltaTime * camSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camPos += glm::normalize(upVec) * deltaTime * camSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camPos -= glm::normalize(upVec) * deltaTime * camSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        config.inputMode = !config.inputMode;
    }

    // Matrix calculation
    projMatrix = glm::perspective(glm::radians(45.0f),
                                    ((float)config.windowWidth) / ((float)config.windowHeight), 
                                    0.1f, 
                                    100.0f
                                );
    viewMartix = glm::lookAt(camPos, camPos + direction, upVec);
}