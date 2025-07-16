#ifndef UTILS_HPP
#define UTILS_HPP

#include <glad/gl.h>
#include <glm/vec4.hpp>

#include <sstream>

class Utils {
    private:
    public:
        Utils();
        
        std::string loadShaderSource(const std::string& path);
        GLuint compileShader(GLenum type, const std::string& source);
        GLuint createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);

        glm::vec4 randomVec4(float min, float max);
};

#endif