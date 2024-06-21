#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <SFML/Graphics.hpp>
#include "BaseComponent.hpp"
#include "Registry.hpp"
#include "Subsystems/Logging.hpp"
#include <memory>
#include <map>
#include <string>

#define NO_DRAW_GUI \
public:             \
    virtual void onDrawGUI() override {};

#define WREGISTER(type)                                                     \
    namespace type##_registration_detail                                    \
    {                                                                       \
        struct Factory                                                      \
        {                                                                   \
            Factory()                                                       \
            {                                                               \
                wpwp::Registry::getInstance().registerType(#type, &create); \
            }                                                               \
            static std::shared_ptr<wpwp::Component> create()                \
            {                                                               \
                return std::make_shared<type>();                            \
            }                                                               \
        };                                                                  \
        static Factory global_##type##Factory;                              \
    }

namespace wpwp
{
    class Entity;
    struct Transform;

    /**
     * @brief Component class for components attached to entities.
     */
    struct Component : BaseComponent
    {
    public:
        std::shared_ptr<Entity> entity;       // Pointer to the entity the component is attached to.
        std::shared_ptr<Transform> transform; // Pointer to the transform of the entity it's attached to.
        bool m_enabled;                       // Flag indicating whether the component is enabled.

    public:
        /**
         * @brief Attaches the component to an entity.
         *
         * @param e Pointer to the entity to attach to.
         */
        void attach(std::shared_ptr<Entity> e);

    public:
        Component(){};

        /**
         * @brief Called when the component is started.
         */
        virtual void start();

        /**
         * @brief Called every frame to update the component.
         */
        virtual void update();

        /**
         * @brief Called when the component is disabled.
         */
        virtual void onDisable();

        /**
         * @brief Virtual destructor.
         */
        virtual ~Component(){};

        NO_DRAW_GUI
        /**
         * @brief Gets the name of the component.
         *
         * @return The name of the component.
         */
        virtual std::string getName() const { return "component"; }
    };
} // namespace wpwp

#endif // COMPONENT_HPP
