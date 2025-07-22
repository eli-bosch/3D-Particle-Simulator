#include <particle_system.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>

#include <utils.hpp>

void Particle_System::initialize(unsigned int count)
{
    this->particleCount = count;

    std::vector<Particle> particles(count);
    for (uint32_t i = 0; i < particleCount; i++)
    {
        particles[i].position = Utils::randomVec4(-1.f, 1.f);
        particles[i].velocity = Utils::randomVec4(-0.5, 0.5);

        particles[i].gridID = i;
    }

    //Initialization of particle ssbo
    glGenBuffers(1, &particle_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particle_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particle_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Particle_System::initializeSpatialGrid() {
    int totalCells = gridDimX * gridDimY * gridDimZ;

    glGenBuffers(1, &spatial_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, spatial_ssbo);

    glBufferData(GL_SHADER_STORAGE_BUFFER, totalCells* sizeof(GridCell), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, spatial_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Particle_System::update(GLuint gridAssignShader, GLuint collisionShader, float dt) { //GPU
    glUseProgram(gridAssignShader);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particle_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, spatial_ssbo);

    glUniform1f(glGetUniformLocation(gridAssignShader, "cellSize"), cellSize);
    glUniform3f(glGetUniformLocation(gridAssignShader, "simMin"), simMin.x, simMin.y, simMin.z);
    glUniform3i(glGetUniformLocation(gridAssignShader, "gridDims"), gridDimX, gridDimY, gridDimZ);
    glUniform1ui(glGetUniformLocation(gridAssignShader, "particleCount"), particleCount);

    GLuint workGroupSize = 256;
    GLuint numGroups = (particleCount + workGroupSize - 1) / workGroupSize;
    glDispatchCompute(numGroups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    glUseProgram(collisionShader);

    glUniform1f(glGetUniformLocation(collisionShader, "dt"), dt);
    glUniform1ui(glGetUniformLocation(collisionShader, "particleCount"), particleCount);
    glUniform1i(glGetUniformLocation(collisionShader, "gridStride"), gridDimX * gridDimY);

    glm::vec3 simMax = simMin + glm::vec3(cellSize * gridDimX,
                                      cellSize * gridDimY,
                                      cellSize * gridDimZ);

    glUniform3f(glGetUniformLocation(collisionShader, "simMax"), simMax.x, simMax.y, simMax.z);

    glDispatchCompute(numGroups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void Particle_System::draw(GLuint shaderProgram)
{ // Batch draw particle vertex array
    glUseProgram(shaderProgram);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particle_ssbo);

    GLuint dummyVAO;
    glGenVertexArrays(1, &dummyVAO);
    glBindVertexArray(dummyVAO);

    glDrawArrays(GL_POINTS, 0, particleCount);
}
