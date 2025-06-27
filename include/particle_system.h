#ifndef PARTICLE_SYSTEM
#define PARTICLE_SYSTEM

#include <glm/vec3.hpp>
#include <glad/gl.h>

struct Particle{
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
};

class Particle_System {
    private:
        std::vector<Particle> particles;
    public:
        Particle_System();
        void initialize(int count);
        void update(float deltaTime);
        void draw(GLuint shaderProgram);
};

#endif 