#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <cassert>
#include <memory>
#include <map>
#include "./imgui/imgui.h"
#include "./imgui/backends/imgui_impl_sdl.h"
#include "./imgui/backends/imgui_impl_sdlrenderer.h"
#include "../ObjectManager.hpp"
#include "./la.hpp"

class Texture : public Object
{
public:
    DEFINE_OBJECT(Texture)

    virtual ~Texture();

    void LoadTexture(const char *path);
    void UnloadTexture();

    void DrawTexture();
    Vec2i GetSize();
    Recti Source;
    Recti Destination;

private:
    SDL_Texture* texture = nullptr;
};

namespace Graphics
{
    // Used to initialize an SDL window
    bool InitWindow(Vec2f winDimensions, const char* title, SDL_WindowFlags winFlags, SDL_RendererFlags rendFlags);

    // Used to close the SDL window initialized by InitWindow
    bool CloseWindow();

    // Returns the width and height of the window in the form of a vector
    Vec2i GetWindowDimensions();

    // Before any internal GPU draw calls are done this function needs to be called
    void BeginDrawing();

    // Used to draw the contents from the buffer and flush it for new content
    void EndDrawing();

    // Used to set the window position
    void SetWindowPosition(Vec2f);

};

namespace Input
{
    // Checks if the passed key is pressed down
    bool IsKeyDown(SDL_KeyCode);

    // Checks if the passed key isn't pressed down
    bool IsKeyUp(SDL_KeyCode);
    
    // Returns mouse position (relative to the focused window) in the form of a vector
    Vec2i GetMousePos();

    // Returns the difference between the mouse position one frame ago and the current frame
    Vec2i GetMouseDelta();

    // Sets the mouse position relative to the window
    void SetMousePos(Vec2i);

    bool IsMouseButtonDown(uint8_t button);

    bool IsMouseButtonUp(uint8_t button);
    // float GetMouseWheelMove();
};

namespace SDLImGui
{
    bool InitImGui();

    void CloseImGui();

    void BeginImGuiDrawing();

    void EndImGuiDrawing();
}