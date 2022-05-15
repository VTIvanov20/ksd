#include "./GameController.hpp"
#include "../Entity.hpp"
#include "./GlobalState.hpp"
#include "../GameManager.hpp"
#include "./NetworkController.hpp"

#include <random>
#include <algorithm>

const std::string GameController::GetOverReason()
{
    /**
     * @brief Gets who won by the last card of the pyramid
     */
    if (gameMode == GameMode::UNKNOWN)
        return "An unknown GameMode has been set";

    if (gameMode == GameMode::MULTIPLAYER_WITHOUT_NOT)
    {
        if (!networkController.lock()->IsActive())
            return "Network isn't active";
    }

    if (GetCard({ 0, static_cast<int>(cards.size()) - 1 }) != CardType::EMPTY) // opponent won
        return "Opponent won the game";
    
    if (GetCard({ 0, (static_cast<int>(cards.size()) - 1) * -1 }) != CardType::EMPTY) // player won
        return "Player won the game";
    
    return "";
}

std::vector<CardType> GameController::GetDeck() const
{
    /**
     * @brief Returns the deck of the player
     */
    return playerDeck;
}

bool GameController::IsGameOver()
{
    /**
     * @brief If a game hasn't been started or gamemode is multiplayer returns true
     */
    if (gameMode == GameMode::UNKNOWN)
        return true;

    if (gameMode == GameMode::MULTIPLAYER_WITHOUT_NOT)
    {
        if (!networkController.lock()->IsActive())
            return true;
    }
    
    bool topState, mostRightState;

    /**
     * @brief Gets the top card and if it's the same as the right most card then returns true
     */
    if (GetCard({ 0, static_cast<int>(cards.size()) - 1 }) != CardType::EMPTY)
    {
        {
            CardType state = GetCard({ 0, static_cast<int>(cards.size()) - 1 });
            topState = static_cast<int>(state) % 2 != 0;
        }
        mostRightState = GetCard({ 0, 0 }) == CardType::STATE_0_1 ? true : false;

        /**
         * @brief If the game is multiplayer, leave the room
         */
        if (gameMode == GameMode::MULTIPLAYER_WITHOUT_NOT)
            networkController.lock()->LeaveRoom();

        if (topState == mostRightState)
            return true;
    }

    /**
     * @brief Same thing but for the player
     */
    if (GetCard({ 0, (static_cast<int>(cards.size()) - 1) * -1 }) != CardType::EMPTY)
    {
        {
            CardType state = GetCard({ 0, (static_cast<int>(cards.size()) - 1) * -1 });
            topState = static_cast<int>(state) % 2 != 0;
        }
        mostRightState = GetCard({ (static_cast<int>(cards.size()) - 1), 0 }) == CardType::STATE_0_1 ? false : true;
        
        if (gameMode == GameMode::MULTIPLAYER_WITHOUT_NOT)
            networkController.lock()->LeaveRoom();

        if (topState == mostRightState)
            return true;
    }
    
    return false;
}

void GameController::FillDeck(std::vector<CardType>& deck)
{
    /**
     * @brief If the player doesn't have 5 cards in his deck, add one to it
     */
    while (deck.size() != 5)
    {
        AddOneToDeck(deck);
    }
}

void GameController::AddOneToDeck(std::vector<CardType>& deck)
{
    /**
     * @brief Adds a card to the back of the deck
     */
    deck.push_back(globalDeck.back());
    globalDeck.pop_back();
}

void GameController::AddOneToPlayerDeck()
{
    /**
     * @brief Gets the current turn and adds a card to the players deck
     */
    currentTurn = currentTurn == Turn::OPPONENT ? Turn::PLAYER : Turn::OPPONENT;
    AddOneToDeck(playerDeck);
}

void GameController::FillPlayerDeck()
{
    /**
     * @brief Fills the deck of the player
     */
    FillDeck(playerDeck);
}

void GameController::InitSinglePlayerGame()
{
    /**
     * @brief Sets the gamemode to singeplayer and randomizes the current turn
     */
    gameMode = GameMode::SINGLEPLAYER_WITHOUT_NOT;
    currentTurn = (Turn)GetRandomValue(0, 1);    

    /**
     * @brief Randomizes the initial binaries
     */
    for (size_t i = 0; i < cards.size(); i++)
    {
        cards[i].val = GetRandomValue(0, 1) ? CardType::STATE_0_1 : CardType::STATE_1_0;
    }

    /**
     * @brief Fills all decks
     */
    FillGlobalDeck();

    FillDeck(playerDeck);
    FillDeck(opponentDeck);
}

