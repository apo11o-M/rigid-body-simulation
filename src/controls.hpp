#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

/**
 * @brief Calculate the projection and view matrices based on the keyboard input
 */
void computeMatricesFromInputs(GLFWwindow *window, float deltaTime);

/**
 * @brief Get the View Matrix object
 * 
 * @return glm::mat4 The camera matrix 
 */
glm::mat4 getViewMatrix();

/**
 * @brief Get the Proj Matrix object
 * 
 * @return glm::mat4 The projection matrix
 */
glm::mat4 getProjMatrix();

#endif