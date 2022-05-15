#pragma once
#include <memory>

/**
 * @brief An enumerator class for all card types
 */
enum class CardType
{
    STATE_0_1, STATE_1_0,
    AND_0, AND_1,
    OR_0, OR_1,
    XOR_0, XOR_1,
    EMPTY
};

/**
 * @brief An enumerator class for all vertical card position (up, initial binaries, down)
 */
enum class VerticalCardPos
{
    TOP,
    CENTER,
    BOTTOM
};

/**
 * @brief A node representing a card
 */
template <typename T>
struct Node
{
    std::shared_ptr<Node<T>> next;
    T val;
};

/**
 * @brief A beginning node representing an initial binary
 */
template <typename T>
struct BeginningNode
{
    std::shared_ptr<Node<T>> topNext;
    std::shared_ptr<Node<T>> bottomNext;
    T val;
};

/**
 * @brief An enumerator class for the current turn
 */
enum class Turn
{
    PLAYER,
    OPPONENT
};