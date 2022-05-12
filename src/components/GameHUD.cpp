#include "./GameHUD.hpp"
#include "./GameController.hpp"
#include "../util/dear_imgui/imgui.h"

void GameHUD::OnUI()
{
    auto gameController = MGetComponent(GameController);
    const char *opponentTurn = "Opponent";
    const char *playerTurn = "Player";

    ImGui::Begin("HUD");
        ImGui::Text("Turn: %s", gameController->GetCurrentTurn() == Turn::OPPONENT ? opponentTurn : playerTurn);
        
        if (ImGui::Button("Get Card (skip turn)"))
        {
            gameController->AddOneToPlayerDeck();
        }
    ImGui::End();
}