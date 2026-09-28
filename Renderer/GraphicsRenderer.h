#pragma once

#include "GraphicsDevice.h"
#include "SpriteRenderer.h"

#include <cstdint>
#include <optional>
#include <windef.h>

interface ID3D11Device;
interface ID3D11DeviceContext;

class CSpriteRenderDescription;
class CRenderTarget;

class CGraphicsRenderer
{
public:
    CGraphicsRenderer();
public:
    bool Initialize(
        HWND hWnd,
        std::uint32_t width,
        std::uint32_t height);
    bool BeginFrame();
    void Present();

    void BeginSprite();
    void DrawSprite(const CSpriteRenderDescription& desc);

    void SetViewport(
        std::uint32_t width,
        std::uint32_t height);

    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetContext();

    CSpriteRenderer& SpriteRenderer();
    std::optional<CRenderTarget> CreateRenderTarget(
        std::uint32_t width,
        std::uint32_t height);

    bool Resize(
        std::uint32_t width,
        std::uint32_t height);
private:
    CGraphicsDevice m_graphicsDevice;
    CSpriteRenderer m_spriteRenderer;
};
