#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include <box2d/box2d.h>
#include "ECS/Component.hpp"
#include "ECS/Components/Transform.hpp"
#include <memory>

namespace wpwp
{
    class PhysicsBody2D : public Component
    {
    public:
        b2Body *body; // Pointer to the Box2D body

    public:
        /**
         * @brief Default constructor.
         */
        PhysicsBody2D();

        /**
         * @brief Called when the component is started.
         */
        void start() override;

        /**
         * @brief Called every frame to update the component.
         */
        void update() override;

        /**
         * @brief Gets the name of the component.
         *
         * @return The name of the component.
         */
        std::string getName() const override { return "PhysicsBody2D"; }

        /**
         * @brief Virtual destructor.
         */
        virtual ~PhysicsBody2D();

        void onDrawGUI() override;

        bool getRotationFixed() const { return m_isRotationFixed; }
        void setRotationFixed(bool state);

    private:
        /**
         * @brief Syncs the transform with the Box2D body.
         */
        void syncTransform();

        /**
         * @brief Initializes the Rigidbody with a Box2D world and body definition.
         *
         * @param world The Box2D world.
         * @param bodyDef The Box2D body definition.
         */
        void init(b2World *world, const b2BodyDef &bodyDef);

    private:
        b2World *m_world; // Pointer to the Box2D world
        bool m_isRotationFixed = false;
    };

    WREGISTER(PhysicsBody2D)
}

#endif // RIGIDBODY_HPP
