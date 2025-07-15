#ifndef BOUNDARY_HPP
#define BOUNDARY_HPP

#include <glad/gl.h>
#include <array>

class Boundary {
    private:
        std::array<float, 72> edgeVertices;
        GLuint boundaryVAO;
        GLuint boundaryVBO;

    public:
        Boundary();
        void draw(GLuint shaderProgram);
};

#endif 