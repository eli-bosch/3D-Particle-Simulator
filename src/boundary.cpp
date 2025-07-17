#include "boundary.hpp"

Boundary::Boundary() : edgeVertices{{
    -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f,

    // Top square
    -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f,

    // Vertical lines
    -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f
}} {
    glGenVertexArrays(1, &boundaryVAO);
    glGenBuffers(1, &boundaryVBO);

    glBindVertexArray(boundaryVAO);
    glBindBuffer(GL_ARRAY_BUFFER, boundaryVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) *edgeVertices.size(),edgeVertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

void Boundary::draw(GLuint shaderProgram) {
    //glUseProgram(shaderProgram);
    
    glBindVertexArray(boundaryVAO);
    glDrawArrays(GL_LINES, 0, 72/3); //(72/3) -> 12 lines
    glBindVertexArray(0);
}
