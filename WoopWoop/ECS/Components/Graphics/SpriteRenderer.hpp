#ifndef SPRITE_RENDERER_HPP
#define SPRITE_RENDERER_HPP

#include "WoopWoop.hpp"

namespace wpwp
{
    /**
     * @brief Component for rendering sprites.
     */
    class SpriteRenderer : public Renderer
    {
    public:
        sf::Sprite sprite; // Sprite to render.

        /**
         * @brief Loads a sprite from the specified file path.
         *
         * @param path The file path of the sprite.
         */
        void loadSprite(std::string path);

        /**
         * @brief Updates the sprite renderer component.
         */
        void update() override;

        void start() override;

        /**
         * @brief Gets the name of the component.
         *
         * @return The name of the component.
         */
        std::string getName() const override { return "SpriteRenderer"; }
        std::string getFilePath() { return m_filePath; }
        void onDrawGUI() override;

    private:
        /**
         * @brief Loads a texture from the specified texture.
         *
         * @param texture Pointer to the texture.
         */
        void loadTexture(sf::Texture *texture);

        void syncTransform();

    private:
        sf::Texture m_texture; // Texture associated with the sprite.
        std::string m_filePath = "";
    };

    WREGISTER(SpriteRenderer)
} // namespace wpwp

#endif // SPRITE_RENDERER_HPP
