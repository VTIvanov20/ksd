#pragma once

#include "Component.hpp"
#include "../util/graphics.hpp"

#include "./CardTypes.hpp"

enum class Turn
{
    YOUR,
    OPPONENT
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

    void PlaceCard(CardType type, Vec2i cardPos);
    std::vector<Vec2i> GetPlaceablePositions();
    std::vector<Vec2i> GetOpponentPlaceablePositions();
    std::vector<CardType> GetPlaceableCards(Vec2i cardPos);

    CardType GetCard(Vec2i cardPos);

private:
    bool CanPlaceCard(Vec2i cardPos); // alternative name: inverted CardExists

    std::array<BeginningNode<CardType>, 6> cards;
    Turn currentTurn;
};