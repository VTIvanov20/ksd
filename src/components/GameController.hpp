#pragma once

#include "Component.hpp"
#include "../util/graphics.hpp"

#include "./CardTypes.hpp"

class GameController : public Component
{
public:
    DEFINE_COMPONENT(GameController)
    ~GameController() = default;

    void OnCreate() override;
    void OnUpdate() override;

    std::array<BeginningNode<CardType>, 6> GetCards();

private:
    std::array<BeginningNode<CardType>, 6> cards;
};