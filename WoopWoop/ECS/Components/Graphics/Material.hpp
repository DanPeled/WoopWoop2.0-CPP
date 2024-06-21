#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <SFML/Graphics.hpp>

namespace wpwp
{
    /**
     * @brief Struct representing material properties.
     */
    struct Material
    {
        sf::Color color; ///< Color of the material.

        /**
         * @brief Default constructor.
         */
        Material() : color(sf::Color::White) {}
    };
} // namespace wpwp

#endif // MATERIAL_HPP
