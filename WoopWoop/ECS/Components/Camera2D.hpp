#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "WoopWoop.hpp"

namespace wpwp
{
    /**
     * @brief Component representing a camera in the game.
     */
    class Camera2D : public Component
    {
    public:
        Camera2D();

        /**
         * @brief Called when the camera component is started.
         */
        void start() override;

        /**
         * @brief Called every frame to update the camera component.
         */
        void update() override;

        bool isMain; // Flag indicating whether the camera is the main camera.

        void onDrawGUI() override;

        /**
         * @brief Gets the name of the component.
         *
         * @return The name of the component.
         */
        std::string getName() const override { return "Camera2D"; }

        sf::Vector2f getCenter() const;

        void setCenter(const sf::Vector2f &center);

        sf::Vector2f getViewSize() const { return m_view.getSize(); }
        void setViewSize(const sf::Vector2f &size);

    private:
        sf::View m_view; // SFML view associated with the camera.
    };

    WREGISTER(Camera2D);
} // namespace wpwp

#endif // CAMERA_HPP
