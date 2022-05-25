#pragma once

#include "Component.hpp"
#include "../util/graphics.hpp"

#include "./CardTypes.hpp"
#include "./NetworkController.hpp"

/**
 * @brief An enumerator class for the gamemodes
 */
enum class GameMode
{
    SINGLEPLAYER_WITHOUT_NOT,
    MULTIPLAYER_WITHOUT_NOT,
    UNKNOWN
};

class GameController : public Component
{
public:
    /**
     * @brief Define a component of type GameController
     */
    DEFINE_COMPONENT(GameController)
    ~GameController() = default;

    void OnCreate() override;
    void OnUpdate() override;
    void OnDestroy() override;

    /**
     * @brief Gets the current cards
     * 
     * @return Returns an array of BeginningNodes (initial binaries)
     */
    std::array<BeginningNode<CardType>, 6> GetCards();

    /**
     * @brief Gets the current turn
     * 
     * @return Returns a turn from the enum class Turn
     */
    Turn GetCurrentTurn();

    /**
     * @brief Places a card from a certain type at a certain position
     * 
     * @param type The type of card we want to place
     * 
     * @param cardPos The position, at which we want to place the card
     */
    void PlaceCard(CardType type, Vec2i cardPos);

    /**
     * @brief Place a card from the deck by index at a certain position
     * 
     * @param deckIndex The index of the card in the deck we want to place
     * 
     * @param cardPos The position, at which we want to place the card
     */
    void PlaceCardFromDeckIndex(unsigned short deckIndex, Vec2i cardPos);

    /**
     * @brief Gets all placeable positions
     * 
     * @return Returns a vector with all placeable positions
     */
    std::vector<Vec2i> GetPlaceablePositions();

    /**
     * @brief Gets all opponent placeable positions
     * 
     * @return Returns a vector with all opponent placeable position
     */
    std::vector<Vec2i> GetOpponentPlaceablePositions();

    /**
     * @brief Gets all placeable cards at a certain position
     * 
     * @param cardPos The position, at which we want to get the placeable cards
     * 
     * @return Returns a vector with all placeable cards at a certain position
     */
    std::vector<CardType> GetPlaceableCards(Vec2i cardPos);

    /**
     * @brief Gets the current deck
     * 
     * @return Returns a vector of the current deck
     */
    std::vector<CardType> GetDeck() const;

    /**
     * @brief Fills up the players deck
     */
    void FillPlayerDeck();

    /**
     * @brief Adds one card to the players deck
     */
    void AddOneToPlayerDeck();

    /**
     * @brief Discards a card from the players deck at a certain index
     * 
     * @param deckIndex Unsigned short for the wanted index
     */
    void DiscardCard(unsigned short deckIndex);

    /**
     * @brief Fills the global deck
     */
    void FillGlobalDeck();

    /**
     * @brief Gets if the game is over
     * 
     * @return Returns a bool whether the game has finished
     */
    bool IsGameOver();

    /**
     * @brief Gets why the game is over
     * 
     * @return Returns a string containing the reason the game has finished
     */
    const std::string GetOverReason();

    /**
     * @brief Gets the card type of a card at a certain position
     * 
     * @param cardPos The position, at which we want to get the placeable cards
     * 
     * @return Returns the card type at a certain positon
     */
    CardType GetCard(Vec2i cardPos);

    /**
     * @brief Leaves the game
     */
    void LeaveGame();

private:
    /**
     * @brief Initializes a new singleplayer game
     */
    void InitSinglePlayerGame();

    /**
     * @brief Initializes a new multiplayer game
     */
    void InitMultiPlayerGame();

    /**
     * @brief Fills the deck
     * 
     * @param deck A reference to the current deck
     */
    void FillDeck(std::vector<CardType>& deck);

    /**
     * @brief Adds one card to the deck
     * 
     * @param deck A reference to the current deck
     */
    void AddOneToDeck(std::vector<CardType>& deck);

    /**
     * @brief Gets if a card can be placed at a certain position
     * 
     * @param cardPos The position, at which we want to get the placeable cards
     * 
     * @return Returns a bool whether a card can be placed at a certain position
     */
    bool CanPlaceCard(Vec2i cardPos);

    /**
     * @brief The array with fixed length of cards
     */
    std::array<BeginningNode<CardType>, 6> cards {};

    /**
     * @brief The string with the multiplayer code
     */
    std::string multiplayerCode;

    /**
     * @brief The array of the global deck of cards
     */
    std::vector<CardType> globalDeck;

    /**
     * @brief The array of the deck of discarded cards
     */
    std::vector<CardType> discardDeck;

    /**
     * @brief The array of the players deck of cards
     */
    std::vector<CardType> playerDeck;

    /**
     * @brief The array of the opponents deck of cards
     */
    std::vector<CardType> opponentDeck;

    /**
     * @brief The current gamemode of type the enum class GameMode
     */
    GameMode gameMode = GameMode::UNKNOWN;

    /**
     * @brief The current turn of type the enum class Turn
     */
    Turn currentTurn;

    /**
     * @brief The networkController of type class NetworkController
     */
    std::weak_ptr<NetworkController> networkController {};
};