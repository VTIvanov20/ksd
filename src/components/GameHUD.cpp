#include "./GameHUD.hpp"
#include "./GameController.hpp"
#include "../util/dear_imgui/imgui.h"

bool DrawImageButton(CardType type)
{
    std::string text;

    switch(type)
    {
    case CardType::AND_0:
        text = "AND_0";
        break;
    case CardType::AND_1:
        text = "AND_1";
        break;
    case CardType::OR_0:
        text = "OR_0";
        break;
    case CardType::OR_1:
        text = "OR_1";
        break;
    case CardType::XOR_0:
        text = "XOR_0";
        break;
    case CardType::XOR_1:
        text = "XOR_1";
        break;
    case CardType::STATE_1_0:
        text = "STATE_1_0";
        break;
    case CardType::STATE_0_1:
        text = "STATE_0_1";
        break;
    case CardType::EMPTY: {}
    }

    return ImGui::Button(text.c_str());
}

void GameHUD::OnUI()
{
    auto gameController = MGetComponent(GameController);

    if (gameController->IsGameOver())
        return;

    const char *opponentTurn = "Opponent";
    const char *playerTurn = "Player";

    ImGui::Begin("HUD");
        if (ImGui::BeginPopup("card_discard_popup"))
        {
            const auto cards = gameController->GetDeck();
            for (unsigned short i = 0; i < static_cast<unsigned short>(cards.size()); i++)
            {
                auto type = cards[i];

                ImGui::SameLine();
                if(DrawImageButton(type))
                {
                    gameController->DiscardCard(i);
                    ImGui::CloseCurrentPopup();
                }
            }
            
            ImGui::EndPopup();
        }
        ImGui::Text("Turn: %s", gameController->GetCurrentTurn() == Turn::OPPONENT ? opponentTurn : playerTurn);
        
        if (ImGui::Button("Discard"))
        {
            // gameController->AddOneToPlayerDeck();
            if (gameController->GetCurrentTurn() == Turn::PLAYER)
                ImGui::OpenPopup("card_discard_popup");
        }
    ImGui::End();
}