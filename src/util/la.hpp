#pragma once
#include <raylib.h>

typedef struct Vec2f {
    float x;
    float y;

    Vec2f (float _x, float _y) : x(_x), y(_y) {};
    Vec2f () : x(0.f), y(0.f) {};

    Vec2f (const Vector2& v) { x = v.x; y = v.y; }
    operator Vector2() { return Vector2 { x, y }; }
} Vec2f;

typedef struct Vec2i {
    int x;
    int y;

    Vec2i (int _x, int _y) : x(_x), y(_y) {};
    Vec2i () : x(0), y(0) {};

    Vec2i (const Vector2& v) { x = static_cast<int>(v.x); y = static_cast<int>(v.y); }
    operator Vector2() { return Vector2 { static_cast<float>(x), static_cast<float>(y) }; }
} Vec2i;

typedef struct Vec3f {
    float x;
    float y;
    float z;

    Vec3f (float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
    Vec3f () : x(0.f), y(0.f), z(0.f) {};

    Vec3f (const Vector3& v) { x = v.x; y = v.y; z = v.z; }
    operator Vector3() { return Vector3 { x, y, z }; }
} Vec3f;

typedef struct Vec3i {
    int x;
    int y;
    int z;

    Vec3i (int _x, int _y, int _z) : x(_x), y(_y), z(_z) {};
    Vec3i () : x(0), y(0), z(0) {};

    Vec3i (const Vector3& v) { x = static_cast<int>(v.x); y = static_cast<int>(v.y); z = static_cast<int>(v.z); }
    operator Vector3() { return Vector3 { static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) }; }
} Vec3i;


typedef struct Vec4f {
    float x;
    float y;
    float z;
    float w;

    Vec4f (float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {};
    Vec4f () : x(0.f), y(0.f), z(0.f), w(0.f) {};

    Vec4f (const Vector4& v) { x = v.x; y = v.y; z = v.z; w = v.w; }
    operator Vector4() { return Vector4 { x, y, z, w }; }
} Vec4f;

typedef struct Vec4i {
    int x;
    int y;
    int z;
    int w;

    Vec4i (int _x, int _y, int _z, int _w) : x(_x), y(_y), z(_z), w(_w) {};
    Vec4i () : x(0), y(0), z(0), w(0) {};

    Vec4i (const Vector4& v) { x = static_cast<int>(v.x); y = static_cast<int>(v.y); z = static_cast<int>(v.z); w = static_cast<int>(v.w); }
    operator Vector4() { return Vector4 { static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(w) }; }
} Vec4i;

typedef struct Rectf {
    float x;                // Rectangle top-left corner position x
    float y;                // Rectangle top-left corner position y
    float width;            // Rectangle width
    float height;           // Rectangle height

    Rectf (float _x, float _y, float _w, float _h) : x(_x), y(_y), width(_w), height(_h) {};
    Rectf () : x(0.f), y(0.f), width(0.f), height(0.f) {};

    Rectf (const Rectangle& r) { x = r.x; y = r.y; width = r.width; height = r.height; }
    operator Rectangle() { return Rectangle { x, y, width, height }; }
} Rectf;

typedef struct Recti {
    int x;
    int y;
    int width;
    int height;

    Recti (int _x, int _y, int _w, int _h) : x(_x), y(_y), width(_w), height(_h) {};
    Recti () : x(0), y(0), width(0), height(0) {};

    Recti (const Rectangle& r) { x = static_cast<int>(r.x); y = static_cast<int>(r.y); width = static_cast<int>(r.width); height = static_cast<int>(r.height); }
    operator Rectangle() { return Rectangle { static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height) }; }
} Recti;

// float lerpf(float a, float b, float t)
// {
//     return a + (b - a) * t;
// }