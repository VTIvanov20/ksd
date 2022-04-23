#include "MainMenu.hpp"

void MainMenu::OnUI()
{
    ImGui::Begin("Main Menu");
        ImGui::Button("Singleplayer");
        ImGui::Button("Multiplayer");
    ImGui::End();
}