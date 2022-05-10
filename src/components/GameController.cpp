#include "./GameController.hpp"

void GameController::OnCreate()
{
    currentTurn = (Turn)GetRandomValue(0, 1);

    for (size_t i = 0; i < cards.size(); i++)
    {
        cards[i].val = GetRandomValue(0, 1) ? CardType::STATE_0_1 : CardType::STATE_1_0;
    }
}

void GameController::OnUpdate()
{
    // if (currentTurn == Turn::OPPONENT)
    // {

    // }
}

std::array<BeginningNode<CardType>, 6> GameController::GetCards()
{
    return cards;
}

Turn GameController::GetCurrentTurn()
{
    return currentTurn;
}

std::vector<Vec2i> GameController::GetPlaceablePositions()
{
    std::vector<Vec2i> out;

    for (size_t i = 0; i < cards.size(); i++)
    {
        auto bottomNode = cards[i].bottomNext;

        if (bottomNode != nullptr)
        {
            Vec2i pos { static_cast<int>(i) - 1, -1 };
            while (bottomNode != nullptr)
            {
                bottomNode = bottomNode->next;
                pos.x--;
                pos.y--;
            } 

            if ((!CanPlaceCard({ pos.x, pos.y + 1 }) &&
                !CanPlaceCard({ pos.x + 1, pos.y + 1 })) && CanPlaceCard(pos))
                out.push_back(pos);
        } else if (i != 0) out.push_back({ static_cast<int>(i) - 1, -1 });
    }

    return out;
}

void GameController::PlaceCard(CardType type, Vec2i cardPos)
{
    if (type == CardType::STATE_0_1 || type == CardType::STATE_1_0)
        return;

    // can place a card (according to linked list)
    if (CanPlaceCard(cardPos))
    {
        // can place a card (according to rules)
        if (CanPlaceCard({ cardPos.x, cardPos.y + (cardPos.y > 0 ? -1 : 1) }) ||
            CanPlaceCard({ cardPos.x + 1, cardPos.y + (cardPos.y > 0 ? -1 : 1) }))
            return;

        std::shared_ptr<Node<CardType>> card;

        if (cardPos.y < 0)
            card = cards[cardPos.x - cardPos.y].bottomNext;
        else if (cardPos.y > 0)
            card = cards[cardPos.x + cardPos.y].topNext;
        
        if (card == nullptr)
        {
            if (cardPos.y < 0)
                cards[cardPos.x - cardPos.y].bottomNext = std::make_shared<Node<CardType>>(Node<CardType> {
                    nullptr,
                    type
                });
            else if (cardPos.y > 0)
                cards[cardPos.x + cardPos.y].topNext = std::make_shared<Node<CardType>>(Node<CardType> {
                    nullptr,
                    type
                });

            return;
        }

        while (card->next != nullptr)
            card = card->next;
        
        card->next = std::make_shared<Node<CardType>>(Node<CardType> {
            nullptr,
            type
        });
    }
}

bool GameController::CanPlaceCard(Vec2i cardPos)
{
    std::shared_ptr<Node<CardType>> currentNode;

    if (cardPos.y == 0 || cardPos.x < 0)
        return false;
    
    if (cardPos.y >= static_cast<int>(cards.size()) || cardPos.x >= static_cast<int>(cards.size()) - abs(cardPos.y))
        return false;

    if (cardPos.y > 0)
    {
        currentNode = cards[cardPos.x + cardPos.y].topNext;
    }
    else if (cardPos.y < 0)
    {
        currentNode = cards[cardPos.x - cardPos.y].bottomNext;
    }
    
    for (int y = 1; y <= abs(cardPos.y); y++)
    {
        if (currentNode == nullptr)
        {
            if (abs(cardPos.y) == y)
                return true;
            else return false;
        }
        currentNode = currentNode->next;
    }

    return false;
}

CardType GameController::GetCard(Vec2i cardPos)
{
    std::shared_ptr<Node<CardType>> currentNode;

    if (cardPos.x < 0)
        return CardType::EMPTY;
    
    if (cardPos.y >= static_cast<int>(cards.size()) || cardPos.x >= static_cast<int>(cards.size()) - abs(cardPos.y))
        return CardType::EMPTY;

    if (cardPos.y > 0)
    {
        currentNode = cards[cardPos.x + cardPos.y].topNext;
    }
    else if (cardPos.y < 0)
    {
        currentNode = cards[cardPos.x - cardPos.y].bottomNext;
    }
    
    for (int y = 1; y <= abs(cardPos.y); y++)
    {
        if (abs(cardPos.y) == y)
            return currentNode->val;
        currentNode = currentNode->next;
    }

    return CardType::EMPTY;
}