#include "MainMenu.hpp"
#include <iostream>
#include "./GlobalState.hpp"
#include "./NetworkController.hpp"
#include "./BackgroundComponent.hpp"

/* - Main Menu Layout -

    MainMenuState::MAIN_MENU
        - Singleplayer -> MainMenuState::SINGLEPLAYER
        - Multiplayer -> MainMenuState::MULTIPLAYER
        - Quit -> GameManager::GetInstance()->Quit()

    MainMenuState::SINGLEPLAYER
        - Without NOT card -> Init game
        - With NOT card -> Init game
        - Back -> MainMenuState::MAIN_MENU

    MainMenuState::MULTIPLAYER_HOST
        - Without NOT card -> Init game
        - With NOT card -> Init game
        - Back -> MainMenuState::MAIN_MENU

    MainMenuState::MULTIPLAYER_JOIN
        - Textbox
        - Join -> Init multiplayer game
        - Back -> MainMenuState::MULTIPLAYER

*/

void MainMenu::OnCreate()
{
    multiplayerCodeBuf = new char[16];
    multiplayerCodeBuf[0] = '\0';
    logoImg = ObjectManager::GetInstance()->CreateObject<TextureObject>();
    logoImg.lock()->LoadFromFile("res/img/logo.png");
}

void MainMenu::OnDestroy()
{
    logoImg.lock()->Unload();
    ObjectManager::GetInstance()->DestroyObjectFromID(logoImg.lock()->GetID());
}

MainMenu::~MainMenu()
{
    delete[] multiplayerCodeBuf;
}

void MainMenu::OnUpdate()
{
    const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    auto logoImgRes = logoImg.lock()->GetSize();
    logoImg.lock()->Draw({ displaySize.x / 2, displaySize.y / 4 }, { 1.f, 1.f }, 0.f, { logoImgRes.x / 2.f, logoImgRes.y / 2.f });
}

