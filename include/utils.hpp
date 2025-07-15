#ifndef UTILS_HPP
#define UTILS_HPP

#include <glad/gl.h>

#include <sstream>

class Utils {
    private:
    public:
        Utils();
        
        std::string loadShaderSource(const std::string& path);
        GLuint compileShader(GLenum type, const std::string& source);
        GLuint createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
};

#endif