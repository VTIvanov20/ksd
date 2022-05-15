#pragma once

#include "./Component.hpp"
#include "../util/graphics.hpp"
#include "../util/dear_imgui/imgui.h"
#include "../GameManager.hpp"

enum class MainMenuState {
    MAIN_MENU,
    SINGLEPLAYER,
    MULTIPLAYER,
    MULTIPLAYER_HOST,
    MULTIPLAYER_JOIN,
    WAITING_FOR_PLAYERS_MULTIPLAYER
};

class MainMenu : public Component
{
public:
    DEFINE_COMPONENT(MainMenu)
    ~MainMenu();

    void OnUI() override;
    void OnCreate() override;

protected:
    MainMenuState state = MainMenuState::MAIN_MENU;
    const ImVec2 windowSize {400, 200};
    char *multiplayerCodeBuf;
};