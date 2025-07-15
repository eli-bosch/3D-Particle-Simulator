#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "view.hpp"
#include <SFML/Window.hpp>

class Controller {
    private:
        View& camera;
        float rotationSpeed;
        float magnificationSpeed;

    public:
        Controller(View& cameraRef);

        void handleEvent(const sf::Event& event);
        void update();
};

#endif