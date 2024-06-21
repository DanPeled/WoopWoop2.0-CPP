#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>

namespace wpwp
{
    /**
     * @brief Base class for defining scenes in the game.
     */
    struct Scene
    {

    public:
        /**
         * @brief Virtual method to load the scene.
         */
        virtual void load();

        std::string getName() const { return m_name; }

    protected:
        std::string m_name; // The name of the scene.

        friend class SceneSerializer;
    };
}

#endif
