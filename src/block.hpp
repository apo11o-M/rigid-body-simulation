#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using glm::vec3;
using std::vector;
using std::cout;
using std::endl;

// void vboInit(GLFWwindow* window, GLuint& vertBuffer, GLuint& colorBuffer, vector<glm::vec3>& modelVert, vector<GLfloat>& modelColors);
/**
 * @brief Initialize the vbo arrays
 * 
 * @param vboVertID 
 * @param vboColorID 
 * @param modelVert 
 * @param modelColors 
 */
void vboInit(GLuint& vboVertID, GLuint& vboColorID, vector<glm::vec3>& modelVert, vector<GLfloat>& modelColors);
/**
 * @brief Draw the vbos onto the screen
 * 
 * @param vboVertID 
 * @param vboColorID 
 * @param modelVert 
 */
void renderModel(GLuint& vboVertID, GLuint& vboColorID, vector<glm::vec3>& modelVert);

#endif