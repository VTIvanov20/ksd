#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "config.h"

#include "ObjectManager.hpp"
#include "Entity.hpp"

#include "components/MainMenu.hpp"
#include "util/graphics.hpp"

class Scene
{
public:
    Scene() {};
    virtual ~Scene() = default;

    virtual void Initialize() {};
};

class GameManager
{
private:
    struct GameState
    {
        bool scene_update;
        bool should_quit;
        Scene scene;
    } state;

    void ReinitializeScene();

    static GameManager* instance;

private:
    GameManager() : state { false, false, {} } {};
    ~GameManager() = default;

public:
    // disallow cloning singleton
    GameManager(GameManager const&) = delete;

    // disallow assignments to singleton
    void operator=(GameManager const&) = delete;

    static GameManager* GetInstance();

    /**
     * @brief Initializes the program
     * 
     * @note Meant to be used in main.cpp
     */
    void Initialize();

    /**
     * @brief Deinitializes the program
     * 
     * @note Meant to be used in main.cpp
     */
    void Deinitialize();

    /**
     * @brief Steps or updates the program
     * 
     * @note Meant to be used in main.cpp
     */
    void Update();

    /**
     * @brief Similar to the WindowShouldClose() function in raylib.h
     * 
     * @note Meant to be used in main.cpp
     * 
     * @return bool
     */
    bool ShouldQuit();

    /**
     * @brief An API function, which triggers a boolean telling the program it should quit;
     * 
     * @attention Meant to be used at any point in the code (unlike functions like initialize and deinitialize)
     */
    void Quit();

    /**
     * @brief An API function, which changes the current scene
     * 
     * @param scene This describes the current scene
     * 
     * @note Meant to be used at any point in the code
     */
    void ChangeScene(Scene scene);
};