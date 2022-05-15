#pragma once

#include "./Component.hpp"
#include "../util/graphics.hpp"
#include "../util/dear_imgui/imgui.h"
#include "../GameManager.hpp"

/**
 * @brief An enumerator class for all main menu states
 */
enum class MainMenuState {
    MAIN_MENU,
    SINGLEPLAYER,
    MULTIPLAYER,
    MULTIPLAYER_HOST,
    MULTIPLAYER_JOIN,
    WAITING_FOR_PLAYERS_MULTIPLAYER,
    SETTINGS,
    HELP
};

class MainMenu : public Component
{
public:
    /**
     * @brief Defines a component of type MainMenu
     */
    DEFINE_COMPONENT(MainMenu)
    ~MainMenu();

    void OnUI() override;
    void OnUpdate() override;
    void OnCreate() override;
    void OnDestroy() override;

protected:
    /**
     * @brief The state of the main menu is MAIN_MENU
     */
    MainMenuState state = MainMenuState::MAIN_MENU;
    
    /**
     * @brief The texture of the logo image
     */
    std::weak_ptr<TextureObject> logoImg;

    /**
     * @brief The windows size
     */
    const ImVec2 windowSize {400, 200};

    /**
     * @brief The buffer where the multiplayer code is entered
     */
    char *multiplayerCodeBuf;
};