#pragma once
#include <cstdio>
#include <cassert>
#include <memory>
#include <unordered_map>
#include "./dear_imgui/imgui.h"
#include "../ObjectManager.hpp"
#include "./la.hpp"
#include <raylib.h>

class TextureObject : public Object
{
public:
    DEFINE_OBJECT(TextureObject)

    virtual ~TextureObject();

    void LoadEmpty(Vec2i res);
    void LoadFromFile(const char *path);
    void Unload();

    void Draw(Recti Source, Rectf Destination, double Rotation, Vec2f Origin);
    void Draw(Vec2f Position, Vec2f Scale, double Rotation, Vec2f Origin);

    void BeginDrawingTo();
    void EndDrawingTo();

    Vec2i GetSize();
    Texture& GetTexture();
    RenderTexture& GetRenderTexture();

private:
    Texture texture {};
    RenderTexture renderTexture {};
    RenderTexture verticallyMirroredTexture {};
};

namespace Graphics
{
    // Used to initialize an SDL window
    bool InitWindow(Vec2f winDimensions, const char* title);

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
    bool IsKeyDown(int);

    // Checks if the passed key isn't pressed down
    bool IsKeyUp(int);
    
    // Returns mouse position (relative to the focused window) in the form of a vector
    Vec2i GetMousePos();

    // Returns the difference between the mouse position one frame ago and the current frame
    Vec2i GetMouseDelta();

    // Sets the mouse position relative to the window
    void SetMousePos(Vec2i);

    bool IsMouseButtonDown(int button);

    bool IsMouseButtonUp(int button);
    // float GetMouseWheelMove();
};

namespace RLImGui
{
    bool InitImGui();

    void CloseImGui();

    void BeginImGuiDrawing();

    void EndImGuiDrawing();
}