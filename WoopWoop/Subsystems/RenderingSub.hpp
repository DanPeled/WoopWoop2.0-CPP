#ifndef RENDERING_SUB_HPP
#define RENDERING_SUB_HPP

#include "WoopWoop.hpp"

namespace wpwp
{
    /**
     * @brief Subsystem responsible for rendering.
     */
    class RenderingSub : public Subsystem
    {
    public:
        /**
         * @brief Initializes the rendering subsystem.
         */
        void init() override;

        /**
         * @brief Updates the rendering subsystem.
         */
        void update() override {};
    };
} // namespace wpwp

#endif // RENDERING_SUB_HPP
