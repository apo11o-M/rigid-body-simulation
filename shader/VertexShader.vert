#version 410 core

// Input vertex data, different fro all executions of this shader
layout(location = 0) in vec3 modelVertPos;
// The model view projection matrix
uniform mat4 mvp;

void main() {
    // Multiply the model with the mvp matrix
    gl_Position = mvp * vec4(modelVertPos, 1);
}