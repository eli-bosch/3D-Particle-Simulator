#ifndef UTILS_HPP
#define UTILS_HPP

#include <glad/gl.h>
#include <glm/vec4.hpp>

#include <sstream>

class Utils {
    private:
    public:
        Utils();
        
        static std::string loadShaderSource(const std::string& path);
        static GLuint compileShader(GLenum type, const std::string& source);
        
        static GLuint createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
        static GLuint createComputeShaderProgram(const std::string& computePath);

        static glm::vec4 randomVec4(float min, float max);
};

#endif