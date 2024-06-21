#ifndef IMGUI_SUB_HPP
#define IMGUI_SUB_HPP

#include "Util/Subsystem.hpp"
#include "WoopWoop.hpp"

namespace wpwp
{
    /**
     * @brief Subsystem responsible for ImGui integration.
     */
    class ImGuiSubsystem : public Subsystem
    {
    public:
        /**
         * @brief Initializes the ImGui subsystem.
         */
        void init() override;

        /**
         * @brief Updates the ImGui subsystem.
         */
        void update() override;

        /**
         * @brief Cleans up the ImGui subsystem.
         */
        void onStop() override;

    private:
        sf::Clock m_deltaClock; // SFML clock to measure delta time.
    };
} // namespace wpwp

#endif // IMGUI_SUB_HPP
