#ifndef VIEW_HPP
#define VIEW_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class View {
    private:
        float yaw; //Horizontal angle
        float pitch; //Verticle angle
        float radius; 
        float magnification;

        glm::vec3 center;

    public:
        View();

        void rotateLeft(float ammount);
        void rotateRight(float ammount);
        void rotateUp(float ammount);
        void rotateDown(float ammount);

        void zoomIn(float ammount);
        void zoomOut(float ammount);

        glm::mat4 getViewMatrix() const;
        glm::vec3 getCameraPosition() const;
};

#endif