#include "Transform.hpp"

namespace wpwp
{
    sf::Vector3f *Transform::getPosition() { return &m_globalPosition; }

    void Transform::setPosition(const sf::Vector3f &position)
    {
        m_globalPosition = position;
        onTransformChanged.invoke();
    }

    sf::Vector3f *Transform::getRotation() { return &m_rotation; }

    void Transform::setRotation(const sf::Vector3f &rotation)
    {
        m_rotation = rotation;
        onTransformChanged.invoke();
    }

    sf::Vector3f *Transform::getScale() { return &m_scale; }

    void Transform::setScale(const sf::Vector3f &scale)
    {
        m_scale = scale;
        onTransformChanged.invoke();
    }

    void Transform::addChild(std::string name)
    {
        if (std::find(m_childrenNames.begin(), m_childrenNames.end(), name) == m_childrenNames.end())
            m_childrenNames.push_back(name);
    }

    void Transform::addChild(std::shared_ptr<Entity> child)
    {
        addChild(child->getName());
    }

    std::vector<std::shared_ptr<Entity>> Transform::getChildren() const
    {
        return VectorUtil::select(m_childrenNames, [](std::string name)
                                  { return Entity::getEntityWithName(name); });
    }

    std::size_t Transform::getChildCount()
    {
        return m_childrenNames.size();
    }

    void Transform::onDrawGUI()
    {
        ImGui::SetWindowFontScale(1.1);
        ImGui::Text("Position: ");
        ImGui::SetWindowFontScale(1);

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); // Red
        ImGui::Text("X");
        ImGui::PopStyleColor();
        ImGui::SameLine();
        float posX = m_globalPosition.x;
        if (ImGui::InputFloat("##XPosition", &posX))
        {
            m_globalPosition.x = posX;
            onTransformChanged.invoke();
        }

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); // Green
        ImGui::Text("Y");
        ImGui::PopStyleColor();
        ImGui::SameLine();
        float posY = m_globalPosition.y;
        if (ImGui::InputFloat("##YPosition", &posY))
        {
            m_globalPosition.y = posY;
            onTransformChanged.invoke();
        }

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 1.0f, 1.0f)); // Blue
        ImGui::Text("Z");
        ImGui::PopStyleColor();
        ImGui::SameLine();
        float posZ = m_globalPosition.z;
        if (ImGui::InputFloat("##ZPosition", &posZ))
        {
            m_globalPosition.z = posZ;
            onTransformChanged.invoke();
        }

        ImGui::Dummy(ImVec2(0.0f, 20.0f));
        ImGui::Separator();
        ImGui::SetWindowFontScale(1.1);
        ImGui::Text("Scale: ");
        ImGui::PushID("Scale");
        ImGui::SetWindowFontScale(1);

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); // Red
        ImGui::Text("X");
        ImGui::PopStyleColor();
        ImGui::SameLine();
        float scaleX = m_scale.x;
        if (ImGui::InputFloat("##XScale", &scaleX))
        {
            m_scale.x = scaleX;
            onTransformChanged.invoke();
        }

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); // Green
        ImGui::Text("Y");
        ImGui::PopStyleColor();
        ImGui::SameLine();
        float scaleY = m_scale.y;
        if (ImGui::InputFloat("##YScale", &scaleY))
        {
            m_scale.y = scaleY;
            onTransformChanged.invoke();
        }

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 1.0f, 1.0f)); // Blue
        ImGui::Text("Z");
        ImGui::PopStyleColor();
        ImGui::SameLine();
        float scaleZ = m_scale.z;
        if (ImGui::InputFloat("##ZScale", &scaleZ))
        {
            m_scale.z = scaleZ;
            onTransformChanged.invoke();
        }

        ImGui::PopID();
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
        ImGui::Separator();
        ImGui::SetWindowFontScale(1.1);
        ImGui::Text("Rotation: ");
        ImGui::PushID("Rotation");

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); // Red
        ImGui::Text("X");
        ImGui::PopStyleColor();
        ImGui::SameLine();
        float rotX = m_rotation.x;
        if (ImGui::InputFloat("##XRotation", &rotX))
        {
            m_rotation.x = rotX;
            onTransformChanged.invoke();
        }

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); // Green
        ImGui::Text("Y");
        ImGui::PopStyleColor();
        ImGui::SameLine();
        float rotY = m_rotation.y;
        if (ImGui::InputFloat("##YRotation", &rotY))
        {
            m_rotation.y = rotY;
            onTransformChanged.invoke();
        }

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 1.0f, 1.0f)); // Blue
        ImGui::Text("Z");
        ImGui::PopStyleColor();
        ImGui::SameLine();
        float rotZ = m_rotation.z;
        if (ImGui::InputFloat("##ZRotation", &rotZ))
        {
            m_rotation.z = rotZ;
            onTransformChanged.invoke();
        }

        ImGui::PopID();
    }

}