#pragma once

#include "RenderTarget.h"

#include <cstdint>
#include <optional>
#include <wrl/client.h>

interface ID3D11Device;
interface ID3D11DeviceContext;
interface ID3D11RenderTargetView;
interface IDXGISwapChain;

class CGraphicsDevice
{
public:
    ~CGraphicsDevice();
public:
    bool Initialize(
        HWND hWnd,
        std::uint32_t width,
        std::uint32_t height);
    bool Resize(
        std::uint32_t width,
        std::uint32_t height);

    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetContext();

    void SetViewport(
        std::uint32_t width,
        std::uint32_t height);

    std::uint32_t GetViewportWidth() const;
    std::uint32_t GetViewportHeight() const;

    void SetRenderTarget(const CRenderTarget& target);
    std::optional<CRenderTarget> CreateRenderTarget(
        std::uint32_t width,
        std::uint32_t height);

    bool BeginFrame();
    void Present();

private:
    void Shutdown();

    void Clear();
    bool CreateRenderTargetView();
private:
    Microsoft::WRL::ComPtr<ID3D11Device> m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;

    std::uint32_t m_viewportWidth = 0;
    std::uint32_t m_viewportHeight = 0;
};
