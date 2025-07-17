#include <particle_system.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>

#include <utils.hpp>

void Particle_System::initialize(unsigned int count) {
    this->particleCount = count;
    Utils utils;

    std::vector<Particle> particles(count);
    for (auto& p : particles) {
        p.position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // center, w = 1
        p.velocity = utils.randomVec4(-0.5f, 0.5f); // static
    }

    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Particle_System::initiate(GLuint computeShader, float dt) {
    glUseProgram(computeShader);

    GLint dtLocation = glGetUniformLocation(computeShader, "dt");
    if (dtLocation != -1) {
        glUniform1f(dtLocation, dt);
    } else {
        //std::cerr << "Warning: Uniform 'dt' not found in compute shader!\n";
    }

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    
    GLuint numGroups = (particleCount + 255) / 256;

    glDispatchCompute(numGroups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}


void Particle_System::draw(GLuint shaderProgram) { //Batch draw particle vertex array
    glUseProgram(shaderProgram);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    //std::cout << "Drawing " << particleCount << " particles\n";

    GLuint dummyVAO;
    glGenVertexArrays(1, &dummyVAO);
    glBindVertexArray(dummyVAO);

    glDrawArrays(GL_POINTS, 0, particleCount);
    GLenum err;
    // if((err = glGetError()) != GL_NO_ERROR) {
    //     std::cerr << "GL ERROR: " << err << std::endl;
    // }
}




