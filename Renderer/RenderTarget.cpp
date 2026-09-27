#include "pch.h"
#include "RenderTarget.h"

#include <d3d11.h>
#include <utility>

CRenderTarget::CRenderTarget(
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv,
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv,
    std::uint32_t width,
    std::uint32_t height)
    : m_rtv(std::move(rtv))
    , m_srv(std::move(srv))
    , m_width(width)
    , m_height(height)
{
}

void CRenderTarget::Reset()
{
    m_rtv.Reset();
    m_srv.Reset();
    m_width = 0;
    m_height = 0;
}

ID3D11RenderTargetView* CRenderTarget::RTV() const
{
    return m_rtv.Get();
}

ID3D11ShaderResourceView* CRenderTarget::SRV() const
{
    return m_srv.Get();
}

std::uint32_t CRenderTarget::Width() const
{
    return m_width;
}

std::uint32_t CRenderTarget::Height() const
{
    return m_height;
}
