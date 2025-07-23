#include <particle_system.hpp>

#include <utils.hpp>

Particle_System::Particle_System(GLuint gridShaderID, GLuint collisionShaderID)
    : gridUniforms(gridShaderID), collisionUniforms(collisionShaderID) {};

void Particle_System::initialize(unsigned int count)
{
    this->particleCount = count;

    //Caching Uniforms
    collisionUniforms.cacheUniform("particleCount");
    collisionUniforms.cacheUniform("gridDims");
    collisionUniforms.cacheUniform("gridStride");
    collisionUniforms.cacheUniform("simMin");
    collisionUniforms.cacheUniform("simMax");
    collisionUniforms.cacheUniform("dt");

    //Initializing Particles
    std::vector<Particle> particles(count);
    for (uint32_t i = 0; i < particleCount; i++)
    {
        particles[i].position = Utils::randomVec4(-1.f, 1.f);
        particles[i].velocity = Utils::randomVec4(-1.f, 1.f);
    }

    //Initialization of particle ssbo
    glGenBuffers(1, &particle_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particle_ssbo);

    glBufferData(GL_SHADER_STORAGE_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particle_ssbo);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Particle_System::initializeSpatialGrid() {
    int totalCells = gridDim * gridDim * gridDim;

    //Caching Uniforms
    gridUniforms.cacheUniform("cellSize");
    gridUniforms.cacheUniform("simMin");
    gridUniforms.cacheUniform("gridDims");
    gridUniforms.cacheUniform("particleCount");

    //Initialization of spatial grid ssbo
    glGenBuffers(1, &spatial_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, spatial_ssbo);

    glBufferData(GL_SHADER_STORAGE_BUFFER, totalCells* sizeof(GridCell), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, spatial_ssbo);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Particle_System::updateGrid(GLuint gridAssignShader) { 
    glUseProgram(gridAssignShader);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particle_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, spatial_ssbo);

    //Updating cached uniforms
    gridUniforms.setFloat("cellSize", this->cellSize);
    gridUniforms.setVec3("simMin", this->simMin);
    gridUniforms.setInt("gridDims", this->gridDim);
    gridUniforms.setUInt("particleCount", this->particleCount);

    //Assigning threads and begining compute
    GLuint workGroupSize = 256;
    GLuint numGroups = (particleCount + workGroupSize - 1) / workGroupSize;

    glDispatchCompute(numGroups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);   
}

void Particle_System::updateParticle(GLuint collisionShader, float dt) {
    glUseProgram(collisionShader);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particle_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, spatial_ssbo);

    // Updating cached uniforms
    collisionUniforms.setUInt("particleCount", this->particleCount);
    collisionUniforms.setInt("gridDims", this->gridDim);
    collisionUniforms.setInt("gridStride", this->gridDim * this->gridDim);
    collisionUniforms.setVec3("simMin", this->simMin);
    collisionUniforms.setVec3("simMax", this->simMin + glm::vec3(this->cellSize * this->gridDim,
                                      this->cellSize * this->gridDim, this->cellSize * gridDim));
    collisionUniforms.setFloat("dt", dt);

    // Assigning threads and beggining compute
    GLuint workGroupSize = 256;
    GLuint numGroups = (particleCount + workGroupSize - 1) / workGroupSize;

    glDispatchCompute(numGroups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
}

void Particle_System::render(GLuint shaderProgram)
{ // Batch draw particle vertex array
    glUseProgram(shaderProgram);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particle_ssbo);

    GLuint dummyVAO;
    glGenVertexArrays(1, &dummyVAO);
    glBindVertexArray(dummyVAO);

    glDrawArrays(GL_POINTS, 0, particleCount);
}