void GameController::FillGlobalDeck()
{
    /**
     * @brief An array of fixed length of 6 for all types of cards
     */
    const std::array<CardType, 6> logicCards {
        CardType::AND_0, CardType::AND_1,
        CardType::OR_0, CardType::OR_1,
        CardType::XOR_0, CardType::XOR_1
    };

    /**
     * @brief For every type of card, adds 8 and then shuffles the whole deck
     */
    for (auto type : logicCards)
    {
        for (int i = 0; i < 8; i++)
        {
            globalDeck.push_back(type);
        }
    }

    std::shuffle(globalDeck.begin(), globalDeck.end(), std::default_random_engine());
}

void GameController::PlaceCardFromDeckIndex(unsigned short deckIndex, Vec2i cardPos)
{
    /**
     * @brief If the index is invalid, stop the function
     */
    if (deckIndex > playerDeck.size())
        return;
    
    /**
     * @brief Gets the placeable cards and the chosen card
     */
    const auto placeableCards = GetPlaceableCards(cardPos);
    const auto chosenCard = playerDeck[deckIndex];

    /**
     * @brief If the gamemode is multiplayer, send a packet
     */
    if (gameMode == GameMode::MULTIPLAYER_WITHOUT_NOT)
    {
        networkController.lock()->SendPacket(nlohmann::json({
            {"t", "place-card"},
            {"d", {
                {"deckIdx", deckIndex},
                {"pos", {{"x", cardPos.x}, {"y", cardPos.y}}}
            }}
        }));
    }
    else if (gameMode == GameMode::SINGLEPLAYER_WITHOUT_NOT)
    {
        /**
         * @brief If the gamemode is singleplayer, if the chosen card is found in the placeable cards, place it. If not, add a card to the players deck
         */
        if (std::find(placeableCards.begin(), placeableCards.end(), chosenCard) != placeableCards.end())
        {
            playerDeck.erase(std::find(playerDeck.begin(), playerDeck.end(), chosenCard));

            PlaceCard(chosenCard, cardPos);
        }

        if (playerDeck.size() < 5)
            AddOneToDeck(playerDeck);
    }
}

void GameController::DiscardCard(unsigned short deckIndex)
{
    /**
     * @brief If it's the opponents turn, stop the function
     */
    if (currentTurn == Turn::OPPONENT)
        return;
    
    /**
     * @brief Get the chosen card
     */
    const auto chosenCard = playerDeck[deckIndex];

    /**
     * @brief If the gamemode is multiplayer, send a packet
     */
    if (gameMode == GameMode::MULTIPLAYER_WITHOUT_NOT)
    {
        networkController.lock()->SendPacket(nlohmann::json({
            {"t", "discard-card"},
            {"d", {
                {"deckIdx", deckIndex}
            }}
        }));
    }
    else
    {
        /**
         * @brief If the gamemode is singleplayer, discard the card, add a card to the players deck and change the turn
         */
        playerDeck.erase(std::find(playerDeck.begin(), playerDeck.end(), chosenCard));
        currentTurn = currentTurn == Turn::OPPONENT ? Turn::PLAYER : Turn::OPPONENT;

        if (playerDeck.size() < 5)
            AddOneToDeck(playerDeck);
        if (globalDeck.empty())
            FillGlobalDeck();
    }
}

void GameController::InitMultiPlayerGame()
{
    /**
     * @brief Gets the instance of a network controller entity 
     */
    gameMode = GameMode::MULTIPLAYER_WITHOUT_NOT;

    auto networkControllerEntity =
        std::static_pointer_cast<Entity>(
            ObjectManager::GetInstance()->GetEntityFromTagName("network_controller").lock());
    networkController = MGetComponentFrom(networkControllerEntity, NetworkController);
}

std::array<BeginningNode<CardType>, 6> GameController::GetCards()
{
    /**
     * @brief Returns the cards array 
     */
    return cards;
}

Turn GameController::GetCurrentTurn()
{
    /**
     * @brief Returns the current turn 
     */
    return currentTurn;
}

std::vector<Vec2i> GameController::GetPlaceablePositions()
{
    /**
     * @brief A vector with the placeable positions
     */
    std::vector<Vec2i> out;

    /**
     * @brief Get every empty position and check if its two predecessor cards exist
     */
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

    /**
     * @brief Returns the vector with placeable positions
     */
    return out;
}

