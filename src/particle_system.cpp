#include <particle_system.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>

#include <utils.hpp>

void Particle_System::initialize(unsigned int count)
{
    this->particleCount = count;
    grid.clear();

    std::vector<Particle> particles(count);
    for (int i = 0; i < particleCount; i++)
    {
        particles[i].position = Utils::randomVec4(-1.f, 1.f);
        particles[i].velocity = Utils::randomVec4(-0.5, 0.5);

        grid.insert(particles[i].position, i);
    }

    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

// void Particle_System::update(GLuint computeShader, float dt) { //GPU
//     glUseProgram(computeShader);

//     GLint dtLocation = glGetUniformLocation(computeShader, "dt");
//     glUniform1f(dtLocation, dt);
//     glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

//     GLuint numGroups = (particleCount + 255) / 256;

//     glDispatchCompute(numGroups, 1, 1);
//     glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
// }

//Formating might be weird because this is glsw code updated for c++

bool checkCollision(const Particle& p1, const Particle& p2) 
{   
    float radius = 0.01;

    glm::vec3 delta = {
        p1.position.x - p2.position.x,
        p1.position.y - p2.position.y,
        p1.position.z - p2.position.z
    };

    float distSq = dot(delta, delta);
    float combinedRadius = 2 * radius;
    return distSq < (combinedRadius * combinedRadius);
};

void resolveCollision(Particle& p1, Particle& p2)
{
    float radius = 0.01f;

    glm::vec3 delta = glm::vec3(p1.position) - glm::vec3(p2.position);
    float dist = glm::length(delta);
    float combinedRadius = 2.0f * radius;

    if (dist < 0.0001f) return;

    glm::vec3 normal = delta / dist;

    glm::vec3 relVel = glm::vec3(p1.velocity) - glm::vec3(p2.velocity);
    float separatingVel = glm::dot(relVel, normal);

    if (separatingVel > 0.0f) return;

    glm::vec3 impulse = -separatingVel * normal;

    // Update velocity (vec3 portion only)
    p1.velocity = glm::vec4(glm::vec3(p1.velocity) + impulse, p1.velocity.w);
    p2.velocity = glm::vec4(glm::vec3(p2.velocity) - impulse, p2.velocity.w);

    // Optional: separate overlapping particles
    float penetration = combinedRadius - dist;
    glm::vec3 correction = 0.5f * penetration * normal;

    p1.position = glm::vec4(glm::vec3(p1.position) + correction, p1.position.w);
    p2.position = glm::vec4(glm::vec3(p2.position) - correction, p2.position.w);
}



void Particle_System::update(GLuint computeShader, float dt)
{
    // 1. Map the SSBO to CPU-accessible memory
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    Particle* particles = (Particle*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);

    if (!particles) {
        std::cerr << "Failed to map SSBO!" << std::endl;
        return;
    }

    const float gravity = -5.f;
    const float radius = 0.01f;
    // 2. Populate spatial grid
    grid.clear();

    for(unsigned int i = 0; i < particleCount; i++) {
        grid.insert(particles[i].position, i);
    }

    // 3. Spatial Partitioning collisions (CPU-side)
    for (unsigned int i = 0; i < particleCount; ++i) {
    glm::ivec3 currentCell = grid.getCellCoord(particles[i].position);
    std::vector<glm::ivec3> neighbors = grid.getNeighborCells(currentCell);

    for (const glm::ivec3& cell : neighbors) {
        const std::vector<uint32_t>& indices = grid.getCell(cell);

        for (uint32_t j : indices) {
            if (j <= i) continue; // avoid duplicates and self-check

            if (checkCollision(particles[i], particles[j])) {
                resolveCollision(particles[i], particles[j]);
            }
        }
    }
}

    // 4. Unmap buffer to sync with GPU
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    // 5. Run compute shader to handle wall collisions (only)
    glUseProgram(computeShader);
    GLint dtLocation = glGetUniformLocation(computeShader, "dt");
    glUniform1f(dtLocation, dt);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    GLuint numGroups = (particleCount + 255) / 256;

    glDispatchCompute(numGroups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}


void Particle_System::draw(GLuint shaderProgram)
{ // Batch draw particle vertex array
    glUseProgram(shaderProgram);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    GLuint dummyVAO;
    glGenVertexArrays(1, &dummyVAO);
    glBindVertexArray(dummyVAO);

    glDrawArrays(GL_POINTS, 0, particleCount);
}
