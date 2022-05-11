#pragma once
#include <memory>

enum class CardType
{
    STATE_0_1, STATE_1_0,
    AND_0, AND_1,
    OR_0, OR_1,
    XOR_0, XOR_1,
    EMPTY
};

enum class VerticalCardPos
{
    TOP,
    CENTER,
    BOTTOM
};

template <typename T>
struct Node
{
    std::shared_ptr<Node<T>> next;
    T val;
};

template <typename T>
struct BeginningNode
{
    std::shared_ptr<Node<T>> topNext;
    std::shared_ptr<Node<T>> bottomNext;
    T val;
};