#include "SpriteRenderer.hpp"
#include <iostream>
#include <cstring>

namespace wpwp
{
    void SpriteRenderer::loadSprite(std::string path)
    {
        if (!path.empty())
        {
            sf::Texture tempTexture;
            if (!tempTexture.loadFromFile(path))
            {
                ERROR("Failed to load texture from file: ", path);
                return;
            }
            m_texture = tempTexture;
            m_filePath = path;
            loadTexture(&m_texture);
        }
    }

    void SpriteRenderer::onDrawGUI()
    {
        static char filePathBuffer[256];

        // Copy the file path to the buffer ensuring null-termination
        std::strncpy(filePathBuffer, m_filePath.c_str(), sizeof(filePathBuffer) - 1);
        filePathBuffer[sizeof(filePathBuffer) - 1] = '\0'; // Ensure null termination

        if (ImGui::InputText("File Path", filePathBuffer, sizeof(filePathBuffer)))
        {
            loadSprite(std::string(filePathBuffer));
        }

        if (sprite.getTexture() != nullptr)
        {
            // Ensure correct usage of ImGui::Image with SFML texture
            ImTextureID textureID = (ImTextureID)(intptr_t)sprite.getTexture()->getNativeHandle();

            ImVec2 size = ImVec2(150, 150);

            ImGui::Image(textureID, size);
            ImGui::Dummy({0.0f, 3.0f});
        }

        // Update m_filePath after interacting with the GUI
        m_filePath = std::string(filePathBuffer);

        // Call the base class onDrawGUI if it exists
        this->Renderer::onDrawGUI();
    }

    void SpriteRenderer::loadTexture(sf::Texture *texture)
    {
        sprite.setTexture(*texture);
    }

    void SpriteRenderer::syncTransform()
    {
        sf::Vector2f pos(entity->transform->getPosition()->x, entity->transform->getPosition()->y);

        if (sprite.getTexture() && transform->getScale()->x != 0 && transform->getScale()->y != 0)
        {
            // Apply the transform scale to the sprite
            sf::Vector2f transformScale{transform->getScale()->x, transform->getScale()->y};
            sf::Vector2u textureSize = m_texture.getSize();

            // Ensure textureSize is not zero to avoid division by zero
            if (textureSize.x != 0 && textureSize.y != 0)
            {
                sprite.setScale(
                    transformScale.x / static_cast<float>(textureSize.x),
                    transformScale.y / static_cast<float>(textureSize.y));
            }

            sprite.setRotation(entity->transform->getRotation()->z);
            sprite.setColor(material.color);
            sprite.setPosition(pos);
        }
    }

    void SpriteRenderer::update()
    {
        if (sprite.getTexture() && transform && m_texture.getSize().x > 0 && m_texture.getSize().y > 0)
        {
            unsigned int scalar = 2;
            sf::Vector2f origin = sf::Vector2f(m_texture.getSize() / scalar);
            sprite.setOrigin(origin);
            Engine::getInstance()->draw(sprite);
        }
    }

    void SpriteRenderer::start()
    {
        transform->onTransformChanged += [this]()
        {
            this->syncTransform();
        };

        syncTransform();
    }
} // namespace wpwp