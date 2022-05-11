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
    auto placeablePositions = GetOpponentPlaceablePositions();

    if (currentTurn == Turn::OPPONENT && placeablePositions.size() != 0)
    {
        auto pos = placeablePositions[GetRandomValue(0, placeablePositions.size() - 1)];
        auto placeableCards = GetPlaceableCards(pos);
        int cardIdx = GetRandomValue(0, placeableCards.size() - 1);

        PlaceCard(placeableCards[cardIdx], pos);
    }
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

std::vector<Vec2i> GameController::GetOpponentPlaceablePositions()
{
    std::vector<Vec2i> out;

    for (size_t i = 0; i < cards.size(); i++)
    {
        auto topNode = cards[i].topNext;

        if (topNode != nullptr)
        {
            Vec2i pos { static_cast<int>(i) - 1, 1 };
            while (topNode != nullptr)
            {
                topNode = topNode->next;
                pos.x--;
                pos.y++;
            }

            if ((!CanPlaceCard({ pos.x, pos.y - 1 }) &&
                !CanPlaceCard({ pos.x + 1, pos.y - 1 })) && CanPlaceCard(pos))
                out.push_back(pos);
        } else if (i != 0) out.push_back({ static_cast<int>(i) - 1, 1 });
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
        
        currentTurn = currentTurn == Turn::OPPONENT ? Turn::YOUR : Turn::OPPONENT;

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
    else if (cardPos.y == 0)
    {
        return cards[cardPos.x].val;
    }
    
    for (int y = 1; y <= abs(cardPos.y); y++)
    {
        if (currentNode == nullptr)
            return CardType::EMPTY;
        
        if (abs(cardPos.y) == y)
            return currentNode->val;
        currentNode = currentNode->next;
    }

    return CardType::EMPTY;
}

std::vector<CardType> GameController::GetPlaceableCards(Vec2i cardPos)
{
    // sanity check
    if ((CanPlaceCard({ cardPos.x, cardPos.y + (cardPos.y > 0 ? -1 : 1) }) ||
        CanPlaceCard({ cardPos.x + 1, cardPos.y + (cardPos.y > 0 ? -1 : 1) })) && !CanPlaceCard(cardPos))
        return {};

    CardType mostLeft, mostRight;

    if (cardPos.y < 0)
    {
        mostLeft = GetCard({ cardPos.x, cardPos.y + 1 });
        mostRight = GetCard({ cardPos.x + 1, cardPos.y + 1 });
    }
    else if (cardPos.y > 0)
    {
        mostLeft = GetCard({ cardPos.x, cardPos.y - 1 });
        mostRight = GetCard({ cardPos.x + 1, cardPos.y - 1 });
    }

    bool mostLeftState, mostRightState;

    switch (mostLeft)
    {
        case CardType::STATE_0_1:
            if (cardPos.y > 0)
                mostLeftState = true;
            else if (cardPos.y < 0)
                mostLeftState = false;
            break;
        case CardType::STATE_1_0:
            if (cardPos.y > 0)
                mostLeftState = false;
            else if (cardPos.y < 0)
                mostLeftState = true;
            break;
        case CardType::AND_0:
            mostLeftState = false;
            break;
        case CardType::AND_1:
            mostLeftState = true;
            break;
        case CardType::OR_0:
            mostLeftState = false;
            break;
        case CardType::OR_1:
            mostLeftState = true;
            break;
        case CardType::XOR_0:
            mostLeftState = false;
            break;
        case CardType::XOR_1:
            mostLeftState = true;
            break;
        default: return {};
    }

    switch (mostRight)
    {
        case CardType::STATE_0_1:
            if (cardPos.y > 0)
                mostRightState = true;
            else if (cardPos.y < 0)
                mostRightState = false;
            break;
        case CardType::STATE_1_0:
            if (cardPos.y > 0)
                mostRightState = false;
            else if (cardPos.y < 0)
                mostRightState = true;
            break;
        case CardType::AND_0:
            mostRightState = false;
            break;
        case CardType::AND_1:
            mostRightState = true;
            break;
        case CardType::OR_0:
            mostRightState = false;
            break;
        case CardType::OR_1:
            mostRightState = true;
            break;
        case CardType::XOR_0:
            mostRightState = false;
            break;
        case CardType::XOR_1:
            mostRightState = true;
            break;
        default: return {};
    }

    if (mostRightState && mostLeftState)
        return { CardType::AND_1, CardType::OR_1, CardType::XOR_0 };
    else if ((mostRightState && !mostLeftState) || (!mostRightState && mostLeftState))
        return { CardType::AND_0, CardType::OR_1, CardType::XOR_1 };
    else return { CardType::AND_0, CardType::OR_0, CardType::XOR_0 };
}