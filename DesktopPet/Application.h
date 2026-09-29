#pragma once

#include "AnimationPlayer.h"
#include "Creature.h"
#include "GraphicsRenderer.h"
#include "TextureCache.h"

#include <cstdint>
#include <optional>
#include <Windows.h>

class CAppRuntime
{
public:
    CAppRuntime();
    ~CAppRuntime();
    CAppRuntime(const CAppRuntime&) = delete;
    CAppRuntime& operator=(const CAppRuntime&) = delete;
public:
    bool Initialize(HINSTANCE hInstance, int nCmdShow);
    int Run();
private:
    HWND CreateWorkspace(
        HINSTANCE hInstance,
        std::uint32_t width,
        std::uint32_t height,
        int nCmdShow);
    bool InitializeGraphics(
        std::uint32_t width,
        std::uint32_t height);
    bool LoadCreature();
    bool Update(float fDeltaTime);
    bool Render();
private:
    static LRESULT CALLBACK WndProc(
        HWND hWnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam);
private:
    Creature::Animation::CAnimationPlayer m_animationPlayer;
    Creature::CCreature m_creature;
    CGraphicsRenderer m_renderer;
    std::optional<CTextureCache> m_textureCache;

    HWND m_hWorkspace = nullptr;
};
