#version 410 core

// Input vertex data, different fro all executions of this shader
layout(location = 0) in vec3 modelVertPos;

void main() {
    gl_Position.xyz = modelVertPos * 0.5;
    gl_Position.w = 1.0;
}