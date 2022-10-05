#version 410 core

// Input vertex data, different fro all executions of this shader
layout(location = 0) in vec3 modelVertPos;
// The model view projection matrix
uniform mat4 mvp;

void main() {
    // gl_Position.xyz = modelVertPos * 0.5;
    // gl_Position.w = 1.0;
    
    // vec4 v - vec4(modelVertPos, 1);
    // gl_Position = mvp * v;

    mat4 M = mat4(
        vec4( 1.211268, -0.625133, -0.698128, -0.696733),
        vec4( 0.000000,  2.262983, -0.349064, -0.348366),
        vec4(-1.345853, -0.562620, -0.628315, -0.627060),
        vec4( 0.000001,  0.000001, 14.181234, 14.352700)
    );
    vec4 v = vec4(modelVertPos, 1);
    gl_Position = M * v;

}