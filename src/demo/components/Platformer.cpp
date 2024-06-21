#include "Platformer.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Components/Transform.hpp"
#include <iostream>

namespace wpwp
{
    Platformer::Platformer() : m_physicsBody(nullptr)
    {
    }

    void Platformer::start()
    {
        // Ensure we have a PhysicsBody2D component
        m_physicsBody = entity->getOrAddComponent<PhysicsBody2D>();
        m_physicsBody->body->GetFixtureList()[0].SetRestitution(1);
    }

    void Platformer::update()
    {
        if (m_physicsBody)
        {
            // Left and right movement
            float moveForce = 1000000.0f; // Adjust this value as needed

            if (Input::isKeyPressed(sf::Keyboard::A)) // Move left
            {
                b2Vec2 leftForce(-moveForce, 0.0f);
                m_physicsBody->body->ApplyForceToCenter(leftForce, true);
            }
            else if (Input::isKeyPressed(sf::Keyboard::D)) // Move right
            {
                b2Vec2 rightForce(moveForce, 0.0f);
                m_physicsBody->body->ApplyForceToCenter(rightForce, true);
            }
            static bool m_jumpRequested;
            // Jump
            if (Input::isKeyPressed(sf::Keyboard::Space) && !m_jumpRequested)
            {
                b2Vec2 jumpForce(0.0f, -1000000.0f); // Apply a force upwards
                m_physicsBody->body->SetLinearVelocity({m_physicsBody->body->GetLinearVelocity().x, -1000});
                m_jumpRequested = true;
            }
            else if (!Input::isKeyPressed(sf::Keyboard::Space))
            {
                m_jumpRequested = false; // Reset the jump request when space key is released
            }
        }
    }
}
