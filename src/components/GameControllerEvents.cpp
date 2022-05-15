#include "./GameController.hpp"
#include "../Entity.hpp"
#include "./GlobalState.hpp"
#include "../GameManager.hpp"
#include "./NetworkController.hpp"

#include <algorithm>

/**
 * @brief Called when the game controller component is created
 */
void GameController::OnCreate()
{
    /**
     * @brief Gets a global state component and if it has a value initializes a game
     */
    auto globalState = std::static_pointer_cast<Entity>(
        ObjectManager::GetInstance()->GetEntityFromTagName("global_state").lock());
    auto optMode = MGetComponentFrom(globalState, GlobalState)->GetValue("mode");

    if (optMode.has_value())
    {
        auto mode = *optMode;

        if (mode == "singleplayer")
            InitSinglePlayerGame();
        else if (mode == "multiplayer")
            InitMultiPlayerGame();
        else gameMode = GameMode::UNKNOWN;
    }
}

void GameController::OnUpdate()
{
    /**
     * @brief If the gamemode is multiplayer, gets the networkController component and sets the default vaues
     */
    if (gameMode == GameMode::MULTIPLAYER_WITHOUT_NOT)
    {
        auto networkControllerEntity =
        std::static_pointer_cast<Entity>(
            ObjectManager::GetInstance()->GetEntityFromTagName("network_controller").lock());
        auto networkController = MGetComponentFrom(networkControllerEntity, NetworkController);

        NetworkState nState = networkController->GetState();
        cards = nState.cardState;
        currentTurn = nState.currentTurn;
        playerDeck = nState.playerDeck;

        if (!nState.gameStarted)
            GameManager::GetInstance()->ChangeScene("res/scenes/main_menu.json");
    }
    /**
     * @brief If the gamemode is singleplayer, the AI gets the opponent placeable positions and cards and thinks and then places
     */
    else if (gameMode == GameMode::SINGLEPLAYER_WITHOUT_NOT)
    {
        auto placeablePositions = GetOpponentPlaceablePositions();

        if (currentTurn == Turn::OPPONENT && placeablePositions.size() != 0)
        {
            const auto pos = placeablePositions[GetRandomValue(0, placeablePositions.size() - 1)];
            auto placeableCards = GetPlaceableCards(pos);
            std::vector<CardType> opponentDeckSorted(opponentDeck.size());
            std::vector<CardType> crossSection;

            std::sort(placeableCards.begin(), placeableCards.end());
            std::partial_sort_copy(opponentDeck.cbegin(), opponentDeck.cend(),
                opponentDeckSorted.begin(), opponentDeckSorted.end());

            std::set_intersection(
                placeableCards.begin(), placeableCards.end(),
                opponentDeckSorted.begin(), opponentDeckSorted.end(), std::back_inserter(crossSection)
            );

            if (crossSection.empty())
            {
                AddOneToDeck(opponentDeck);
                currentTurn = currentTurn == Turn::OPPONENT ? Turn::PLAYER : Turn::OPPONENT;
                return;
            }

            auto chosenCard = crossSection.at(GetRandomValue(0, crossSection.size() - 1));
            opponentDeck.erase(std::find(opponentDeck.begin(), opponentDeck.end(), chosenCard));

            PlaceCard(chosenCard, pos);

            /**
             * @brief If the AI deck is empty, fill it
             */
            if (opponentDeck.empty())
                FillDeck(opponentDeck);
        }
    }
}