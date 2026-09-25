#include "pch.h"

#include "GraphicsDevice.h"
#include <iterator>

bool CGraphicsDevice::Initialize(
    HWND hWnd,
    std::uint32_t width,
    std::uint32_t height)
{
    if (hWnd == nullptr) return false;
    if (width == 0 || height == 0) return false;

    DXGI_SWAP_CHAIN_DESC desc{};

    desc.BufferCount = 2;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = hWnd;
    desc.SampleDesc.Count = 1;
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    constexpr D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };

    D3D_FEATURE_LEVEL featureLevel{};

    auto hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        featureLevels,
        static_cast<UINT>(std::size(featureLevels)),
        D3D11_SDK_VERSION,
        &desc,
        m_swapChain.GetAddressOf(),
        m_device.GetAddressOf(),
        &featureLevel,
        m_deviceContext.GetAddressOf());

    if (FAILED(hr) || !m_device) return false;

    if (!CreateRenderTarget()) return false;

    SetViewport(width, height);

    return true;
}

bool CGraphicsDevice::Resize(std::uint32_t width, std::uint32_t height)
{
    if (width == 0 || height == 0) return false;
    if (!m_deviceContext || !m_swapChain) return false;

    m_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
    m_renderTargetView.Reset();

    const auto hr = m_swapChain->ResizeBuffers(
        0,
        width,
        height,
        DXGI_FORMAT_UNKNOWN,
        0);
    if (FAILED(hr)) return false;
    if (!CreateRenderTarget()) return false;

    SetViewport(width, height);

    return true;
}

ID3D11Device& CGraphicsDevice::GetDevice()
{
    return *(m_device.Get());
}

ID3D11DeviceContext& CGraphicsDevice::GetContext()
{
    return *(m_deviceContext.Get());
}

void CGraphicsDevice::SetViewport(std::uint32_t width, std::uint32_t height)
{
    if (width == 0 || height == 0) return;

    D3D11_VIEWPORT viewport{};
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    m_deviceContext->RSSetViewports(1, &viewport);
}

void CGraphicsDevice::BeginFrame()
{
    if (!m_deviceContext || !m_renderTargetView)
    {
        return;
    }

    constexpr float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

    m_deviceContext->ClearRenderTargetView(
        m_renderTargetView.Get(),
        clearColor);

    auto renderTarget = m_renderTargetView.Get();

    m_deviceContext->OMSetRenderTargets(
        1,
        &renderTarget,
        nullptr);
}

void CGraphicsDevice::Present()
{
    if (!m_swapChain) return;
    m_swapChain->Present(1, 0);
}

void CGraphicsDevice::Shutdown()
{
    Clear();
    m_swapChain.Reset();
    m_deviceContext.Reset();
    m_device.Reset();
}

void CGraphicsDevice::Clear()
{
    if (m_deviceContext)
    {
        m_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
    }
    m_renderTargetView.Reset();
}

bool CGraphicsDevice::CreateRenderTarget()
{
    if (!m_swapChain || !m_device) return false;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

    auto hr = m_swapChain->GetBuffer(
        0,
        IID_PPV_ARGS(backBuffer.GetAddressOf()));

    if (FAILED(hr)) return false;

    hr = m_device->CreateRenderTargetView(
        backBuffer.Get(),
        nullptr,
        m_renderTargetView.GetAddressOf());

    return SUCCEEDED(hr);
}
