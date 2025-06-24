#include "view.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

View::View() {
    this->yaw = glm::radians(0.f);
    this->pitch = glm::radians(0.f);
    this->radius = 10.f;
    this->center = glm::vec3(0.f);
}

void View::rotateLeft(float ammount) {
    this->yaw -= glm::radians(ammount);
}

void View::rotateRight(float ammount) {
    this->yaw += glm::radians(ammount);
}

void View::rotateUp(float ammount) {
    this->pitch += glm::radians(ammount);
    //Clamp used to avoid gimbal lock
    pitch = std::clamp(pitch, -glm::half_pi<float>() + 0.01f, glm::half_pi<float>() - 0.01f);
}

void View::rotateDown(float ammount) {
    this->pitch -= glm::radians(ammount);
    pitch = std::clamp(pitch, -glm::half_pi<float>() + 0.01f, glm::half_pi<float>() - 0.01f);
}

glm::vec3 View::getCameraPosition() const {
    // Spherical to Cartesian conversion
    float x = center.x + radius * cos(pitch) * sin(yaw);
    float y = center.y + radius * sin(pitch);
    float z = center.z + radius * cos(pitch) * cos(yaw);
    return glm::vec3(x, y, z);
}

glm::mat4 View::getViewMatrix() const {
    glm::vec3 cameraPos = getCameraPosition();
    return glm::lookAt(cameraPos, center, glm::vec3(0.0f, 1.0f, 0.0f));
}



