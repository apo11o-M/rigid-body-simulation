#ifndef UTIL_HPP
#define UTIL_HPP

/**
 * @brief Reads the vertex and fragment shader files and returns the programID of the combined 
 * shader. Note that we only have two kinds of shaders in this program.
 * 
 * @param vertFilePath The file path of the vertex shader
 * @param fragFilePath The file path of the fragment shader
 * @return GLuint
 */
GLuint LoadShaders(const std::string vertFilePath, const std::string fragFilePath);

void drawText(int x, int y, float r, float g, float b, int font, char *str);

#endif