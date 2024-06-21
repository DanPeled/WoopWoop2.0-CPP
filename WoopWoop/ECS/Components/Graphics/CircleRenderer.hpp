#ifndef CIRCLE_RENDERER_HPP
#define CIRCLE_RENDERER_HPP

#include "Renderer.hpp"
#include <SFML/Graphics.hpp>

namespace wpwp
{
    /**
     * @brief Component for rendering circles.
     */
    struct CircleRenderer : public Renderer
    {
    public:
        /**
         * @brief Called when the circle renderer component is started.
         */
        void start() override;

        /**
         * @brief Called every frame to update the circle renderer component.
         */
        void update() override;

        /**
         * @brief Gets the name of the component.
         *
         * @return The name of the component.
         */
        std::string getName() const override { return "CircleRenderer"; }

    private:
        sf::CircleShape m_circleShape; // Circle shape to render.
    };

    WREGISTER(CircleRenderer)
} // namespace wpwp

#endif // CIRCLE_RENDERER_HPP
