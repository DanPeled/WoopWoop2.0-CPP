#include "RenderingSub.hpp"

namespace wpwp
{
    void RenderingSub::init()
    {
        Engine::getInstance()->onStartOfFrame += []() {
        };

        Engine::getInstance()->onEndOfFrame += []()
        {
            Engine::getInstance()->window.display();
        };

        Engine::getInstance()->onStartRender += []()
        {
            Engine::getInstance()->m_renderTexture.display();
            Engine::getInstance()->window.clear();
        };
    }
}