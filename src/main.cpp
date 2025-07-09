#include <SFML/Window.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/gl.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "view.h"
#include "controller.h"
#include "particle_system.h"

//Utility Functions
//TODO: Create new util folder

std::string loadShaderSource(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open " << path << std::endl;
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "Shader compilation failed:\n" << log << std::endl;
    }

    return shader;
}

GLuint createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode = loadShaderSource(vertexPath);
    std::string fragmentCode = loadShaderSource(fragmentPath);

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        std::cerr << "Shader linking failed:\n" << log << std::endl;
    }

    return program;
}

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

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(16.0f); 

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // Set up camera and controller
    View camera;
    Controller controller(camera);

    // Create shaders
    GLuint frameShader = createShaderProgram("shaders/frame.vert", "shaders/frame.frag");
    GLuint particleShader = createShaderProgram("shaders/particle.vert", "shaders/particle.frag");

    // Vertex data for a cube
    float edgeVertices[] = {
    // Bottom square
    -0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,   0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,  -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,  -0.5f, -0.5f, -0.5f,

    // Top square
    -0.5f,  0.5f, -0.5f,   0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,   0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,  -0.5f,  0.5f, -0.5f,

    // Vertical lines
    -0.5f, -0.5f, -0.5f,  -0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,   0.5f,  0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,   0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,  -0.5f,  0.5f,  0.5f
    };
    
    GLuint edgeVBO, edgeVAO;
    glGenVertexArrays(1, &edgeVAO);
    glGenBuffers(1, &edgeVBO);

    glBindVertexArray(edgeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, edgeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(edgeVertices), edgeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //TODO: Check why duplicated (If carry over from tutorial or error)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Particle_System particles;
    particles.initialize(100);


    // Setup render loop
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        particles.update(dt);

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

        // Draws Frame
        glUseProgram(frameShader);

        GLuint frameViewLoc = glGetUniformLocation(frameShader, "view");
        GLuint frameProjLoc = glGetUniformLocation(frameShader, "projection");
        GLuint frameModelLoc = glGetUniformLocation(frameShader, "model");

        glm::mat4 frameModel = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f)); 

        glUniformMatrix4fv(frameViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(frameProjLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(frameModelLoc, 1, GL_FALSE, glm::value_ptr(frameModel));

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(edgeVAO);
        glDrawArrays(GL_LINES, 0, 24);
        glBindVertexArray(0);

        //Draws Particles
        glUseProgram(particleShader);

        float fov = glm::radians(60.0f); // match your projection matrix
        int height = window.getSize().y;

        GLuint particleViewLoc = glGetUniformLocation(particleShader, "view");
        GLuint particleProjLoc = glGetUniformLocation(particleShader, "projection");

        glUniform1f(glGetUniformLocation(particleShader, "fov"), fov);
        glUniform1f(glGetUniformLocation(particleShader, "viewportHeight"), (float)height);
        glUniformMatrix4fv(particleViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(particleProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Reset to fill if needed
        particles.draw(particleShader);

        // Display
        window.display();
    }

    return 0;
}
