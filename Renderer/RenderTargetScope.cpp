#include "pch.h"
#include "RenderTarget.h"
#include "RenderTargetScope.h"

CRenderTargetScope::CRenderTargetScope(
    ID3D11DeviceContext* context,
    const CRenderTarget& target)
    : m_context(context)
{
    ID3D11RenderTargetView* prevRTV = nullptr;
    ID3D11DepthStencilView* prevDSV = nullptr;
    m_context->OMGetRenderTargets(
        1,
        &prevRTV,
        &prevDSV);

    m_prevRenderTarget.Attach(prevRTV);
    m_prevDepthStencil.Attach(prevDSV);

    UINT count = 1;
    m_context->RSGetViewports(
        &count,
        &m_prevViewport);

    auto rtv = target.RTV();
    m_context->OMSetRenderTargets(
        1,
        &rtv,
        nullptr);
}

CRenderTargetScope::CRenderTargetScope(
    ID3D11DeviceContext* context,
    const CRenderTarget& target,
    const std::array<float, 4>& clearColor)
    : CRenderTargetScope(context, target)
{
    m_context->ClearRenderTargetView(
        target.RTV(),
        clearColor.data());
}

CRenderTargetScope::~CRenderTargetScope()
{
    auto rtv = m_prevRenderTarget.Get();
    auto dsv = m_prevDepthStencil.Get();

    m_context->OMSetRenderTargets(
        1,
        &rtv,
        dsv);

    m_context->RSSetViewports(
        1,
        &m_prevViewport);
}
