#include "WoopWoop.hpp"

namespace wpwp
{
    void CircleRenderer::start()
    {
        m_circleShape = sf::CircleShape(entity->transform->getScale()->x);
        material.color = sf::Color::White;

        transform->onTransformChanged += [&]()
        {
            sf::Vector2f pos(entity->transform->getPosition()->x, entity->transform->getPosition()->y);
            m_circleShape.setPosition(pos);
            m_circleShape.setRadius(0.5);
            m_circleShape.setScale(sf::Vector2f(entity->transform->getScale()->x, entity->transform->getScale()->y));
            m_circleShape.setRotation(entity->transform->getRotation()->z);
        };
    }

    void CircleRenderer::update()
    {
        m_circleShape.setFillColor(material.color);
        wpwp::Engine::getInstance()->draw(m_circleShape);
    }
};