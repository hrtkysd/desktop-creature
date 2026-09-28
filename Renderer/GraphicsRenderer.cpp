#include "pch.h"
#include "GraphicsRenderer.h"
#include "RenderTarget.h"

CGraphicsRenderer::CGraphicsRenderer()
    : m_spriteRenderer(m_graphicsDevice)
{
}

bool CGraphicsRenderer::Initialize(
    HWND hWnd,
    std::uint32_t width,
    std::uint32_t height)
{
    if (!m_graphicsDevice.Initialize(hWnd, width, height)) return false;
    if (!m_spriteRenderer.Initialize()) return false;
    return true;
}

bool CGraphicsRenderer::BeginFrame()
{
    return m_graphicsDevice.BeginFrame();
}

void CGraphicsRenderer::Present()
{
    m_graphicsDevice.Present();
}

void CGraphicsRenderer::BeginSprite()
{
    m_spriteRenderer.Begin();
}

void CGraphicsRenderer::DrawSprite(const CSpriteRenderDescription& desc)
{
    m_spriteRenderer.Draw(desc);
}

void CGraphicsRenderer::SetViewport(
    std::uint32_t width,
    std::uint32_t height)
{
    m_graphicsDevice.SetViewport(width, height);
}

ID3D11Device* CGraphicsRenderer::GetDevice()
{
    return m_graphicsDevice.GetDevice();
}

ID3D11DeviceContext* CGraphicsRenderer::GetContext()
{
    return m_graphicsDevice.GetContext();
}

CSpriteRenderer& CGraphicsRenderer::SpriteRenderer()
{
    return m_spriteRenderer;
}

std::optional<CRenderTarget> CGraphicsRenderer::CreateRenderTarget(
    std::uint32_t width,
    std::uint32_t height)
{
    return m_graphicsDevice.CreateRenderTarget(width, height);
}

bool CGraphicsRenderer::Resize(
    std::uint32_t width,
    std::uint32_t height)
{
    return m_graphicsDevice.Resize(width, height);
}
