#pragma once

#include <cstdint>
#include <wrl/client.h>

class CGraphicsDevice;

interface ID3D11RenderTargetView;
interface ID3D11ShaderResourceView;

class CRenderTarget
{
public:
    explicit CRenderTarget(
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv,
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv,
        std::uint32_t width,
        std::uint32_t height
    );
public:

    void Reset();

    ID3D11RenderTargetView* RTV() const;
    ID3D11ShaderResourceView* SRV() const;

    std::uint32_t Width() const;
    std::uint32_t Height() const;

private:
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
    std::uint32_t m_width = 0;
    std::uint32_t m_height = 0;
};
