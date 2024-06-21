#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "ECS/Component.hpp"
#include "Material.hpp"

namespace wpwp
{
    /**
     * @brief Base class for rendering components.
     */
    struct Renderer : public Component
    {
    public:
        Material material{}; ///< Material associated with the renderer.
        virtual void onDrawGUI() override;
    };
} // namespace wpwp

#endif // RENDERER_HPP
