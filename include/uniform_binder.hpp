#ifndef UNIFORM_BINDER_HPP
#define UNIFORM_BINDER_HPP

#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>
#include <string>

class Uniform_Binder {
    public:
        GLuint programID;
        std::unordered_map<std::string, GLint> uniformCache;

        Uniform_Binder(GLuint id) : programID(id) {}

        void cacheUniform(const std::string& name) {
            GLint location = glGetUniformLocation(programID, name.c_str());
            uniformCache[name] = location;
        }

        GLint getUniform(const std::string& name) {
            auto it = uniformCache.find(name);
            if(it != uniformCache.end()) return it->second;

            GLint loc = glGetUniformLocation(programID, name.c_str());
            uniformCache[name] = loc;
            return loc;
        }

        void setMat4(const std::string& name, const glm::mat4& mat){
            glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, glm::value_ptr(mat));
        }

        void setFloat(const std::string& name, float val) {
            glUniform1f(getUniform(name), val);
        }
};

#endif