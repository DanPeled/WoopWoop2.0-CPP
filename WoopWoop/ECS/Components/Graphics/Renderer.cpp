#include "Renderer.hpp"
#include "imgui/imgui.h"

void wpwp::Renderer::onDrawGUI()
{
    float col[4] = {
        material.color.r / 255.0f,
        material.color.g / 255.0f,
        material.color.b / 255.0f,
        material.color.a / 255.0f};

    ImGui::ColorEdit4("Color", col);

    material.color.r = col[0] * 255.0f;
    material.color.g = col[1] * 255.0f;
    material.color.b = col[2] * 255.0f;
    material.color.a = col[3] * 255.0f;
}