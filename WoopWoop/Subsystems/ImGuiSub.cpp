#include "ImGuiSub.hpp"

namespace wpwp
{
    void ImGuiSubsystem::init()
    {
        ImGui::SFML::Init(Engine::getInstance()->window);

        Engine::getInstance()->onStartOfFrame += [&]()
        {
            ImGui::SFML::Update(Engine::getInstance()->window, m_deltaClock.restart());
        };

        Engine::getInstance()->onEvent += [&](sf::Event event)
        {
            ImGui::SFML::ProcessEvent(Engine::getInstance()->window, event);
        };
    }

    void ImGuiSubsystem::update()
    {
        if (Engine::getInstance())
            ImGui::SFML::Render(Engine::getInstance()->window);
    }

    void ImGuiSubsystem::onStop()
    {
        ImGui::SFML::Shutdown();
    }
}