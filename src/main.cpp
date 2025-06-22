#include <SFML/Window.hpp>
#include <iostream>
#include <glad/gl.h> // Use your GLAD header

// Vertex and Fragment Shader Sources
const char* vertexShaderSource = R"( 
#version 460 core
layout (location = 0) in vec3 aPos;
void main() {
    gl_Position = vec4(aPos, 1.0);
}
)"; //GLSL defines single vec3 @ location 0, glPosition tells gpu where to place vertex

const char* fragmentShaderSource = R"(
#version 460 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0); // orange
}
)"; //Outputs a solid orange color for every pixel inside of the triangle

// Shader compilation helper
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type); //Empty Shader
    glShaderSource(shader, 1, &source, nullptr); //Attaches GLSL source code to object
    glCompileShader(shader); //Compile the source code into gpu instructions

    // Optional error logging
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
    }

    return shader; //Returns shader id
}

int main() {
    // Request an OpenGL 4.6 Core Profile context
    sf::ContextSettings settings;
    settings.depthBits = 24; //z-buffer
    settings.stencilBits = 8; //masking
    settings.majorVersion = 4; //request OpenGL v4.x
    settings.minorVersion = 6; //request OpenGL vx.6
    settings.attributeFlags = sf::ContextSettings::Core; //Modern OpenGL

    // Create window with OpenGL context
    sf::Window window(sf::VideoMode(800, 600), "OpenGL Triangle (SFML + GLAD)", sf::Style::Default, settings);
    window.setActive(true);

    // Load OpenGL functions with GLAD
    if (!gladLoadGL((GLADloadfunc)sf::Context::getFunction)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Triangle vertices
    float vertices[] = {
        -1.f, -1.f, 0.0f,
         1.f, -1.f, 0.0f,
         1.f,  1.f, 0.0f
    };

    // Create VAO(Stores how vertex data is laid out) and 
    //VBO(stores the actual vertex data in GPU mem)
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Compile shaders
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // Link shaders into a program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Delete individual shaders after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers
        window.display();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    return 0;
}
