#include "pch.h"

#include "GraphicsDevice.h"
#include "RenderTarget.h"

#include <iterator>

CGraphicsDevice::~CGraphicsDevice()
{
    Shutdown();
}

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

    if (FAILED(hr) || !m_device || !m_deviceContext) return false;

    if (!CreateRenderTargetView()) return false;

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
    if (!CreateRenderTargetView()) return false;

    SetViewport(width, height);

    return true;
}

ID3D11Device* CGraphicsDevice::GetDevice()
{
    if (!m_device) return nullptr;
    return m_device.Get();
}

ID3D11DeviceContext* CGraphicsDevice::GetContext()
{
    if (!m_deviceContext) return nullptr;
    return m_deviceContext.Get();
}

void CGraphicsDevice::SetViewport(std::uint32_t width, std::uint32_t height)
{
    if (width == 0 || height == 0) return;
    if (!m_deviceContext) return;

    D3D11_VIEWPORT viewport{};
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    m_deviceContext->RSSetViewports(1, &viewport);

    m_viewportWidth = width;
    m_viewportHeight = height;
}

std::uint32_t CGraphicsDevice::GetViewportWidth() const
{
    return m_viewportWidth;
}

std::uint32_t CGraphicsDevice::GetViewportHeight() const
{
    return m_viewportHeight;
}

void CGraphicsDevice::SetRenderTarget(const CRenderTarget& target)
{
    auto rtv = target.RTV();
    if (rtv == nullptr) return;
    m_deviceContext->OMSetRenderTargets(1, &rtv, nullptr);
}

std::optional<CRenderTarget> CGraphicsDevice::CreateRenderTarget(
    std::uint32_t width,
    std::uint32_t height)
{
    D3D11_TEXTURE2D_DESC desc{};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags =
        D3D11_BIND_RENDER_TARGET |
        D3D11_BIND_SHADER_RESOURCE;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;

    auto hr = m_device->CreateTexture2D(
        &desc,
        nullptr,
        texture.GetAddressOf());

    if (FAILED(hr)) return std::nullopt;

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
    hr = m_device->CreateRenderTargetView(
        texture.Get(),
        nullptr,
        rtv.GetAddressOf());

    if (FAILED(hr)) return std::nullopt;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
    hr = m_device->CreateShaderResourceView(
        texture.Get(),
        nullptr,
        srv.GetAddressOf());

    if (FAILED(hr)) return std::nullopt;

    return CRenderTarget(
        std::move(rtv),
        std::move(srv),
        width,
        height);
}

bool CGraphicsDevice::BeginFrame()
{
    if (!m_deviceContext || !m_renderTargetView)
    {
        return false;
    }

    auto rtv = m_renderTargetView.Get();

    constexpr float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

    m_deviceContext->ClearRenderTargetView(
        rtv,
        clearColor);

    m_deviceContext->OMSetRenderTargets(
        1,
        &rtv,
        nullptr);

    return true;
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

bool CGraphicsDevice::CreateRenderTargetView()
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
