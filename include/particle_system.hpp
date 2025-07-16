#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <glm/vec4.hpp>
#include <glad/gl.h>
#include <iostream>

struct Particle{
    glm::vec4 position;
    glm::vec4 velocity;
};

class Particle_System {
    private:
        GLuint ssbo;
        unsigned int particleCount;
    public:
        void initialize(unsigned int count);
        void initiate();
        void draw(GLuint shaderProgram);
};

#endif 