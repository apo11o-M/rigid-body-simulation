#include <block.hpp>

// void vboInit(GLuint& vertBuffer, GLuint& colorBuffer, vector<glm::vec3>& modelVert, GLfloat (&modelColors)[]) {
void vboInit(GLuint& vboVertID, GLuint& vboColorID, vector<vec3>& modelVert, vector<GLfloat>& modelColors) {
    // Generate 1 vertex buffer object and put the result identifer in the vertBuffer
    glGenBuffers(1, &vboVertID);
    // Specify the target of the buffer object. In this case vertBuffer is bound to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, vboVertID);
    // Populate the GL_ARRAY_BUFFER with the vertices of our triangle
    // glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, modelVert.size() * sizeof(vec3), &modelVert[0], GL_STATIC_DRAW);
    
    // Same thing but for the color at each vertices
    glGenBuffers(1, &vboColorID);
    glBindBuffer(GL_ARRAY_BUFFER, vboColorID);
    glBufferData(GL_ARRAY_BUFFER, modelColors.size() * sizeof(GLfloat), &modelColors[0], GL_STATIC_DRAW);
}

void renderModel(GLuint& vboVertID, GLuint& vboColorID, vector<vec3>& modelVert) {
    // 1st attribute buffer: vertices
    glEnableVertexAttribArray(0);
    // Specify which array we want to draw. In this case we want to draw the vertex buffer obj 
    glBindBuffer(GL_ARRAY_BUFFER, vboVertID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 2nd attribute buffer: colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vboColorID);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // Actually draw the triangle starting from vertex 0. Note that one face has three vertices
    glDrawArrays(GL_TRIANGLES, 0, modelVert.size()); 
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}