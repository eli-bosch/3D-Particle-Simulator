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


int main() {
    // Set up SFML OpenGL context (4.6 core)
    sf::Clock clock;

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
    window.setVerticalSyncEnabled(true);

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


    // Set up util, camera and controller
    Utils util;
    View camera;
    Controller controller(camera);
    Boundary boundary;

    //Initilizes particles
    Particle_System particles;  

    // Create shaders
    GLuint frameShader = util.createShaderProgram("shaders/frame.vert", "shaders/frame.frag");
    GLuint particleShader = util.createShaderProgram("shaders/particle.vert", "shaders/particle.frag");
    particles.initialize(100);

    // Setup render loop
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        //particles.update(dt);

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

        // Draws Cube
        glUseProgram(frameShader);

        //TODO: Cache these look ups 
        GLuint frameViewLoc = glGetUniformLocation(frameShader, "view");
        GLuint frameProjLoc = glGetUniformLocation(frameShader, "projection");
        GLuint frameModelLoc = glGetUniformLocation(frameShader, "model");

        glm::mat4 frameModel = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f)); 

        glUniformMatrix4fv(frameViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(frameProjLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(frameModelLoc, 1, GL_FALSE, glm::value_ptr(frameModel));

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        boundary.draw(frameShader);

        //Draws Particles
        glUseProgram(particleShader);

        float fov = glm::radians(60.0f); // match the projection matrix
        int height = window.getSize().y;

        //TODO: Chache these look ups
        GLuint particleViewLoc = glGetUniformLocation(particleShader, "view");

        GLuint particleProjLoc = glGetUniformLocation(particleShader, "projection");

        glUniform1f(glGetUniformLocation(particleShader, "fov"), fov);

        glUniform1f(glGetUniformLocation(particleShader, "viewportHeight"), (float)height);

        glUniformMatrix4fv(particleViewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glUniformMatrix4fv(particleProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

        if(particleViewLoc == -1) std::cerr << "View uniform not found!" << std::endl;
        if(particleProjLoc == -1) std::cerr << "Projection uniform not found!" << std::endl;
        if(fov == -1) std::cerr << "FOV uniform not found!" << std::endl;
        if(height == -1) std::cerr << "Viewport uniform not found!" << std::endl;


        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Reset to fill if needed
        particles.draw(particleShader);

        // Display
        window.display();
    }

    return 0;
}
