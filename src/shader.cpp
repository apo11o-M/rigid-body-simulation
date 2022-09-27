#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include "shader.hpp"

using namespace std;

GLuint LoadShaders(const string vertFilePath, const string fragFilePath) {

    // Read the vertex shader from the file
    string vertShaderCode;
    ifstream vertShaderFile;
    vertShaderFile.open(vertFilePath);
    if (vertShaderFile.is_open()) {
        string line;
        while (getline(vertShaderFile, line)) {
            vertShaderCode = vertShaderCode + line + "\n";
        }
    } else {
        fprintf(stderr, "Failed to open vertex shader file at path \"%s\"\n", vertFilePath.c_str());
        return 0;    
    }
    // cout << "vertShaderCode:" << endl;
    // cout << vertShaderCode << endl;

    // Read the fragment shader from the file
    string fragShaderCode;
    ifstream fragShaderFile;
    fragShaderFile.open(fragFilePath);
    if (fragShaderFile.is_open()) {
        string line;
        while (getline(fragShaderFile, line)) {
            fragShaderCode = fragShaderCode + line + "\n";
        }
    } else {
        fprintf(stderr, "Failed to open fragment shader file at path \"%s\"\n", fragFilePath.c_str());
        return 0;
    }
    // cout << "fragShaderCode:" << endl;
    // cout << fragShaderCode << endl;

    GLint result = GL_FALSE;
    int infoLogLength;

    // Create the shaders with their unique IDs
    GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile vertex shader
    cout << "Compiling shader: \"" << vertFilePath << "\"...";
    char const *vertSourcePt = vertShaderCode.c_str();
    glShaderSource(vertShaderID, 1, &vertSourcePt, NULL);
    glCompileShader(vertShaderID);

    // Check vertex shader validity
    glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        char *errorMsg = new char[infoLogLength];
        glGetShaderInfoLog(vertShaderID, infoLogLength, NULL, errorMsg);
        cout << " Failed" << endl;
        fprintf(stderr, "%s\n", errorMsg);
        delete[] errorMsg;
    } else {
        cout << " Success" << endl;
    }
    
    // Compile fragment shader
    cout << "Compiling shader: \"" << fragFilePath << "\"...";
    char const *fragSourcePt = fragShaderCode.c_str();
    glShaderSource(fragShaderID, 1, &fragSourcePt, NULL);
    glCompileShader(fragShaderID);
    
    // Check fragment shader validity
    glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        char *errorMsg = new char[infoLogLength];
        glGetShaderInfoLog(fragShaderID, infoLogLength, NULL, errorMsg);
        cout << " Failed" << endl;
        fprintf(stderr, "%s\n", errorMsg);
        delete[] errorMsg;
    } else {
        cout << " Success" << endl;
    }

    // Link the shader to the program
    cout << "Linking programm...";
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertShaderID);
    glAttachShader(programID, fragShaderID);
    glLinkProgram(programID);

    // Check the program validity
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        char *errorMsg = new char[infoLogLength];
        glGetProgramInfoLog(programID, infoLogLength, NULL, errorMsg);
        cout << " Failed" << endl;
        fprintf(stderr, "%s\n", errorMsg);
        delete[] errorMsg;
    } else {
        cout << " Success" << endl;
    }

    glDetachShader(programID, vertShaderID);
    glDetachShader(programID, fragShaderID);
    glDeleteShader(vertShaderID);
    glDeleteShader(fragShaderID);

    return programID;
}