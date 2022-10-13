#version 410 core

// Input vertex data, different fro all executions of this shader
layout(location = 0) in vec3 modelVertPos;
// Note that the location # is equal to the # in the glVertexAttribPointer
layout(location = 1) in vec3 vertColor;

// Output data, will be interpolated for each fragment
out vec3 fragmentColor;

// The model view projection matrix
uniform mat4 mvp;

void main() {
    // Multiply the model with the mvp matrix
    gl_Position = mvp * vec4(modelVertPos, 1);
     
    // The color of each vertex will be interpolated to produce the color of each fragment
    fragmentColor = vertColor;
}