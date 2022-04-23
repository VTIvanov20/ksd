#include "UIComponent.hpp"

void UIComponent::OnUpdate()
{
    SDLImGui::BeginImGuiDrawing();
    OnUI();
    SDLImGui::EndImGuiDrawing();
}