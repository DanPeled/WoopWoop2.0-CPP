#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/System/Vector3.hpp>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Component.hpp"
#include "Util/Signal.hpp"
#include "Util/Util.hpp"
#define DEFAULT_ENTITY_NAME "__woopwoop_generatename"

namespace wpwp
{
    struct Transform;
    namespace Editor
    {
        class Editor;
    }

    /**
     * @brief Class representing an entity in the game.
     */
    class Entity
    {
    protected:
        /**
         * @brief Constructs an entity with the specified initial position.
         * Will auto-generate a uuid v4 name for the entity.
         *
         * @param initialPos The initial position of the entity.
         */
        Entity(sf::Vector3f initialPos);

        /**
         * @brief Constructs an entity with the specified initial position and name.
         *
         * @param initialPos The initial position of the entity.
         * @param name The name of the entity.
         */
        Entity(sf::Vector3f initialPos, std::string name);

    public:
        /**
         * @brief Creates and returns an entity with the specified initial position.
         *
         * @param initialPos The initial position of the entity.
         * @return A shared pointer to the created entity.
         */
        static std::shared_ptr<Entity> createEntity(sf::Vector3f initialPos);

        /**
         * @brief Creates and returns an entity with the specified name and initial position.
         *
         * @param name The name of the entity.
         * @param initialPos The initial position of the entity.
         * @return A shared pointer to the created entity.
         */
        static std::shared_ptr<Entity> createEntity(std::string name, sf::Vector3f initialPos);

    public:
        /**
         * @brief Starts the entity.
         * This method should be called after all components are added and the entity is fully initialized.
         */
        void start();

        /**
         * @brief Updates the entity.
         *
         * @param deltaTime The time elapsed since the last update.
         */
        void update(float deltaTime);

        /**
         * @brief Enables or disables the entity.
         *
         * @param enabled True to enable the entity, false to disable.
         */
        void setEnabled(bool enabled);

        /**
         * @brief Checks if the entity is enabled.
         *
         * @return True if the entity is enabled, false otherwise.
         */
        bool getEnabled() const;

        /**
         * @brief Gets the name of the entity.
         *
         * @return The name of the entity.
         */
        std::string getName() const { return m_name; };

        void setName(std::string newName);

        void setName(const char *newName);

        /**
         * @brief Gets all components attached to the entity.
         *
         * @return A vector containing pointers to all components.
         */
        std::vector<std::shared_ptr<Component>> getComponents() const;

        /**
         * @brief Adds a new component of type T to the entity.
         *
         * @tparam T The type of component to add.
         * @tparam Args Types of arguments to forward to the T's constructor.
         * @param args Arguments to forward to the T's constructor.
         * @return A shared pointer to the added component.
         */
        template <typename T, typename... Args>
        std::shared_ptr<T> addComponent(Args &&...args)
        {
            static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

            auto newComponent = std::make_shared<T>(std::forward<Args>(args)...);

            // newComponent.get()->attach(std::shared_ptr<Entity>(this));

            auto componentPtr = std::dynamic_pointer_cast<T>(newComponent);
            addComponent(componentPtr);
            return newComponent;
        }

        /**
         * @brief Adds an existing component to the entity.
         *
         * @param component The component to add.
         */
        void addComponent(std::shared_ptr<Component> component);

        /**
         * @brief Gets a component of a specified type attached to the entity.
         *
         * @tparam T The type of component to retrieve.
         * @return A pointer to the component of the specified type, or nullptr if not found.
         */
        template <typename T>
        std::shared_ptr<T> getComponent() const
        {
            for (auto &c : m_components)
            {
                auto casted = std::dynamic_pointer_cast<T>(c);
                if (casted)
                {
                    return casted;
                }
            }
            return nullptr;
        }

        /**
         * @brief Gets a component of a specified type attached to the entity using the registry.
         *
         * @param componentName The name of the component type.
         * @return A pointer to the component of the specified type, or nullptr if not found.
         */
        std::shared_ptr<Component> getComponent(const std::string &componentName) const;

        /**
         * @brief Gets or adds a component of a specified type attached to the entity.
         *
         * @tparam T The type of component to retrieve or add.
         * @return A shared pointer to the component.
         */
        template <class T>
        std::shared_ptr<T> getOrAddComponent()
        {
            if (!getComponent<T>())
            {
                return addComponent<T>();
            }

            return std::dynamic_pointer_cast<T>(getComponent<T>());
        }

        /**
         * @brief Removes a component of a specified type from the entity.
         *
         * @tparam T The type of component to remove.
         */
        template <typename T>
        void removeComponent()
        {
            for (auto it = m_components.begin(); it != m_components.end(); ++it)
            {
                auto casted = std::dynamic_pointer_cast<T>(*it);
                if (casted)
                {
                    m_components.erase(it);
                    LOG("Component of type ", typeid(T).name(), " removed successfully.");
                    return;
                }
            }
            ERROR("Component of type ", typeid(T).name(), " not found in the entity's component list.");
        }

        template <class T>
        static std::vector<std::shared_ptr<Entity>> getEntitiesWithComponent()
        {
            return VectorUtil::filter(s_entities, [&](std::shared_ptr<Entity> e)
                                      { return (e->getComponent<T>() != nullptr); });
        }

        void removeComponent(std::shared_ptr<Component> comp);

        /**
         * @brief Clears all components attached to the entity.
         */
        void clearComponents();

        /**
         * @brief Signal emitted when an entity is instantiated.
         */
        static Signal<std::shared_ptr<Entity>> onEntityInstantiated;

        /**
         * @brief Signal emitted when an entity is destroyed.
         */
        static Signal<std::shared_ptr<Entity>> onEntityDestroyed;

        /**
         * @brief Destroys the specified entity.
         *
         * @param e Pointer to the entity to destroy.
         */
        static void destroy(std::shared_ptr<Entity>);

        /**
         * @brief Instantiates the specified entity.
         *
         * @param e Pointer to the entity to instantiate.
         */
        static void instantiate(std::shared_ptr<Entity>);

        /**
         * @brief Gets all instantiated entities.
         *
         * @return A vector containing pointers to all instantiated entities.
         */
        static std::vector<std::shared_ptr<Entity>> getAllEntities();

        /**
         * @brief Gets the entity with the specified name.
         *
         * @param name The name of the entity to retrieve.
         * @return Pointer to the entity with the specified name, or nullptr if not found.
         */
        static std::shared_ptr<Entity> getEntityWithName(std::string name);

        std::shared_ptr<Transform> transform; // Pointer to the transform component of the entity.

    protected:
        std::string m_name; // Name of the entity.

        static std::vector<std::shared_ptr<Entity>> s_entities;                         // Vector containing pointers to all instantiated entities.
        static std::unordered_map<std::string, std::shared_ptr<Entity>> s_nameToEntity; // Map storing names and corresponding entities.
        static std::unordered_map<std::string, int> s_nameCount;                        // Map storing the count of entities with the same name.

        std::vector<std::shared_ptr<Component>> m_components; // Vector containing pointers to all components attached to the entity.
        bool m_enabled = true;                                // Flag indicating whether the entity is enabled.

        friend Editor::Editor;
    };

} // namespace wpwp

#endif // ENTITY_HPP
