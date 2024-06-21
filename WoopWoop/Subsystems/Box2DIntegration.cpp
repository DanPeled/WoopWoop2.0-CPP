#include "Box2DIntegration.hpp"
#include "Util/Util.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Components/Transform.hpp"
#include "ImGuiSub.hpp"

namespace wpwp
{
    std::shared_ptr<b2World> Box2DIntegration::m_world;

    void Box2DIntegration::init()
    {
        b2Vec2 gravity(0, 981.0f);
        m_world = std::make_shared<b2World>(gravity);
        {
            b2BodyDef bodyDef;
            bodyDef.position.Set(0.0f, 900.0f);

            b2Body *groundBody = m_world->CreateBody(&bodyDef);

            b2PolygonShape groundBox;
            groundBox.SetAsBox(5000.0f, 10.0f);

            groundBody->CreateFixture(&groundBox, 0.0f);
        }
        {
            b2BodyDef bodyDef;
            bodyDef.position.Set(0.0f, 0.0f);

            b2Body *groundBody = m_world->CreateBody(&bodyDef);

            b2PolygonShape groundBox;
            groundBox.SetAsBox(10.0f, 5000.0f);

            groundBody->CreateFixture(&groundBox, 0.0f);
        }
        {
            b2BodyDef bodyDef;
            bodyDef.position.Set(1800.0f, 0.0f);

            b2Body *groundBody = m_world->CreateBody(&bodyDef);

            b2PolygonShape groundBox;
            groundBox.SetAsBox(10.0f, 5000.0f);

            groundBody->CreateFixture(&groundBox, 0.0f);
        }
    }

    void Box2DIntegration::update()
    {
        if (!Engine::getInstance()->m_isPaused)
            m_world->Step(1.0f / 900.0f, 6, 2);
    }
}