#include "BallComponent.hpp"
#include <cstdlib>
#include <ctime>
#include "Util/Util.hpp"

void wpwp::BallComponent::start()
{
    m_velocity = sf::Vector2f(1, 2);
    m_renderer = entity->getComponent<wpwp::CircleRenderer>();
}

void wpwp::BallComponent::changeColor()
{
    if (m_renderer)
    {
        m_renderer->material.color = sf::Color(rand() % 256, rand() % 256, rand() % 256);
    }
}

void wpwp::BallComponent::update()
{
    auto position = *transform->getPosition();
    if (position.y >= 1000 || position.y < 0)
    {
        m_velocity.y *= -1;
        changeColor();
    }

    if (position.x >= 1800 || position.x < 0)
    {
        m_velocity.x *= -1;
        changeColor();
    }

    float delta = wpwp::Util::deltaTime() * 1000;
    position.y += m_velocity.y * delta;
    position.x += m_velocity.x * delta;

    transform->setPosition(position);
}

void wpwp::BallComponent::onDrawGUI()
{
    float v[2] = {m_velocity.x, m_velocity.y};
    if (ImGui::InputFloat2("Velocity", v))
    {
        m_velocity = sf::Vector2f(v[0], v[1]);
    }
}
