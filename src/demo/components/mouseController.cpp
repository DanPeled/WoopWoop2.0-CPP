#include "mouseController.hpp"
#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>
using namespace wpwp;

void MouseController::start()
{
    entity->transform->setScale(sf::Vector3f(70, 70, 0));
}

wpwp::MouseController::~MouseController()
{
    LOG("Removed mouse controller");
}

void MouseController::update()
{
    sf::Vector2i mousePos = wpwp::Input::getMouseWorldPosition();
    entity->transform->setPosition(sf::Vector3f(mousePos.x, mousePos.y, 0));
}