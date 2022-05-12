#include "./GameController.hpp"
#include "../Entity.hpp"
#include "./GlobalState.hpp"
#include "../GameManager.hpp"

#include <random>
#include <algorithm>

const std::string GameController::GetOverReason()
{
    if (gameMode == GameMode::UNKNOWN)
        return "An unknown GameMode has been set";

    if (GetCard({ 0, static_cast<int>(cards.size()) - 1 }) != CardType::EMPTY) // opponent won
        return "Opponent won the game";
    
    if (GetCard({ 0, (static_cast<int>(cards.size()) - 1) * -1 }) != CardType::EMPTY) // player won
        return "Player won the game";
    
    return "";
}

std::vector<CardType> GameController::GetDeck() const
{
    return playerDeck;
}

bool GameController::IsGameOver()
{
    if (gameMode == GameMode::UNKNOWN)
        return true;
    
    bool topState, mostRightState;

    if (GetCard({ 0, static_cast<int>(cards.size()) - 1 }) != CardType::EMPTY)
    {
        {
            CardType state = GetCard({ 0, static_cast<int>(cards.size()) - 1 });
            topState = static_cast<int>(state) % 2 != 0;
        }
        mostRightState = GetCard({ 0, 0 }) == CardType::STATE_0_1 ? true : false;

        if (topState == mostRightState) // opponent won
            return true;
    }


    if (GetCard({ 0, (static_cast<int>(cards.size()) - 1) * -1 }) != CardType::EMPTY)
    {
        {
            CardType state = GetCard({ 0, (static_cast<int>(cards.size()) - 1) * -1 });
            topState = static_cast<int>(state) % 2 != 0;
        }
        mostRightState = GetCard({ (static_cast<int>(cards.size()) - 1), 0 }) == CardType::STATE_0_1 ? false : true;
        
        if (topState == mostRightState) // player won
            return true;
    }
    
    return false;
}

void GameController::FillDeck(std::vector<CardType>& deck)
{
    while (deck.size() != 5)
    {
        AddOneToDeck(deck);
    }
}

void GameController::AddOneToDeck(std::vector<CardType>& deck)
{
    deck.push_back(globalDeck.back());
    globalDeck.pop_back();
}

void GameController::AddOneToPlayerDeck()
{
    currentTurn = currentTurn == Turn::OPPONENT ? Turn::YOUR : Turn::OPPONENT;
    AddOneToDeck(playerDeck);
}

void GameController::FillPlayerDeck()
{
    FillDeck(playerDeck);
}

void GameController::InitSinglePlayerGame()
{
    gameMode = GameMode::SINGLEPLAYER_WITHOUT_NOT;
    currentTurn = (Turn)GetRandomValue(0, 1);    

    for (size_t i = 0; i < cards.size(); i++)
    {
        cards[i].val = GetRandomValue(0, 1) ? CardType::STATE_0_1 : CardType::STATE_1_0;
    }

    const std::array<CardType, 6> logicCards {
        CardType::AND_0, CardType::AND_1,
        CardType::OR_0, CardType::OR_1,
        CardType::XOR_0, CardType::XOR_1
    };

    for (auto type : logicCards)
    {
        for (int i = 0; i < 8; i++)
        {
            globalDeck.push_back(type);
        }
    }

    std::shuffle(globalDeck.begin(), globalDeck.end(), std::default_random_engine());

    FillDeck(playerDeck);
    FillDeck(opponentDeck);
}

void GameController::PlaceCardFromDeckIndex(unsigned short deckIndex, Vec2i cardPos)
{
    if (deckIndex > playerDeck.size())
        return;
    
    const auto placeableCards = GetPlaceableCards(cardPos);
    const auto chosenCard = playerDeck[deckIndex];

    if (std::find(placeableCards.begin(), placeableCards.end(), chosenCard) != placeableCards.end())
    {
        playerDeck.erase(std::find(playerDeck.begin(), playerDeck.end(), chosenCard));

        PlaceCard(chosenCard, cardPos);
    }
    
    if (playerDeck.empty())
        FillDeck(playerDeck);
}

void GameController::InitMultiPlayerGame(std::string code)
{
    gameMode = GameMode::UNKNOWN;
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

    std::vector<CardType> placeablePos {};

    if (mostRightState && mostLeftState)
        placeablePos = { CardType::AND_1, CardType::OR_1, CardType::XOR_0 };
    else if ((mostRightState && !mostLeftState) || (!mostRightState && mostLeftState))
        placeablePos = { CardType::AND_0, CardType::OR_1, CardType::XOR_1 };
    else placeablePos = { CardType::AND_0, CardType::OR_0, CardType::XOR_0 };
    
    if (cardPos.y == static_cast<int>(cards.size()) - 1)
    {
        bool initialRightState = GetCard({ 0, 0 }) == CardType::STATE_0_1 ? true : false;

        std::vector<CardType> filtered;
        std::copy_if(placeablePos.begin(), placeablePos.end(), std::back_inserter(filtered), [&](CardType i) {
            return initialRightState ? static_cast<int>(i) % 2 != 0 : static_cast<int>(i) % 2 == 0;
        });
        
        return filtered;
    }
    else if (cardPos.y == -(static_cast<int>(cards.size()) - 1))
    {
        bool initialRightState = GetCard({ static_cast<int>(cards.size()) - 1, 0 }) == CardType::STATE_0_1 ? false : true;

        std::vector<CardType> filtered;
        std::copy_if(placeablePos.begin(), placeablePos.end(), std::back_inserter(filtered), [&](CardType i) {
            return initialRightState ? static_cast<int>(i) % 2 != 0 : static_cast<int>(i) % 2 == 0;
        });

        return filtered;
    }

    return placeablePos;
}