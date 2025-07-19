#include <particle_system.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>

#include <utils.hpp>

void Particle_System::initialize(unsigned int count) {
    this->particleCount = count;

    std::vector<Particle> particles(count);
    for (auto& p : particles) {
        p.position = Utils::randomVec4(-1.f, 1.f); 
        p.velocity = Utils::randomVec4(-0.5f, 0.5f); 
    }

    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Particle_System::update(GLuint computeShader, float dt) {
    glUseProgram(computeShader);

    GLint dtLocation = glGetUniformLocation(computeShader, "dt");
    glUniform1f(dtLocation, dt);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    
    GLuint numGroups = (particleCount + 255) / 256;

    glDispatchCompute(numGroups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}


void Particle_System::draw(GLuint shaderProgram) { //Batch draw particle vertex array
    glUseProgram(shaderProgram);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    GLuint dummyVAO;
    glGenVertexArrays(1, &dummyVAO);
    glBindVertexArray(dummyVAO);

    glDrawArrays(GL_POINTS, 0, particleCount);
}




