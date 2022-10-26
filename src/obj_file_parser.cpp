#include "obj_file_parser.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <glm/gtx/string_cast.hpp>

ObjFileParser::ObjFileParser() {

}

void ObjFileParser::parseObjFile(   string filename, 
                                    vector<glm::vec3> &outVertices, 
                                    vector<glm::vec2> &outVertTexture, 
                                    vector<glm::vec3> &outVertNormal) {
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        fprintf(stderr, "The file %s could not be opened.\n", filename.c_str());
        return;
    }
    vector<glm::vec3> vertices, vertNormal;
    vector<glm::vec2> vertTexture;
    vector<int> faceAttribIndices;
    string line, tag, face, vert;
    while (getline(file, line)) {
        if (!line.empty()) {
            tag = line.substr(0, line.find(' '));
            line = line.substr(line.find(' '));
            std::stringstream lineStream(line);
            if (strcmp(tag.c_str(), "v") == 0) {
                glm::vec3 vertex;
                lineStream >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(vertex);
            } else if (strcmp(tag.c_str(), "vt") == 0) {
                glm::vec2 uv;
                vertTexture.push_back(uv);
            } else if (strcmp(tag.c_str(), "vn") == 0) {
                glm::vec3 normal;
                lineStream >> normal.x >> normal.y >> normal.z;
                vertNormal.push_back(normal);
            } else if (strcmp(tag.c_str(), "f") == 0) {
                std::stringstream check1(line);
                while (getline(check1, face, ' ')) {
                    std::stringstream check2(face);
                    while (getline(check2, vert, '/') && vert != "f") {
                        if (vert.empty()) {
                            faceAttribIndices.push_back(-1);
                        } else {
                            faceAttribIndices.push_back(stoi(vert) - 1);
                        }
                    }
                }
            } else {
                // probably a comment and/or other lines. Ignore them.
            }
        }
    }
    for (unsigned int i = 0; i < faceAttribIndices.size(); i+=3) {
        // get the attributes using the indices
        glm::vec3 vertex =  (faceAttribIndices[i] >= 0) ? vertices[faceAttribIndices[i]] : glm::vec3(0);
        glm::vec2 texture = (faceAttribIndices[i + 1] >= 0) ? vertTexture[faceAttribIndices[i + 1]] : glm::vec3(0);
        glm::vec3 normal =  (faceAttribIndices[i + 2] >= 0) ? vertNormal[faceAttribIndices[i + 2]] : glm::vec3(0);

        // put those attributes into the buffers
        outVertices.push_back(vertex);
        outVertTexture.push_back(texture);
        outVertNormal.push_back(normal);
    }
    file.close();
}