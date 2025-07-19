#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <glm/vec4.hpp>
#include <glad/gl.h>
#include <iostream>

struct Particle{ // Needs to be multiple of 16 bytes for performance and cpu/gpu communication
    glm::vec4 position; //16 bytes
    glm::vec4 velocity; //16 bytes
    float radius; //4 bytes
    float padding[3]; //12 bytes -> can be used for other variables in the future
};

class Particle_System {
    private:
        GLuint ssbo;
        unsigned int particleCount;
    public:
        void initialize(unsigned int count);
        void update(GLuint computeShader, float dt);
        void draw(GLuint shaderProgram);
};

#endif 