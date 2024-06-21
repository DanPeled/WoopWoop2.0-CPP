#include "Entity.hpp"
#include <iostream>
#include "Util/Util.hpp"
#include "ECS/Components/Transform.hpp"
#include "WoopWoop.hpp"

namespace wpwp
{

    std::vector<std::shared_ptr<Entity>> Entity::s_entities{};
    std::unordered_map<std::string, std::shared_ptr<Entity>> Entity::s_nameToEntity{};
    std::unordered_map<std::string, int> Entity::s_nameCount;
    Signal<std::shared_ptr<Entity>> Entity::onEntityInstantiated;
    Signal<std::shared_ptr<Entity>> Entity::onEntityDestroyed;

    Entity::Entity(sf::Vector3f initialPosition) : Entity(initialPosition, DEFAULT_ENTITY_NAME)
    {
    }

    Entity::Entity(sf::Vector3f initialPosition, std::string name) : transform(new Transform()), m_name(name)
    {
        this->m_components = std::vector<std::shared_ptr<Component>>();

        if (s_nameToEntity.find(name) != s_nameToEntity.end())
        {
            int count = ++s_nameCount[name];
            name += " (" + std::to_string(count) + ")";
        }

        this->m_name = name;
        this->transform = addComponent<Transform>();
        transform->setPosition(initialPosition);
        transform->setRotation(sf::Vector3f(0, 0, 0));
    }

    std::shared_ptr<Entity> Entity::createEntity(sf::Vector3f initialPos)
    {
        return std::make_shared<Entity>(Entity(initialPos));
    }

    std::shared_ptr<Entity> Entity::createEntity(std::string name, sf::Vector3f initialPos)
    {
        return std::make_shared<Entity>(Entity(initialPos, name));
    }

    void Entity::addComponent(std::shared_ptr<Component> component)
    {
        if (getComponent(component->getName()))
            return;

        m_components.push_back(component);
        component->attach(std::shared_ptr<Entity>(this));
        component->start();
    }

    std::shared_ptr<wpwp::Component> Entity::getComponent(const std::string &componentName) const
    {
        for (auto &comp : m_components)
        {
            if (comp->getName() == componentName)
            {
                return comp;
            }
        }
        return {}; // Default-constructed shared_ptr
    }

    void Entity::removeComponent(std::shared_ptr<Component> comp)
    {
        if (!comp)
        {
            ERROR("Null component pointer passed to removeComponent");
            return;
        }

        auto it = std::find_if(m_components.begin(), m_components.end(),
                               [&](auto c)
                               { return c->getName() == comp->getName(); });

        if (it != m_components.end())
        {
            m_components.erase(it);
            LOG("Component removed successfully.");
        }
        else
        {
            ERROR("Component not found in the entity's component list.");
        }
    }

    void Entity::clearComponents()
    {
        m_components.clear();
    }

    void Entity::destroy(std::shared_ptr<Entity> e)
    {
        onEntityDestroyed.invoke(e);

        // Remove from the entities vector
        s_entities.erase(std::remove(s_entities.begin(), s_entities.end(), e), s_entities.end());

        // Remove from the name to entity map
        auto it = s_nameToEntity.find(e->m_name);
        if (it != s_nameToEntity.end())
        {
            s_nameToEntity.erase(it);
            s_nameCount[e->m_name]--; // Decrement the count for this name
            if (s_nameCount[e->m_name] == 0)
            {
                s_nameCount.erase(e->m_name); // Remove from count if no more entities with this name
            }
        }
    }

    void Entity::start()
    {
    }

    void Entity::update(float deltaTime)
    {
        for (auto &comp : m_components)
        {
            if (!comp)
            {
                ERROR("NON VALID COMP");
                return;
            }
            if (!comp->transform)
            {
                comp.get()->attach(std::shared_ptr<Entity>(this));
            }
            comp.get()->update();
        }
    }
    void Entity::instantiate(std::shared_ptr<Entity> e)
    {
        for (auto &c : e->m_components)
        {
            // c->attach(e);
            // c->start();
        }

        if (e->m_name == DEFAULT_ENTITY_NAME)
        {
            e->m_name = Util::generate_uuid_v4();
        }

        if (s_nameToEntity.find(e->m_name) != s_nameToEntity.end())
        {
            int count = ++s_nameCount[e->m_name];
            e->m_name += " (" + std::to_string(count) + ")";
        }
        else
        {
            s_nameToEntity[e->m_name] = e;
            s_nameCount[e->m_name] = 0;
        }

        s_entities.push_back(e);
        onEntityInstantiated.invoke(e);
    }

    std::vector<std::shared_ptr<Entity>> Entity::getAllEntities()
    {
        return Entity::s_entities;
    }

    std::shared_ptr<Entity> Entity::getEntityWithName(std::string name)
    {
        auto it = s_nameToEntity.find(name);
        if (it != s_nameToEntity.end())
        {
            return it->second;
        }
        return nullptr; // Entity not found
    }

    void Entity::setEnabled(bool enabled)
    {
        this->m_enabled = enabled;
    }

    bool Entity::getEnabled() const
    {
        return this->m_enabled;
    }

    void Entity::setName(std::string newName)
    {
        // TODO: update in arrays
        this->m_name = newName;
    }

    void Entity::setName(const char *newName)
    {
        setName(std::string(newName));
    }

    std::vector<std::shared_ptr<Component>> Entity::getComponents() const
    {
        return m_components;
    }

}
