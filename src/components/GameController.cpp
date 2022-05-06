#include "./GameController.hpp"

void GameController::OnCreate()
{
    for (uint8_t i = 0; i < cards.size(); i++)
        cards[i].val = GetRandomValue(0, 1) ? CardType::STATE_0_1 : CardType::STATE_1_0;
}

void GameController::OnUpdate()
{
    
}

std::array<BeginningNode<CardType>, 6> GameController::GetCards()
{
    return cards;
}