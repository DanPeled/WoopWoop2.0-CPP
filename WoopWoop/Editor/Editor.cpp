#include "Editor.hpp"
#include <SFML/Graphics.hpp>
#include "Serlization/SceneSerializer.hpp"
#include "ECS/Entity.hpp"
#include <unordered_set>

namespace wpwp::Editor
{
    const int INITIAL_HEIGHT = 27;

    void Editor::init()
    {
#ifdef DEBUG

        Entity::onEntityInstantiated += [&](std::shared_ptr<Entity> e)
        {
            m_names.emplace_back(e->getName());
        };

        Entity::onEntityDestroyed += [&](std::shared_ptr<Entity> e)
        {
            m_names.erase(std::remove(m_names.begin(), m_names.end(), e->getName()), m_names.end());
        };
#endif
#ifndef DEBUG
        Input::mouseOffset = sf::Vector2i(0, 0); // Make sure mouse is offseted at default
#endif
    }

    void Editor::update()
    {
        if (m_editorActive)
        {
            editorUpdate();
        }
    }

    void Editor::editorUpdate()
    {
#pragma region File Menu
#ifdef DEBUG
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Preferences"))
                {
                    // TODO
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Save", "Ctrl+S"))
                {
                    SceneSerializer sceneSerializer = SceneSerializer(*Engine::getInstance()->getCurrentScene());
                    sceneSerializer.serialize("main");
                }
                else if (ImGui::MenuItem("Load", "Ctrl+O"))
                {
                    SceneSerializer sceneSerializer = SceneSerializer(*Engine::getInstance()->getCurrentScene());
                    sceneSerializer.deserialize("main");
                }

                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
#endif
#pragma endregion File Menu

#pragma region Entities List
#ifdef DEBUG

        {
            ImGui::Begin("Scene Hierarchy", NO_OPEN_IMGUI, NON_CHANGABLE_IMGUI);
            ImGui::SetWindowPos(ImVec2(13, INITIAL_HEIGHT));
            ImGui::SetWindowSize(ImVec2(219, 675));

            bool canOpenPopUp = true;
            std::unordered_set<std::string> namesToIgnore; // Using unordered_set for faster lookups

            for (int i = 0; i < m_names.size(); ++i)
            {
                if (namesToIgnore.find(m_names[i]) != namesToIgnore.end())
                {
                    continue;
                }

                auto children = Entity::getEntityWithName(m_names[i])->transform->getChildren();
                if (!children.empty())
                {
                    for (auto child : children)
                    {
                        namesToIgnore.insert(child->getName());
                    }
                }

                handleEntityTreeSelectionDrawing(i, canOpenPopUp);
            }

            bool clickedOnBlankSpace = ImGui::IsWindowFocused() &&
                                       !ImGui::IsAnyItemHovered() &&
                                       Input::isButtonPressed(sf::Mouse::Right);

            if (clickedOnBlankSpace)
            {
                ImGui::OpenPopup("Create Entity");
            }

            if (canOpenPopUp && ImGui::BeginPopupContextWindow("Create Entity"))
            {
                if (ImGui::MenuItem("Create Entity"))
                {
                    Entity::instantiate(Entity::createEntity("New Entity", sf::Vector3f(0, 0, 0)));
                }
                ImGui::EndPopup();
            }

            ImGui::End();

            ImGui::Begin("Details", NO_OPEN_IMGUI, NON_CHANGABLE_IMGUI);
            ImGui::SetWindowPos(ImVec2(1540, INITIAL_HEIGHT));
            ImGui::SetWindowSize(ImVec2(370, 975));

            if (m_selectedEntity)
            {
                ImGui::SetWindowFontScale(1.3);
                const char *name = m_names[m_selectedNameIndex].c_str();
                std::string text = "Name: " + std::string(name);
                ImGui::Text("%s", text.c_str());

                float windowWidth = ImGui::GetWindowWidth();
                float textWidth = ImGui::CalcTextSize(text.c_str()).x;
                float checkboxWidth = ImGui::CalcTextSize("##Enabled").x + ImGui::GetStyle().FramePadding.x * 2.0f;
                float availableWidth = windowWidth - textWidth - checkboxWidth - ImGui::GetStyle().ItemSpacing.x * 2.0f;

                // Ensure the checkbox is aligned to the right side
                if (availableWidth > 0.0f)
                {
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + availableWidth);
                }

                ImGui::SameLine();
                bool entityEnabled = m_selectedEntity->getEnabled();
                if (ImGui::Checkbox("##Enabled", &entityEnabled))
                {
                    m_selectedEntity->setEnabled(entityEnabled);
                }

                // if (ImGui::InputText("Name: ", name, size_t(255)))
                // {
                //     if (!Entity::getEntityWithName(name))
                //     {
                //         m_selectedEntity->setName(name);
                //         m_names[m_selectedNameIndex] = name;
                //     }
                // }

                ImGui::Separator();
                m_components = m_selectedEntity->getComponents();

                std::vector<std::string> componentsToRemove{};
                for (auto &comp : m_components)
                {
                    if (!comp)
                        continue;

                    ImGui::SetWindowFontScale(1.2);
                    if (ImGui::TreeNodeEx(comp->getName().c_str()))
                    {
                        if (comp->getName() != "Transform")
                        {
                            // BUG: For some reason removing any component crashes everything
                            if (comp->getName() != "Transform" && ImGui::IsItemClicked(1))
                            {
                                ImGui::OpenPopup("Remove Component");
                            }

                            if (ImGui::BeginPopupContextItem("Remove Component"))
                            {
                                if (comp->getName() != "Transform")
                                    if (ImGui::MenuItem(std::string("Remove " + (comp->getName()) + " Component").c_str()))
                                    {
                                        componentsToRemove.push_back(comp->getName());
                                    }
                                ImGui::EndPopup();
                            }
                        }

                        ImGui::SetWindowFontScale(1);
                        comp->onDrawGUI();
                        ImGui::TreePop();
                    }

                    ImGui::SetWindowFontScale(1);
                    ImGui::Separator();

                    ImGui::Dummy(ImVec2(0.0f, 3.0f));
                }

                if (ImGui::Button("Add Component", ImVec2{ImGui::GetWindowWidth() - 2, 20}))
                {
                    ImGui::OpenPopup("Add Component Popup");
                }

                if (ImGui::BeginPopup("Add Component Popup"))
                {
                    for (auto compName : Registry::getInstance().getAllRegistered())
                    {
                        if (compName == "Transform")
                            continue;

                        if (ImGui::MenuItem(compName.c_str()))
                        {
                            m_selectedEntity->addComponent(Registry::getInstance().createInstance(compName));
                        }
                    }
                    ImGui::EndPopup();
                }

                // for (auto name : componentsToRemove)
                // {
                //     auto comp = m_selectedEntity->getComponent(name);
                //     if (comp)
                //     {
                //         m_selectedEntity->removeComponent(comp);
                //         m_components = m_selectedEntity->getComponents();
                //     }
                // }
            }
            ImGui::End();
        }
#pragma endregion Entities List

#pragma region Debug Screen
        {
            ImGui::Begin("Screen View", NO_OPEN_IMGUI, NON_CHANGABLE_IMGUI | NO_SCROLL_IMGUI | ImGuiWindowFlags_NoTitleBar);
            float scaleFactor = 1.6;
            ImGui::SetWindowSize(ImVec2(1920 / scaleFactor, 1080 / scaleFactor));
            ImGui::SetWindowPos(ImVec2(252, INITIAL_HEIGHT));
            ImGui::BeginTabBar("View");

            if (ImGui::BeginTabItem("Gameplay"))
            {
                sf::Sprite sprite;
                sprite.setTexture(Engine::getInstance()->m_renderTexture.getTexture());
                sf::Vector2u textureSize = sprite.getTexture()->getSize();
                ImGui::Image(Engine::getInstance()->m_renderTexture, scaleFactor, sf::Color::White, sf::Color::White);
                Input::mouseOffset = sf::Vector2i(ImGui::GetWindowPos().x + 164, ImGui::GetWindowPos().y + 49);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Editor"))
            {
                Engine::getInstance()->m_isPaused = true;
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
            ImGui::End();

            if (m_selectedEntity)
            {
                m_components = m_selectedEntity->getComponents();

                m_componentNames = VectorUtil::select(m_components, [](std::shared_ptr<Component> c)
                                                      { return c->getName(); });
            }
        }
#pragma endregion Debug Screen

#pragma region Controls
        {
            ImGui::Begin("Controls", NO_OPEN_IMGUI, NON_CHANGABLE_IMGUI | NO_SCROLL_IMGUI | ImGuiWindowFlags_NoTitleBar);

            ImGui::SetWindowPos({1458, INITIAL_HEIGHT});
            ImGui::SetWindowSize({73, 677});

            sf::Sprite sprite;
            sf::Texture texture;
            texture.loadFromFile("assets/tile_0000.png");
            sprite.setTexture(texture);
            if (ImGui::ImageButton("Play/Pause", sprite, sf::Vector2f(20, 20)))
            {
                Engine::getInstance()->togglePaused();
            }
            ImGui::End();
        }
#pragma endregion Controls

#pragma region Debug Console
        {
            ImGui::Begin("Bottom Menu", nullptr, ImGuiWindowFlags_NoTitleBar | NON_CHANGABLE_IMGUI); // Ensure flags are correctly combined

            ImGui::SetWindowSize({1204 + 73, 293});
            ImGui::SetWindowPos({252, 709});

            if (ImGui::BeginTabBar("Bottom Menu"))
            {
                if (ImGui::BeginTabItem("Console"))
                {
                    if (ImGui::BeginPopupContextWindow("Clear Console"))
                    {
                        if (ImGui::MenuItem("Clear"))
                        {
                            Logging::s_logs.clear();
                            Logging::s_logMapCount.clear();
                        }
                        ImGui::EndPopup();
                    }

                    if (ImGui::BeginMenuBar())
                    {
                        if (ImGui::BeginMenu("Options"))
                        {
                            if (ImGui::MenuItem("Clear"))
                            {
                                Logging::clear();
                            }
                            ImGui::EndMenu();
                        }
                        ImGui::EndMenuBar();
                    }

                    Logging::renderLogs();
                    ImGui::EndTabItem();
                }

                // if (ImGui::BeginTabItem("Files"))
                // {
                //     // TODO
                //     ImGui::EndTabItem();
                // }

                ImGui::EndTabBar();
            }

            ImGui::End();
        }
#pragma endregion Debug Console

#else
        sf::Sprite sprite;
        sprite.setTexture(Engine::getInstance()->m_renderTexture.getTexture());
        sf::Vector2u textureSize = sprite.getTexture()->getSize();
        Engine::getInstance()->window.draw(sprite); // render usually to the screen if isn't in debug mode
#endif
    }

    void Editor::selectEntity(int index)
    {
#ifdef DEBUG
        m_selectedNameIndex = index;
        m_selectedEntity = Entity::getEntityWithName(m_names[m_selectedNameIndex]);

        m_componentNames = VectorUtil::select(m_selectedEntity->getComponents(), [](std::shared_ptr<Component> c)
                                              { return c->getName(); });

        m_components = m_selectedEntity->getComponents();
#endif
    }

    void Editor::handleEntityTreeSelectionDrawing(int i, bool &canOpenCreateEntityPopup)
    {
#ifdef DEBUG

        const bool isSelected = (m_selectedNameIndex == i);
        ImVec4 col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White color by default
        bool isEnabled = Entity::getEntityWithName(m_names[i])->getEnabled();
        if (isSelected)
        {
            col = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Green color for selected item\

            if (!isEnabled)
            {
                ImVec4 col = ImVec4(15.0f / 255.0f, 184.0f / 255.0f, 15.0f / 255.0f, 1.0f);
            }
        }
        else if (!isEnabled)
        {
            ImVec4 col = ImVec4(176.0f / 255.0f, 181.0f / 255.0f, 176.0f / 255.0f, 1.0f);
        }
        if (!isEnabled)
        {
            col.w = 0.7f;
        }

        ImGui::PushStyleColor(ImGuiCol_Text, col);

        bool nodeOpen = ImGui::TreeNodeEx(m_names[i].c_str());
        if (isSelected)
        {
            ImGui::SetItemDefaultFocus();
        }
        ImGui::PopStyleColor();

        if (ImGui::IsItemHovered())
        {
            canOpenCreateEntityPopup = false;
        }

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            selectEntity(i);
        }

        if (nodeOpen)
        {
            auto entity = Entity::getEntityWithName(m_names[i]);
            if (entity)
            {
                for (auto child : entity->transform->getChildren())
                {
                    for (int j = 0; j < m_names.size(); j++)
                    {
                        if (m_names[j] == child->getName())
                        {
                            handleEntityTreeSelectionDrawing(j, canOpenCreateEntityPopup);
                        }
                    }
                }
            }
            ImGui::TreePop();
        }
#endif
    }

}