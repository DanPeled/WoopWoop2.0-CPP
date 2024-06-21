#include "Camera2D.hpp"

namespace wpwp
{
    Camera2D::Camera2D()
    {
    }

    void Camera2D::start()
    {
        m_view = Engine::getInstance()->window.getDefaultView();

        transform->onTransformChanged += [&]()
        {
            sf::Vector3f position = *transform->getPosition();
            setCenter({position.x, position.y});

            sf::Vector3f rotation = *transform->getRotation();
            m_view.setRotation(rotation.z);
        };
    };

    void Camera2D::update()
    {
        if (isMain)
        {
            Engine::getInstance()->m_renderTexture.setView(m_view);
        }
    }

    void Camera2D::onDrawGUI()
    {
        bool prevMain = isMain;

        ImGui::Checkbox("Main", &isMain);
        // TODO: change all other camera to not being main if the current one is main

        float v[2] = {getViewSize().x, getViewSize().y};

        if (ImGui::InputFloat2("View Size", v))
        {
            setViewSize({v[0], v[1]});
        }
    }

    sf::Vector2f Camera2D::getCenter() const
    {
        return sf::Vector2f(transform->getPosition()->x, transform->getPosition()->y);
    }

    void Camera2D::setCenter(const sf::Vector2f &center)
    {
        m_view.setCenter(center);

        if (center != getCenter())
        {
            transform->setPosition({center.x, center.y, transform->getPosition()->z});
        }
    }

    void Camera2D::setViewSize(const sf::Vector2f &size)
    {
        m_view.setSize(size);
    }
}