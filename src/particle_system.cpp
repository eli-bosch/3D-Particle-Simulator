#include <particle_system.h>
#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint particleVAO = 0;
GLuint particleVBO = 0;

Particle_System::Particle_System() {
    glGenVertexArrays(1, &particleVAO);
    glGenBuffers(1, &particleVBO);
}

void Particle_System::initialize(int count) {
    this->particles.clear();
    for (int i = 0; i < count; ++i) {
        Particle p;
        p.position = glm::linearRand(glm::vec3(-1.f), glm::vec3(1.f));
        p.velocity = glm::linearRand(glm::vec3(-0.5f), glm::vec3(0.5f));
        p.acceleration = glm::vec3(0.0f);
        particles.push_back(p);
    }

    // Create position buffer
    std::vector<glm::vec3> positions;
    for (const auto& p : particles)
        positions.push_back(p.position);

    glBindVertexArray(particleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Particle_System::update(float dt) {
    for(auto& p: particles) {
        p.velocity += p.acceleration;
        p.velocity *= 0.995f; // slows over time

        p.position += p.velocity;

        // Wall collisions
        for (int axis = 0; axis < 3; ++axis) {
            p.position[axis] += p.velocity[axis];

            if (p.position[axis] < -1.f) {
                p.position[axis] = -1.f;
                p.velocity[axis] = -p.velocity[axis];
            }
            else if (p.position[axis] > 1.f) {
                p.position[axis] = 1.f;
                p.velocity[axis] = -p.velocity[axis];
        }
}

    }

    // Upload updated positions
    std::vector<glm::vec3> positions;
    for (const auto& p : particles)
        positions.push_back(p.position);

    glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(glm::vec3), positions.data());
}

void Particle_System::draw(GLuint shaderprogram) {
    glUseProgram(shaderprogram);
    glBindVertexArray(particleVAO);

    glDrawArrays(GL_POINTS, 0, particles.size());

    glBindVertexArray(0);
}
