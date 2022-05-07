#include "./GameController.hpp"

void GameController::OnCreate()
{
    currentTurn = (Turn)GetRandomValue(0, 1);

    for (uint8_t i = 0; i < cards.size(); i++)
    {
        cards[i].val = GetRandomValue(0, 1) ? CardType::STATE_0_1 : CardType::STATE_1_0;
    }
}

void GameController::OnUpdate()
{
    
}

std::array<BeginningNode<CardType>, 6> GameController::GetCards()
{
    return cards;
}

Turn GameController::GetCurrentTurn()
{
    return currentTurn;
}

bool GameController::CanPlaceCard(Vec2i cardPos)
{
    std::shared_ptr<Node<CardType>> currentNode;

    if (cardPos.y == 0 || abs(cardPos.y) > abs(cardPos.x))
    {
        return false;
    }
    if (cardPos.y > 0)
    {
        currentNode = cards[abs(cardPos.x)].topNext;
    }
    else if (cardPos.y < 0)
    {
        currentNode = cards[abs(cardPos.x)].bottomNext;
    }
    
    for (int i = 0; i <= abs(cardPos.x); i++)
    {
        if (currentNode == nullptr && abs(cardPos.y) >= i)
        {
            return true;
        }
        currentNode = currentNode->next;
    }

    return false;
}