#pragma once

enum class CardType
{
    STATE_0_1, STATE_1_0,
    AND_0, AND_1,
    OR_0, OR_1,
    XOR_0, XOR_1
};

enum class VerticalCardPos
{
    TOP,
    BOTTOM
};

template <typename T>
struct Node
{
    std::shared_ptr<Node<T>> left;
    std::shared_ptr<Node<T>> right;
    T val;
};

template <typename T>
struct BeginningNode
{
    std::shared_ptr<Node<T>> topL;
    std::shared_ptr<Node<T>> topR;
    std::shared_ptr<Node<T>> bottomL;
    std::shared_ptr<Node<T>> bottomR;
    T val;
};