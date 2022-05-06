#pragma once
#include <raylib.h>

typedef Vector2 Vec2f;

typedef struct Vec2i {
    int x;
    int y;
} Vec2i;

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

typedef Rectangle Rectf;

typedef struct Recti {
    int x;
    int y;
    int width;
    int height;
} Recti;

typedef Color Color;

// float lerpf(float a, float b, float t)
// {
//     return a + (b - a) * t;
// }