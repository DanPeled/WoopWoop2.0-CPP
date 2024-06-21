#ifndef BOX2D_INTEGRATION_HPP
#define BOX2D_INTEGRATION_HPP

#include <Util/Subsystem.hpp>
#include <memory>
#include "box2d/box2d.h"

namespace wpwp
{
    class Box2DIntegration : public Subsystem
    {
    public:
        void init() override;
        void update() override;

        static b2World *getWorld() { return m_world.get(); }

    private:
        static std::shared_ptr<b2World> m_world;
    };
};
#endif