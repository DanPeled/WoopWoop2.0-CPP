#ifndef DEMO_SCENE_HPP
#define DEMO_SCENE_HPP
#include "WoopWoop.hpp"
#include "Scene.hpp"

namespace wpwp
{
    struct MainScene : public wpwp::Scene
    {
        void load() override;
    };
};

#endif