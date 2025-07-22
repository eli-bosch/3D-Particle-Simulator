#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glad/gl.h>
#include <iostream>

constexpr int MaxParticlesPerCell = 32;

struct Particle{ // Needs to be multiple of 16 bytes for performance and cpu/gpu communication
    glm::vec4 position; //16 bytes
    glm::vec4 velocity; //16 bytes
    uint32_t gridID; //4 bytes

    int padding[3]; // 12 bytes;
};

struct GridCell {
    uint32_t count;
    uint32_t indices[MaxParticlesPerCell];
};

class Particle_System {
    private:
        GLuint particle_ssbo;
        GLuint spatial_ssbo;

        unsigned int particleCount;

        float cellSize = 0.02;
        glm::vec3 simMin = {-1.f, -1.f, -1.f};
        int gridDimX = 100, gridDimY = 100, gridDimZ = 100;
    public:
        void initialize(unsigned int count);
        void initializeSpatialGrid();

        void update(GLuint gridAssignShader, GLuint collisionShader, float dt);
        void draw(GLuint shaderProgram);

        void cleanup();
};

#endif 