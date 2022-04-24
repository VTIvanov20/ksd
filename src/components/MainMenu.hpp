#pragma once

#include "UIComponent.hpp"
#include "../GameManager.hpp"

enum class MainMenuState {
    MAIN_MENU,
    SINGLEPLAYER,
    MULTIPLAYER,
    MULTIPLAYER_HOST,
    MULTIPLAYER_JOIN,
};

class MainMenu : public UIComponent
{
public:
    DEFINE_DERIVED_COMPONENT(MainMenu, UIComponent)
    void OnUI() override;
    void OnCreate() override;
    void OnDestroy() override;

protected:
    MainMenuState state = MainMenuState::MAIN_MENU;
    char *textBuf;
};