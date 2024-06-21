#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "Util/Subsystem.hpp"
#include "Util/Signal.hpp"
#include <thread>
#include <iostream>
#include <memory>
#include <filesystem>

namespace wpwp
{
    struct Scene;

    namespace Editor
    {
        class Editor;
    };

    /**
     * @brief The main engine class responsible for managing the game loop, rendering, and event handling.
     */
    class Engine
    {
    public:
        Engine();

        /**
         * @brief Constructs an Engine object with the given window title.
         *
         * @param title The title of the game window.
         */
        Engine(const std::string &title);

        /**
         * @brief Initializes the engine.
         */
        void init();

        /**
         * @brief Runs the game loop.
         */
        void run();

        /**
         * @brief Draws the specified drawable object onto the screen.
         *
         * @param drawable The drawable object to draw.
         */
        void draw(const sf::Drawable &drawable);

        /**
         * @brief Draws a collection of vertices onto the screen.
         *
         * @param vertices Pointer to the vertices array.
         * @param vertexCount Number of vertices in the array.
         * @param type Type of primitive to draw (points, lines, triangles, etc.).
         * @param states Render states to apply to the vertices.
         */
        void draw(const sf::Vertex *vertices, std::size_t vertexCount,
                  sf::PrimitiveType type, const sf::RenderStates &states = sf::RenderStates::Default);

        /**
         * @brief Loads a new scene into the engine.
         *
         * @param scenePath The path to the scene file to load.
         */
        void loadScene(std::string scenePath);

        /**
         * @brief Adds a subsystem to the engine.
         *
         * @param subs Reference to the subsystem to add.
         */
        void addSubsystem(Subsystem &subs);

        /**
         * @brief Gets the instance of the Engine.
         *
         * @return Pointer to the Engine instance.
         */
        static Engine *getInstance();

        void loadProject(const std::filesystem::path &filepath);

        /**
         * @brief Gets the current active scene.
         *
         * @return Pointer to the current Scene object.
         */
        Scene *getCurrentScene() const { return m_currentScene; }

        sf::RenderWindow window;           // The SFML window for rendering.
        Signal<sf::Event> onEvent;         // Signal for handling events.
        Signal<> onStartOfFrame;           // Signal emitted at the start of each frame.
        Signal<> onEndOfFrame;             // Signal emitted at the end of each frame.
        Signal<> onStartRender;            // Signal emitted at the start of the render sequence
        sf::RenderTexture m_renderTexture; // SFML render texture for off-screen rendering.

        friend Editor::Editor;
        friend class Box2DIntegration;

    private:
        /**
         * @brief Draws the FPS counter onto the screen.
         */
        void drawFPSCounter();
        // TODO:  make a dropdown using IMGUI that will have performance details

        /**
         * @brief Checks for SFML events and emits the appropriate signals.
         */
        void checkForEvents();

        /**
         * @brief Updates the game loop sequence.
         */
        void updateSequence();

        /**
         * @brief Updates all registered subsystems.
         */
        void updateSubsystems();

        /**
         * @brief Shuts down the engine and performs cleanup.
         */
        void shutdown();

        /**
         * @brief Toggles the paused state of the engine.
         */
        void togglePaused();

        /**
         * @brief Checks if the engine is valid to run.
         *
         * @return True if the engine is valid to run, false otherwise.
         */
        bool checkForValidRun();

        static Engine *instance; // Static pointer to the Engine instance.
        static sf::Font font;    // Static font object for rendering text.

        bool m_isPaused;                             // Flag indicating whether the engine is paused.
        sf::Clock m_clock;                           // SFML clock to measure elapsed time.
        sf::Clock m_deltaClock;                      // SFML clock to measure delta time.
        sf::Text m_fpsText;                          // SFML text object for displaying FPS.
        std::thread m_updateThread;                  // Thread for update operations.
        std::vector<wpwp::Subsystem *> m_subsystems; // Vector of registered subsystems.
        Scene *m_currentScene;                       // Pointer to the current scene.
    };

} // namespace wpwp

#endif // ENGINE_HPP
