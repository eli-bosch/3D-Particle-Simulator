#include "utils.hpp"

#include <glm/glm.hpp>
#include <random>

#include <fstream>
#include <iostream>

Utils::Utils() {};

std::string Utils::loadShaderSource(const std::string& path) {
    std::ifstream file(path);
    if(!file) {
        std::cerr << "Failed to open shader" << path << std::endl;
        exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

GLuint Utils::compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        glDeleteShader(shader);
        std::cerr << "Shader compilation failed:\n" << log << std::endl;

        return 0;
    }

    return shader;
}

GLuint Utils::createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
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
    if(!success) {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        std::cerr << "Shader linking failed:\n" << log << std::endl;
    }

    return program;
}

GLuint Utils::createComputeShaderProgram(const std::string& computePath) {
    std::string computeCode = loadShaderSource(computePath);

    GLuint computeShader = compileShader(GL_COMPUTE_SHADER, computeCode);

    GLuint program = glCreateProgram();
    glAttachShader(program, computeShader);
    glLinkProgram(program);

    glDeleteShader(computeShader);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        std::cerr << "Compute Shader linking failed\n" << log << std::endl;
    }

    return program;
}

glm::vec4 Utils::randomVec4(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);

    return glm::vec4(dist(gen), dist(gen), dist(gen), 1.0f); // w = 1.0 for position
}