#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "view.h"
#include <SFML/Window.hpp>

class Controller {
    private:
        View& camera;
        float rotationSpeed;

    public:
        Controller(View& cameraRef);

        void handleEvent(const sf::Event& event);
        void update();
};

#endif