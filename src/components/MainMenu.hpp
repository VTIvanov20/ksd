#pragma once

#include "./Component.hpp"
#include "../util/imgui/imgui.h"
#include "../GameManager.hpp"

enum class MainMenuState {
    MAIN_MENU,
    SINGLEPLAYER,
    MULTIPLAYER,
    MULTIPLAYER_HOST,
    MULTIPLAYER_JOIN,
};

class MainMenu : public Component
{
public:
    DEFINE_COMPONENT(MainMenu)
    void OnUI() override;
    void OnCreate() override;
    void OnDestroy() override;

protected:
    MainMenuState state = MainMenuState::MAIN_MENU;
    char *textBuf;
};