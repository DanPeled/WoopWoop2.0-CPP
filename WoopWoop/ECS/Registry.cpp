#include "Registry.hpp"
#include "Component.hpp"

using namespace wpwp;

Registry &Registry::getInstance()
{
    static Registry instance;
    return instance;
}

void Registry::registerType(const std::string &typeName, FactoryFunc factory)
{
    factoryMap[typeName] = factory;
}

std::shared_ptr<Component> Registry::createInstance(const std::string &typeName) const
{
    auto it = factoryMap.find(typeName);
    if (it != factoryMap.end())
    {
        return it->second();
    }
    return nullptr;
}

std::vector<std::string> Registry::getAllRegistered() const
{
    std::vector<std::string> registeredTypes;
    for (const auto &pair : factoryMap)
    {
        registeredTypes.push_back(pair.first);
    }
    return registeredTypes;
}

const std::type_info *Registry::getType(const std::string &typeName) const
{
    auto it = factoryMap.find(typeName);
    if (it != factoryMap.end())
    {
        std::shared_ptr<Component> instance = it->second();
        if (instance)
        {
            return &typeid(*instance);
        }
    }
    return nullptr;
}