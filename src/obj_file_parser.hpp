#ifndef OBJFILEPARSER_HPP
#define OBJFILEPARSER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>

using std::vector;
using std::string;
using std::cout;
using std::endl;

/**
 * @brief An obj file parser that reads in a 3D model obj file.
 */
class ObjFileParser {
    public:
        ObjFileParser();
        void parseObjFile(  string filename, 
                            vector<glm::vec3> &outVertices, 
                            vector<glm::vec2> &outVertTexture, 
                            vector<glm::vec3> &outVertNormal
        );
    private:
};

#endif