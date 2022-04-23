#pragma once
#include <SDL2/SDL.h>

typedef SDL_FPoint Vec2f;
typedef SDL_Point Vec2i;

typedef struct Vec3f {
    float x;
    float y;
    float z;
} Vec3f;

typedef struct Vec3i {
    int x;
    int y;
    int z;
} Vec3i;


typedef struct Vec4f {
    float x;
    float y;
    float z;
    float w;
} Vec4f;

typedef struct Vec4i {
    int x;
    int y;
    int z;
    int w;
} Vec4i;

typedef SDL_FRect Rectf;
typedef SDL_Rect Recti;

// float lerpf(float a, float b, float t)
// {
//     return a + (b - a) * t;
// }