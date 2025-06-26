#include "controller.h"

Controller::Controller(View& cameraRef) : camera(cameraRef)
{
    this->rotationSpeed = 5.f;
    this->magnificationSpeed = 1.f;
}

void Controller::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Left:
            case sf::Keyboard::A:
                camera.rotateLeft(rotationSpeed);
                break;
            case sf::Keyboard::Right:
            case sf::Keyboard::D:
                camera.rotateRight(rotationSpeed);
                break;
            case sf::Keyboard::Up:
            case sf::Keyboard::W:
                camera.rotateUp(rotationSpeed);
                break;
            case sf::Keyboard::Down:
            case sf::Keyboard::S:
                camera.rotateDown(rotationSpeed);
                break;
            case sf::Keyboard::Hyphen:
                camera.zoomOut(magnificationSpeed);
                break;
            case sf::Keyboard::Equal:
                camera.zoomIn(magnificationSpeed);
                break;
            default:
                break;
        }
    }

}