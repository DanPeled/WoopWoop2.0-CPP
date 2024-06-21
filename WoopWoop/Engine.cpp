#include "Editor/Editor.hpp"
#include "WoopWoop.hpp"

#include "Util/Subsystem.hpp"
#include "Subsystems/ImGuiSub.hpp"
#include "Subsystems/Box2DIntegration.hpp"
#include "Subsystems/RenderingSub.hpp"
#include "Serlization/SceneSerializer.hpp"
#include "Engine.hpp"

#include <unordered_set>
#include <mutex>
#include <iostream>

namespace wpwp
{
    Engine *Engine::instance = nullptr;
    sf::Font Engine::font;
    Input input;
    Editor::Editor editor;
    ImGuiSubsystem imguiSub;
    RenderingSub renderingSub;
    Box2DIntegration box2d;
    Logging logs;

    Engine::Engine() : window(sf::VideoMode(1920, 1080), "Unnamed")
    {
    }

    Engine::Engine(const std::string &title) : window(sf::VideoMode(1920, 1080), title)
    {
        init();
    }

    void Engine::init()
    {
        LOG("Initializing engine...");
        // Load font for displaying FPS
        if (!font.loadFromFile("assets/arial.TTF")) // Replace "arial.ttf" with the path to your font file
        {
            // Handle font loading failure
            ERROR("Failed to load font for display! (arial.TTF)");
        }

        if (instance == nullptr)
        {
            instance = this; // Only assign the instance if it's not already set
        }

        m_renderTexture.create(window.getSize().x, window.getSize().y);

        m_fpsText.setFont(font);                  // Set font for FPS text
        m_fpsText.setCharacterSize(24);           // Set character size
        m_fpsText.setFillColor(sf::Color::White); // Set text color
        m_fpsText.setPosition(10, 10);

        onEvent += [&](sf::Event e)
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (e.type == sf::Event::Resized)
            {
            }
        };

        addSubsystem(input);
        addSubsystem(editor);
        addSubsystem(box2d);
        addSubsystem(renderingSub);
        addSubsystem(logs);
        addSubsystem(imguiSub);
    }

    Engine *
    Engine::getInstance()
    {
        return instance;
    }

    void Engine::loadProject(const std::filesystem::path &filepath)
    {
        std::filesystem::path path = std::string("./data/") + filepath.c_str() + ".conf";
        LOG("Loading project from: ", path);

        std::ifstream stream(path);
        std::stringstream strStream;
        strStream << stream.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());

        std::string title;
        sf::Vector2u screenSize{};
        std::string mainSceneName;

        if (auto projNameData = data["Project Name"])
        {
            title = projNameData.as<std::string>();
        }
        else
        {
            ERROR("Project name is missing from config file!");
            title = "Untitled";
        }

        if (auto screenSizeData = data["Screen Size"])
        {
            sf::Vector2f screenSizeF = screenSizeData.as<sf::Vector2f>();
            screenSize = {(uint)screenSizeF.x, (uint)screenSizeF.y};
        }
        else
        {
            ERROR("Screen size missing from config file!");
            screenSize = {1920, 1080};
        }

        if (auto mainScene = data["Main Scene"])
        {
            mainSceneName = mainScene.as<std::string>();
        }
        else
        {
            ERROR("Main scene missing from config file!");
            return;
        }

        window.create(sf::VideoMode(screenSize.x, screenSize.y), title);

#ifndef DEBUG
        window.setSize({screenSize.x, screenSize.y});
#endif
        init();

        loadScene(mainSceneName);
        loadScene(mainSceneName);
    }

    void Engine::run()
    {
        if (!checkForValidRun())
        {
            ERROR("Engine couldn't run");
            return;
        }

        LOG("Starting main loop...");
        Logging::clear();

        while (window.isOpen())
        {
            onStartOfFrame.invoke();

            drawFPSCounter();

            updateSequence();
            checkForEvents();
            onStartRender.invoke();

            updateSubsystems();

            onEndOfFrame.invoke();
        }

        shutdown();
    }

    void Engine::shutdown()
    {
        LOG("Shutting down engine...");
        for (auto &sub : m_subsystems)
        {
            sub->onStop();
            LOG("Shutting down subsystem");
        }
        LOG("Shut down successfully.");
    }

    void Engine::togglePaused()
    {
        m_isPaused = !m_isPaused;
    }

    void Engine::updateSequence()
    {
        std::unordered_set<std::shared_ptr<Entity>> entitiesToIgnore{};
        if (!m_isPaused)
        {
            for (auto ent : wpwp::Entity::getAllEntities())
            {
                if (entitiesToIgnore.find(ent) == entitiesToIgnore.end())
                {
                    if (ent->getEnabled())
                    {
                        ent->update(0.0);
                    }
                }
                if (!ent->getEnabled())
                {
                    for (auto child : ent->transform->getChildren())
                    {
                        entitiesToIgnore.insert(child);
                    }
                }
            }
        }
    }

    void Engine::drawFPSCounter()
    {
        sf::Time elapsedTime = m_clock.restart();
        float fps = 1.0f / elapsedTime.asSeconds();
        m_fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));
        Util::m_deltaTime = elapsedTime.asSeconds();

        if (!m_isPaused)
        {
            m_renderTexture.clear();
            draw(m_fpsText);
        }
    }

    void Engine::checkForEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            onEvent.invoke(event);
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
    }

    void Engine::addSubsystem(wpwp::Subsystem &subs)
    {
        this->m_subsystems.push_back(&subs);
        if (subs.isEnabled)
        {
            subs.init();
        }
    }

    bool Engine::checkForValidRun()
    {
        if (!instance)
        {
            init();
            LOG("Intiailizing");
        }

        if (!m_currentScene)
        {
            ERROR("No current scene was defined...");
            return false;
        }
        else
        {
        }

        LOG("Preparing start...");

        return true;
    }

    void Engine::updateSubsystems()
    {
        for (auto &sub : m_subsystems)
        {
            if (sub)
                if (sub->isEnabled)
                {
                    sub->update();
                }
        }
    }

    void Engine::draw(const sf::Drawable &drawable)
    {
        m_renderTexture.draw(drawable);
    }

    void Engine::draw(const sf::Vertex *vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates &states)
    {
        m_renderTexture.draw(vertices, vertexCount, type, states);
    }

    void Engine::loadScene(std::string filePath)
    {
        m_currentScene = new Scene();
        SceneSerializer serializer(*m_currentScene);
        if (serializer.deserialize(filePath))
        {
            LOG("Loaded scene successfuly");
        }
    }
}
