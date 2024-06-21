#ifndef INPUT_HPP
#define INPUT_HPP

#include "WoopWoop.hpp"
#include "Editor/Editor.hpp"
#include <map>

namespace wpwp
{
    namespace Editor
    {
        class Editor;
    }
    /**
     * @brief Subsystem responsible for handling user input.
     */
    class Input : public Subsystem
    {
    public:
        /**
         * @brief Initializes the Input subsystem.
         */
        void init() override;

        /**
         * @brief Updates the Input subsystem.
         */
        void update() override;

        /**
         * @brief Cleans up the Input subsystem.
         */
        void onStop() override;

        /**
         * @brief Retrieves the current mouse position in screen coordinates.
         *
         * @return The mouse position in screen coordinates.
         */
        static sf::Vector2i getMouseScreenPosition();

        /**
         * @brief Retrieves the current mouse position in world coordinates.
         *
         * @return The mouse position in world coordinates.
         */
        static sf::Vector2i getMouseWorldPosition();

        /**
         * @brief Checks if a key is currently pressed.
         *
         * @param key The key to check.
         * @return True if the key is currently pressed, false otherwise.
         */
        static bool isKeyPressed(sf::Keyboard::Key key);

        /**
         * @brief Checks if a key has been released since the last frame.
         *
         * @param key The key to check.
         * @return True if the key has been released, false otherwise.
         */
        static bool isKeyReleased(sf::Keyboard::Key key);

        /**
         * @brief Checks if a key has been clicked (pressed and released) since the last frame.
         *
         * @param key The key to check.
         * @return True if the key has been clicked, false otherwise.
         */
        static bool isKeyClicked(sf::Keyboard::Key key);

        static bool isButtonPressed(sf::Mouse::Button button);
        static bool isButtonReleased(sf::Mouse::Button button);

        friend Editor::Editor;

    private:
        static std::map<sf::Keyboard::Key, bool>
            previouslyPressedKeys; // Map storing the state of previously pressed keys.
        static sf::Vector2i mouseOffset;
    };
}

#endif