void MainMenu::OnUI()
{
    auto networkControllerEntity =
        std::static_pointer_cast<Entity>(
            ObjectManager::GetInstance()->GetEntityFromTagName("network_controller").lock());
    auto networkController = MGetComponentFrom(networkControllerEntity, NetworkController);
    
    const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    const ImVec2 windowPadding = ImGui::GetStyle().WindowPadding;
    const ImVec2 windowPos {
        displaySize.x / 2 - windowSize.x / 2,
        displaySize.y / 1.5f - windowSize.y / 2
    };

    const ImVec2 buttonSize {
        -1,
        windowSize.y / 4 - windowPadding.y
    };

    if (!networkController->IsActive() && (state == MainMenuState::MULTIPLAYER || state == MainMenuState::MULTIPLAYER_HOST || state == MainMenuState::MULTIPLAYER_JOIN || state == MainMenuState::WAITING_FOR_PLAYERS_MULTIPLAYER))
        state = MainMenuState::SINGLEPLAYER;
    
    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(windowPos);
    ImGui::Begin("Main Menu", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking);
    switch(state)
    {
        case MainMenuState::MAIN_MENU:
        {
            if(networkController->GetState().inGame)
                state = MainMenuState::WAITING_FOR_PLAYERS_MULTIPLAYER;
            
            if(ImGui::Button("Singleplayer", buttonSize))
            {
                state = MainMenuState::SINGLEPLAYER;
            }
            if (networkController->IsActive())
            {
                if(ImGui::Button("Multiplayer", buttonSize))
                {
                    state = MainMenuState::MULTIPLAYER;
                }
            }
            if(ImGui::Button("Quit", buttonSize))
            {
                GameManager::GetInstance()->Quit();
            }
            if (ImGui::Button("Settings", buttonSize))
            {
                state = MainMenuState::SETTINGS;
            }
            break;
        }
        case MainMenuState::SINGLEPLAYER:
        {
            if(ImGui::Button("Without NOT card", buttonSize))
            {
                auto globalState = std::static_pointer_cast<Entity>(
                    ObjectManager::GetInstance()->GetEntityFromTagName("global_state").lock());
                MGetComponentFrom(globalState, GlobalState)->SetValue("mode", "singleplayer");
                GameManager::GetInstance()->ChangeScene("res/scenes/game_scene.json");
            }
            if(ImGui::Button("Back", buttonSize))
            {
                state = MainMenuState::MAIN_MENU;
            }
            break;
        }
        case MainMenuState::MULTIPLAYER:
        {
            if(ImGui::Button("Host", buttonSize))
            {
                state = MainMenuState::MULTIPLAYER_HOST;
            }
            if(ImGui::Button("Join", buttonSize))
            {
                state = MainMenuState::MULTIPLAYER_JOIN;
            }
            if(ImGui::Button("Back", buttonSize))
            {
                state = MainMenuState::MAIN_MENU;
            }
            break;
        }
        case MainMenuState::MULTIPLAYER_HOST:
        {
            if(ImGui::Button("Without NOT card", buttonSize))
            {
                auto globalState = std::static_pointer_cast<Entity>(
                    ObjectManager::GetInstance()->GetEntityFromTagName("global_state").lock());
                networkController->CreateRoom();
                MGetComponentFrom(globalState, GlobalState)->SetValue("mode", "multiplayer");

                state = MainMenuState::WAITING_FOR_PLAYERS_MULTIPLAYER;
            }
            if(ImGui::Button("Back", buttonSize))
            {
                state = MainMenuState::MULTIPLAYER;
            }
            break;
        }
        case MainMenuState::MULTIPLAYER_JOIN:
        {
            ImGui::Text("Enter game code:");
            if(ImGui::InputText(" ", multiplayerCodeBuf, 9))
            {
                // GameManager::GetInstance()->JoinMultiplayerGame();
            }
            ImGui::SameLine();
            if(ImGui::Button("Join", ImVec2(100, 0)))
            {
                auto globalState = std::static_pointer_cast<Entity>(
                    ObjectManager::GetInstance()->GetEntityFromTagName("global_state").lock());
                networkController->JoinRoom(std::string { multiplayerCodeBuf });
                MGetComponentFrom(globalState, GlobalState)->SetValue("mode", "multiplayer");

                state = MainMenuState::WAITING_FOR_PLAYERS_MULTIPLAYER;
                // GameManager::GetInstance()->JoinMultiplayerGame();
            }
            if(ImGui::Button("Back"))
            {
                state = MainMenuState::MULTIPLAYER;
            }
            break;
        }
        case MainMenuState::WAITING_FOR_PLAYERS_MULTIPLAYER:
        {
            auto nState = networkController->GetState();

            if (!nState.inGame)
                state = MainMenuState::MAIN_MENU;
            
            ImGui::Text("Waiting for a player to join...");
            ImGui::Text("inGame = %s", nState.inGame ? "true" : "false");
            ImGui::Text("gameStarted = %s", nState.gameStarted ? "true" : "false");
            ImGui::Text("code = %s", nState.code.c_str());

            if (ImGui::Button("Leave"))
                networkController->LeaveRoom();

            if (nState.gameStarted)
                GameManager::GetInstance()->ChangeScene("res/scenes/game_scene.json");
            break;
        }
        case MainMenuState::SETTINGS:
        {
            static const char* themes[] = {
                "res/img/themes/space/bg.png",
                "res/img/themes/top-secret/bg.png",
                "res/img/themes/wildwest/bg.png",
            };
            
            static int themeIndex = 0;
            auto bgEntity =
                std::static_pointer_cast<Entity>(
                    ObjectManager::GetInstance()->GetEntityFromTagName("background_component").lock());
            auto bgCmp = MGetComponentFrom(bgEntity, BackgroundComponent);

            if (ImGui::Button("Change Background", buttonSize))
            {
                bgCmp->ChangeBGFile(themes[themeIndex]);
                themeIndex++;
                if (themeIndex == 3)
                    themeIndex = 0;
            }

            if (ImGui::Button("Help", buttonSize))
                state = MainMenuState::HELP;
            
            if (ImGui::Button("Back", buttonSize))
                state = MainMenuState::MAIN_MENU;
            break;
        }
        case MainMenuState::HELP:
        {
            static const char* help = R"T(A standard b00le0 deck contains a total of 64 cards:
2 - Truth Table Cards
6 - Initial Binaries
8 - Not Cards
48 - Logical Gate Cards

In the beginning, the initial binaries will
be randomly generated, where some of the sides
facing you may be signed with a 1 or a 0.
After said binaries are generated, your hand
will have 5 cards, consisting of 3 types:
AND, OR & XOR.

After playing the first card, your opponent
(either a real person or the computer) will
start to fill their nand gate pyramid. The
first one to fully complete said pyramid is
the winner!

During the game, you will have to take turns
playing your cards. At the start of your turn,
you must draw the top card from the deck.
After drawing your card, you have to either
play one from your hand or discard one, in
order to have 4 cards at the end of your turn.
If the deck runs out, the discarded cards get
shuffled back into the newly-formed deck.
)T";
            ImGui::Text(help);
            if (ImGui::Button("Back"))
                state = MainMenuState::SETTINGS;
        }
    }
    ImGui::End();
}