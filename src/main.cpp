#include <SFML/Window.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/gl.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "view.hpp"
#include "controller.hpp"
#include "particle_system.hpp"
#include "boundary.hpp"
#include "utils.hpp"
#include "uniform_binder.hpp"

int main() {
    // Set up SFML OpenGL context (4.6 core)
    sf::Clock clock;

    float totalTime = 0.0f;
    int frameCount = 0;
    const int sampleWindow = 100;

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 4;
    settings.minorVersion = 6;
    settings.attributeFlags = sf::ContextSettings::Core;

    //Rendering window
    sf::Window window(sf::VideoMode(800, 600), "3D Particle Simulator",
                      sf::Style::Default, settings);
    window.setVerticalSyncEnabled(false);

    // Load OpenGL via GLAD
    if (!gladLoadGL((GLADloadfunc)sf::Context::getFunction)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }
    

    int width = window.getSize().x;
    int height = window.getSize().y;
    glViewport(0, 0, width, height);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl; //OpenGL version confirmation

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(8.0f); 

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create shaders
    GLuint frameShader = Utils::createShaderProgram("shaders/frame.vert", "shaders/frame.frag");
    GLuint particleShader = Utils::createShaderProgram("shaders/particle.vert", "shaders/particle.frag");
    GLuint gridAssignShader = Utils::createComputeShaderProgram("shaders/assign_grid.comp");
    GLuint collisionShader = Utils::createComputeShaderProgram("shaders/collision.comp");

    // Set up object references
    View camera;
    Controller controller(camera);
    Boundary boundary;
    Particle_System particles(gridAssignShader, collisionShader);  

    // Cached uniform look ups for transforms
    Uniform_Binder boundaryUniforms(frameShader);
    boundaryUniforms.cacheUniform("view");
    boundaryUniforms.cacheUniform("projection");
    boundaryUniforms.cacheUniform("model");

    Uniform_Binder particleUniforms(particleShader);
    particleUniforms.cacheUniform("view");
    particleUniforms.cacheUniform("projection");
    particleUniforms.cacheUniform("fov");
    particleUniforms.cacheUniform("height");

    particles.initialize(1);

    // Setup render loop
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        totalTime += dt;
        frameCount++;

        if(frameCount >= sampleWindow) { //For testing
            float avgFrameTime = totalTime / frameCount;
            float avgFPS = 1.0f / avgFrameTime;

            std::cout << "AVG FPS: " << avgFPS << ", Frame Time: " << avgFrameTime * 1000 << " ms" << std::endl;

            totalTime = 0.f;
            frameCount = 0;
        }

        // Handle input
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            controller.handleEvent(event);
        }

        // Clear screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Camera instance
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), 800.f / 600.f, 0.1f, 100.0f);
        float fov = glm::radians(60.0f); 
        int height = window.getSize().y;

        // Draws boundary frame
        glUseProgram(frameShader);

        glm::mat4 frameModel = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f)); 
        boundaryUniforms.setMat4("view", view);
        boundaryUniforms.setMat4("projection", projection);
        boundaryUniforms.setMat4("model", frameModel);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        boundary.render(frameShader);

        //Update Particles based on time that has passed
        particles.updateGrid(gridAssignShader);
        particles.updateParticle(collisionShader, dt);

        //Draws Particles
        glUseProgram(particleShader);

        particleUniforms.setMat4("view", view);
        particleUniforms.setMat4("projection", projection);
        particleUniforms.setFloat("fov", fov);
        particleUniforms.setFloat("height", (float)height);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Reset to fill if needed
        particles.render(particleShader);

        // Display
        window.display();
    }

    return 0;
}
