#include "Component.hpp"
#include "Entity.hpp"
namespace wpwp
{
    void Component::attach(std::shared_ptr<Entity> e)
    {
        entity = e;
        transform = entity->transform;
    }

    void Component::start()
    {
    }

    void Component::update()
    {
    }

    void Component::onDisable()
    {
    }
}
