#pragma once

#include "Component.hpp"
#include "../util/graphics.hpp"

#include "./CardTypes.hpp"

enum class Turn
{
    PLAYER,
    OPPONENT
};

enum class GameMode
{
    SINGLEPLAYER_WITHOUT_NOT,
    MULTIPLAYER_WITHOUT_NOT,
    SINGLEPLAYER_WITH_NOT,
    MULTIPLAYER_WITH_NOT,
    UNKNOWN
};

class GameController : public Component
{
public:
    DEFINE_COMPONENT(GameController)
    ~GameController() = default;

    void OnCreate() override;
    void OnUpdate() override;

    std::array<BeginningNode<CardType>, 6> GetCards();
    Turn GetCurrentTurn();

    void PlaceCard(CardType type, Vec2i cardPos); // does not perform card type checking,
                                                  // that's the frontend's job with GetPlaceableCards
    void PlaceCardFromDeckIndex(unsigned short deckIndex, Vec2i cardPos); // places from playerDeck
    std::vector<Vec2i> GetPlaceablePositions();

    std::vector<Vec2i> GetOpponentPlaceablePositions();

    // multiplayer and singleplayer
    std::vector<CardType> GetPlaceableCards(Vec2i cardPos);
    std::vector<CardType> GetDeck() const; // returns playerDeck

    void FillPlayerDeck();
    void AddOneToPlayerDeck();

    bool IsGameOver();
    const std::string GetOverReason();

    CardType GetCard(Vec2i cardPos);

private:
    void InitSinglePlayerGame();
    void InitMultiPlayerGame(std::string code);

    void FillDeck(std::vector<CardType>& deck);
    void AddOneToDeck(std::vector<CardType>& deck);

    bool CanPlaceCard(Vec2i cardPos); // alternative name: inverted CardExists

    std::array<BeginningNode<CardType>, 6> cards;
    std::string multiplayerCode;

    // multiplayer and singleplayer
    std::vector<CardType> globalDeck;
    std::vector<CardType> playerDeck;

    // singleplayer only
    std::vector<CardType> opponentDeck;

    GameMode gameMode = GameMode::UNKNOWN;
    Turn currentTurn;
};