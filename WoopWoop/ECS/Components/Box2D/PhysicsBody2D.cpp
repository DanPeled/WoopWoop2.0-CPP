#include "PhysicsBody2D.hpp"
#include "Subsystems/Box2DIntegration.hpp"
#include "ECS/Components/Transform.hpp"
#include <iostream>

namespace wpwp
{
    PhysicsBody2D::PhysicsBody2D() : body(nullptr), m_world(nullptr)
    {
    }

    void PhysicsBody2D::init(b2World *world, const b2BodyDef &bodyDef)
    {
        m_world = world;
        body = m_world->CreateBody(&bodyDef);

        // Ensure the body has a fixture
        b2PolygonShape boxShape;
        // Convert SFML scale to Box2D size
        float width = transform->getScale()->x;
        float height = transform->getScale()->y;

        // Set the shape centered around the position
        boxShape.SetAsBox(width, height, b2Vec2(width, height), 0.0f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;

        body->CreateFixture(&fixtureDef);

        transform->onTransformChanged += [&]()
        {
            b2Vec2 position = {transform->getPosition()->x, transform->getPosition()->y};
            body->SetTransform(position, transform->getRotation()->z);
            if (body->GetFixtureList())
            {
                b2Fixture *fixture = body->GetFixtureList(); // Get pointer to the fixture
                b2FixtureDef def;
                def.density = fixture->GetDensity();
                def.restitution = fixture->GetRestitution();
                def.friction = fixture->GetFriction();
                b2PolygonShape boxShape;

                // Convert SFML scale to Box2D size
                float width = transform->getScale()->x;
                float height = transform->getScale()->y;

                // Set the shape centered around the position
                boxShape.SetAsBox(width / 2.0f, height / 2.0f, b2Vec2(width / 2.0f, height / 2.0f), 0.0f);

                def.shape = &boxShape;

                // Destroy the old fixture
                body->DestroyFixture(fixture);

                // Create a new fixture with updated properties
                body->CreateFixture(&def);
            }

            // TODO: update body shape
        };
    }

    void PhysicsBody2D::start()
    {
        auto world = Box2DIntegration::getWorld();
        if (world)
        {
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(transform->getPosition()->x, transform->getPosition()->y);
            init(world, bodyDef);
        }
        else
        {
            ERROR("Box2D world is not initialized.");
        }
        // Ensure the transform is synced initially
        syncTransform();
    }

    void PhysicsBody2D::update()
    {
        syncTransform();

#ifdef DEBUG
        const float lineThickness = 2.0f; // Adjust thickness as desired

        if (body == nullptr)
        {
            LOG("Body is null");
            return;
        }

        for (b2Fixture *f = body->GetFixtureList(); f; f = f->GetNext())
        {
            b2Shape::Type shapeType = f->GetType();
            if (shapeType == b2Shape::e_polygon)
            {
                b2PolygonShape *poly = dynamic_cast<b2PolygonShape *>(f->GetShape());
                if (poly != nullptr)
                {
                    for (int i = 0; i < poly->m_count; ++i)
                    {
                        b2Vec2 vertex1 = body->GetWorldPoint(poly->m_vertices[i]);
                        b2Vec2 vertex2 = body->GetWorldPoint(poly->m_vertices[(i + 1) % poly->m_count]);

                        sf::Vector2f point1(vertex1.x, vertex1.y);
                        sf::Vector2f point2(vertex2.x, vertex2.y);

                        sf::Vector2f direction = point2 - point1;
                        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                        sf::RectangleShape line(sf::Vector2f(length, lineThickness));
                        line.setOrigin(0, lineThickness / 2);
                        line.setPosition(point1);
                        float angle = std::atan2(direction.y, direction.x) * 180 / M_PI;
                        line.setRotation(angle);
                        line.setFillColor(sf::Color::White);

                        Engine::getInstance()->draw(line);
                    }
                }
                else
                {
                    ERROR("Dynamic cast to b2PolygonShape failed");
                }
            }
            else
            {
                ERROR("Non-polygon shape detected");
            }
        }
#endif
    }

    void PhysicsBody2D::syncTransform()
    {
        if (body && transform && transform->getScale()->x > 0 && transform->getScale()->y > 0)
        {
            transform->setPosition(sf::Vector3f(body->GetPosition().x, body->GetPosition().y, transform->getPosition()->z));
            transform->setRotation(sf::Vector3f(transform->getRotation()->x, transform->getRotation()->y, body->GetAngle() * 180 / 3.14));
        }
        else
        {
            ERROR("Transform Component is missing!");
        }
    }

    PhysicsBody2D::~PhysicsBody2D()
    {
        if (m_world && body)
        {
            m_world->DestroyBody(body);
        }
    }

    void PhysicsBody2D::onDrawGUI()
    {
        int selected = static_cast<int>(body->GetType());
        const char *items[] = {
            "Static",
            "Kinematic",
            "Dynamic"};

        const char **itemsPtr = items; // Pointer to array of const char *

        if (ImGui::Combo("Body Type", &selected, itemsPtr, IM_ARRAYSIZE(items)))
        {
            b2BodyType b2type = static_cast<b2BodyType>(selected);

            body->SetType(b2type);
        }

        b2Vec2 vec2 = body->GetLinearVelocity();
        ImGui::SetWindowFontScale(1.1);
        ImGui::Dummy({0.0f, 3.0f});

        ImGui::Text("Linear Velocity");
        ImGui::Separator();
        ImGui::SetWindowFontScale(1);
        if (ImGui::InputFloat("X", &vec2.x) ||
            ImGui::InputFloat("Y", &vec2.y))
        {

            body->SetLinearVelocity(vec2);
        }

        ImGui::Dummy({0.0f, 20.0f});

        float angularV = body->GetAngularVelocity();
        if (ImGui::InputFloat("Angular Velocity", &angularV))
        {
            body->SetAngularVelocity(angularV);
        }

        ImGui::Dummy({0.0f, 20.0f});
        ImGui::SetWindowFontScale(1.1);
        ImGui::Text("Propreties");
        ImGui::Separator();
        ImGui::SetWindowFontScale(1);

        float density = body->GetFixtureList()[0].GetDensity();
        float friction = body->GetFixtureList()[0].GetFriction();
        float restitution = body->GetFixtureList()[0].GetRestitution();

        ImGui::InputFloat("Density", &density);
        ImGui::InputFloat("Friction", &friction);
        ImGui::InputFloat("Restitution", &restitution);

        bool previousFixedRotation = m_isRotationFixed;

        if (ImGui::Checkbox("Fixed Rotation", &m_isRotationFixed))
        {

            // BUG: doesnt actually change the m_isRotationFixed value
            if (body && m_isRotationFixed != previousFixedRotation)
            {
                body->SetFixedRotation(m_isRotationFixed);
            }
        }

        body->GetFixtureList()[0].SetDensity(density);
        body->GetFixtureList()[0].SetFriction(friction);
        body->GetFixtureList()[0].SetRestitution(restitution);
    }

    void PhysicsBody2D::setRotationFixed(bool state)
    {
        m_isRotationFixed = state;
        if (body)
            body->SetFixedRotation(state);
        else
            ERROR("BODY IS MISSING");
    }
}
