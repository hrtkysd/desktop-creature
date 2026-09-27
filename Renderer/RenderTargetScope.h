#pragma once

#include <array>
#include <d3d11.h>
#include <wrl/client.h>

class CRenderTarget;

class CRenderTargetScope
{
public:
    CRenderTargetScope(
        ID3D11DeviceContext* context,
        const CRenderTarget& target);
    CRenderTargetScope(
        ID3D11DeviceContext* context,
        const CRenderTarget& target,
        const std::array<float, 4>& clearColor);
    ~CRenderTargetScope();

private:
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_prevRenderTarget;

    D3D11_VIEWPORT m_prevViewport{};
    ID3D11DeviceContext* m_context = nullptr;
};