std::vector<Vec2i> GameController::GetOpponentPlaceablePositions()
{
    /**
     * @brief Same thing but for the opponent 
     */
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
    /**
     * @brief If the wanted type is an initial binary, stop the function
     */
    if (type == CardType::STATE_0_1 || type == CardType::STATE_1_0)
        return;

    /**
     * @brief If a card can be placed at the desired positon and its two predecessors exist, continue
     */
    if (CanPlaceCard(cardPos))
    {
        if (CanPlaceCard({ cardPos.x, cardPos.y + (cardPos.y > 0 ? -1 : 1) }) ||
            CanPlaceCard({ cardPos.x + 1, cardPos.y + (cardPos.y > 0 ? -1 : 1) }))
            return;

        currentTurn = currentTurn == Turn::OPPONENT ? Turn::PLAYER : Turn::OPPONENT;

        std::shared_ptr<Node<CardType>> card;

        /**
         * @brief If the card is in the players side, go up. If the card is in the opponents side, go down
         */
        if (cardPos.y < 0)
            card = cards[cardPos.x - cardPos.y].bottomNext;
        else if (cardPos.y > 0)
            card = cards[cardPos.x + cardPos.y].topNext;
        
        /**
         * @brief If it's the first card after the initial binaries, place it
         */
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

        /**
         * @brief If it's not the first card after the initial binaries, find it and then place it
         */
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

    /**
     * @brief If the position is invalid, return false
     */
    if (cardPos.y == 0 || cardPos.x < 0)
        return false;

    if (cardPos.y >= static_cast<int>(cards.size()) || cardPos.x >= static_cast<int>(cards.size()) - abs(cardPos.y))
        return false;

    /**
     * @brief If the card is in the players side, go up. If the card is in the opponents side, go down
     */
    if (cardPos.y > 0)
    {
        currentNode = cards[cardPos.x + cardPos.y].topNext;
    }
    else if (cardPos.y < 0)
    {
        currentNode = cards[cardPos.x - cardPos.y].bottomNext;
    }
    
    /**
     * @brief Continue going up/down until the desired card is found, then return true, else return false
     */
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

    /**
     * @brief If the position is invalid, return empty
     */
    if (cardPos.x < 0)
        return CardType::EMPTY;
    
    if (cardPos.y >= static_cast<int>(cards.size()) || cardPos.x >= static_cast<int>(cards.size()) - abs(cardPos.y))
        return CardType::EMPTY;

    /**
     * @brief If the card is in the players side, go up. If the card is in the opponents side, go down. If it's an initial binary, return an initial binary
     */
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
    
    /**
     * @brief Continue until we find the wanted card and then return it, if it's not found, return empty
     */
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
    /**
     * @brief If a card can be placed at the desired positon and its two predecessors exist, continue
     */
    if ((CanPlaceCard({ cardPos.x, cardPos.y + (cardPos.y > 0 ? -1 : 1) }) ||
        CanPlaceCard({ cardPos.x + 1, cardPos.y + (cardPos.y > 0 ? -1 : 1) })) && !CanPlaceCard(cardPos))
        return {};

    CardType mostLeft, mostRight;

    /**
     * @brief If the card is in the players side, get the upper most right and left card. If the card is in the opponents side, get the down most right and left card
     */
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

    /**
     * @brief If the left most card is an initial binary return true, else return false
     */
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

    /**
     * @brief If the right most card is an initial binary return true, else return false
     */
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

    /**
     * @brief Return cards depending on the value of their predecessors
     */
    if (mostRightState && mostLeftState)
        placeablePos = { CardType::AND_1, CardType::OR_1, CardType::XOR_0 };
    else if ((mostRightState && !mostLeftState) || (!mostRightState && mostLeftState))
        placeablePos = { CardType::AND_0, CardType::OR_1, CardType::XOR_1 };
    else placeablePos = { CardType::AND_0, CardType::OR_0, CardType::XOR_0 };
    
    /**
     * @brief If it's the final card, return the only possible cards to be placed
     */
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

void GameController::LeaveGame()
{
    /**
     * @brief Goes back to main menu
     */
    GameManager::GetInstance()->ChangeScene("res/scenes/main_menu.json");
}

void GameController::OnDestroy()
{
    /**
     * @brief If the game is multiplayer, leave the current room
     */
    if (gameMode == GameMode::MULTIPLAYER_WITHOUT_NOT)
        networkController.lock()->LeaveRoom();
}