#pragma once

#include "./Component.hpp"

#include "./CardTypes.hpp"
#include "../util/json.hpp"

struct NetworkState
{
public:
    std::string code;
    std::string lastErrorReason;
    bool hasError = false;
    std::array<BeginningNode<CardType>, 6> cardState;
    std::vector<CardType> playerDeck;
    Turn currentTurn;
    bool inGame = false;
    bool gameStarted = false;
};

class NetworkController : public Component
{
public:
    DEFINE_COMPONENT(NetworkController);

    void OnCreate() override;
    void OnUpdate() override;
    void OnDestroy() override;

    void JoinRoom(std::string code);
    void LeaveRoom();
    void CreateRoom();

    void SendPacket(nlohmann::json);
    NetworkState GetState();

    bool IsActive();

    std::string WsUri = "ws://localhost:8080";

    bool DestroyOnReload() override { return false; };

private:
    bool alreadyInit = false;
};