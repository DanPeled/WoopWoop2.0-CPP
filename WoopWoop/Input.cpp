#include "Engine.hpp"
#include "Input.hpp"
#include <iostream>

namespace wpwp
{
    std::map<sf::Keyboard::Key, bool> Input::previouslyPressedKeys{};
    sf::Vector2i Input::mouseOffset = sf::Vector2i(0, 0);

    sf::Vector2i Input::getMouseScreenPosition()
    {
        return sf::Mouse::getPosition();
    }

    sf::Vector2i Input::getMouseWorldPosition()
    {
        if (Engine::getInstance())
        {
            sf::Vector2i pos = sf::Mouse::getPosition(Engine::getInstance()->window);

#ifdef DEBUG
            pos.x = static_cast<int>(pos.x * 1.6);
            pos.y = static_cast<int>(pos.y * 1.6);
            return pos - mouseOffset;
#else
            auto &window = Engine::getInstance()->window;
            sf::Vector2i pixelPos = sf::Mouse::getPosition(window);    // Mouse position in pixel coordinates
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos); // Convert to world coordinates

            return worldPos - mouseOffset;
#endif
        }

        ERROR("No engine instance!");
        return sf::Vector2i();
    }

    bool Input::isKeyPressed(sf::Keyboard::Key key)
    {
        return sf::Keyboard::isKeyPressed(key);
    }

    bool Input::isKeyReleased(sf::Keyboard::Key key)
    {
        return !(Input::isKeyPressed(key));
    }

    bool Input::isKeyClicked(sf::Keyboard::Key key)
    {
        ERROR("OY");
        return false;
        return (!previouslyPressedKeys[key] && isKeyPressed(key));
    }

    bool Input::isButtonPressed(sf::Mouse::Button button)
    {
        return sf::Mouse::isButtonPressed(button);
    }

    bool Input::isButtonReleased(sf::Mouse::Button button)
    {
        return !isButtonPressed(button);
    }

    void Input::init()
    {
        Engine::getInstance()->onEvent.bind([&](sf::Event event)
                                            {
            if (event.type == sf::Event::KeyPressed) {
                previouslyPressedKeys[event.key.code] = true;
            } else if (event.type == sf::Event::KeyReleased) {
                previouslyPressedKeys[event.key.code] = false;
            } });
    }

    void Input::update()
    {
    }

    void Input::onStop()
    {
    }
}