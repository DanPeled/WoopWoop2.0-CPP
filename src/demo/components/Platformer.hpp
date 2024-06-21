#ifndef PLATFORMER_HPP
#define PLATFORMER_HPP

#include "ECS/Component.hpp"
#include "ECS/Components/Box2D/PhysicsBody2D.hpp"

namespace wpwp
{
    class Platformer : public Component
    {
    public:
        Platformer();
        virtual void start() override;
        virtual void update() override;

        std::string getName() const override { return "Platformer"; }

    private:
        std::shared_ptr<PhysicsBody2D> m_physicsBody;
    };

    WREGISTER(Platformer)
}

#endif // PLATFORMER_HPP
