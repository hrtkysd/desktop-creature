#pragma once

#include <windef.h>

namespace Creature
{
    namespace Math
    {
        class CTransform2D;
        struct Vec2;
    }
}

class CTexture;

class CRenderer
{
public:
    bool Initialize(HWND hwnd);

    void BeginFrame();
    void DrawTexture(
        const CTexture& texture,
        const Creature::Math::CTransform2D& transform,
        const Creature::Math::Vec2& pivot);

    void EndFrame();
};
