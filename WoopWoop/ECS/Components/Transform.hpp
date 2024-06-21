#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "WoopWoop.hpp"

namespace wpwp
{
    /**
     * @brief Component representing the transform of an entity.
     */
    class Transform : public Component
    {
    public:
#pragma region World Position
        /**
         * @brief Gets the position of the transform.
         *
         * @return A pointer to the position vector.
         */
        sf::Vector3f *getPosition();

        /**
         * @brief Sets the position of the transform.
         *
         * @param position The new position vector.
         */
        void setPosition(const sf::Vector3f &position);

        /**
         * @brief Gets the rotation of the transform.
         *
         * @return A pointer to the rotation vector.
         */
        sf::Vector3f *getRotation();

        /**
         * @brief Sets the rotation of the transform.
         *
         * @param rotation The new rotation vector.
         */
        void setRotation(const sf::Vector3f &rotation);

        /**
         * @brief Gets the scale of the transform.
         *
         * @return A pointer to the scale vector.
         */
        sf::Vector3f *getScale();

        /**
         * @brief Sets the scale of the transform.
         *
         * @param scale The new scale vector.
         */
        void setScale(const sf::Vector3f &scale);
#pragma endregion World Position

#pragma region Child Parent Relation
        /**
         * @brief Adds a child entity by name.
         *
         * @param name The name of the child entity.
         */
        void addChild(std::string name);

        /**
         * @brief Adds a child entity.
         *
         * @param child The child entity to add.
         */
        void addChild(std::shared_ptr<Entity> child);

        /**
         * @brief Gets the list of child entities.
         *
         * @return A vector of pointers to the child entities.
         */
        std::vector<std::shared_ptr<Entity>> getChildren() const;

        /**
         * @brief Gets the count of child entities.
         *
         * @return The number of child entities.
         */
        std::size_t getChildCount();
#pragma endregion Child Parent Relation

        /**
         * @brief A signal that is invoked whenever the transform is changed.
         */
        Signal<> onTransformChanged;

        /**
         * @brief Gets the name of the component.
         *
         * @return The name of the component.
         */
        std::string getName() const override { return "Transform"; }
        virtual void onDrawGUI() override;

    private:
        sf::Vector3f m_globalPosition;                   // Global position of the entity.
        sf::Vector3f m_scale = sf::Vector3f(1, 1, 1);    // Scale of the entity.
        sf::Vector3f m_rotation = sf::Vector3f(0, 0, 0); // Rotation of the entity.

        std::vector<std::string> m_childrenNames; // Names of the child entities.
    };

    WREGISTER(Transform)

} // namespace wpwp

#endif // TRANSFORM_HPP
