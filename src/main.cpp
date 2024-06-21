#include "WoopWoop.hpp"
#include "demo/demoScene.hpp"
#include "Serlization/SceneSerializer.hpp"

int main()
{
    wpwp::Engine engine;
    engine.loadProject("config");
    engine.run();
    return 0;
}