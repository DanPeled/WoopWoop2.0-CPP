#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "WoopWoop.hpp"
#include "../ECS/Entity.hpp"
#include "../ECS/Component.hpp"
#include <vector>

#define NO_OPEN_IMGUI (bool *)__null
#define NO_SCROLL_IMGUI ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
#define NON_CHANGABLE_IMGUI ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove

namespace wpwp::Editor
{
    /// @brief Editor class for managing editor functionalities.
    class Editor : public wpwp::Subsystem
    {
    public:
        /// @brief Initialize the editor.
        void init() override;

        /// @brief Update the editor.
        void update() override;

    private:
        /// @brief Update the editor state.
        void editorUpdate();

        /** @brief Select an entity by its index.
         *
         * @param index The index of the entity to select.
         */
        void selectEntity(int index);

        /** @brief Handle the drawing of tree selection.
         *
         * @param index The index of the selected entity.
         */
        void handleEntityTreeSelectionDrawing(int index, bool &canOpenPopup);

    private:
        bool m_editorActive = true; // Flag indicating whether the editor is active.

        std::vector<std::string> m_names; // List of entity names.

#pragma region DEBUG_VALUES
        int m_selectedNameIndex = -1;                         // Index of the selected entity name.
        std::shared_ptr<Entity> m_selectedEntity = nullptr;   // Pointer to the selected entity.
        std::vector<std::string> m_componentNames;            // List of component names.
        std::vector<std::shared_ptr<Component>> m_components; // List of components.
#pragma endregion
    };
};

#endif
