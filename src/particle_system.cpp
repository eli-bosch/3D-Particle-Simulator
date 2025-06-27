#include <particle_system.h>
#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint particleVAO = 0;

Particle_System::Particle_System() {
    glGenVertexArrays(1, &particleVAO);
    glBindVertexArray(particleVAO);
    glBindVertexArray(0);
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
}

void Particle_System::update(float dt){
    for(auto& p: particles) {
        p.velocity += p.acceleration;
        p.position += p.velocity;

        //Wall Collisions
        for (int axis = 0; axis < 3; axis++) {
            if (p.position[axis] < -1.f) {
                p.position[axis] = -1.f;
                p.velocity[axis] *= -1.f;
            }
            if (p.position[axis] > 1.f) {
                p.position[axis] = 1.f;
                p.velocity[axis] *= -1.f;
            }
        }
    }
}

void Particle_System::draw(GLuint shaderprogram) {
    glUseProgram(shaderprogram);
    glBindVertexArray(particleVAO);
    glPointSize(5.f);

    for(const auto& p : particles) {
        glm::mat4 model = glm::translate(glm::mat4(1.f), p.position);
        GLuint modelLoc = glGetUniformLocation(shaderprogram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_POINTS, 0, 1);
    }

    glBindVertexArray(0);
}