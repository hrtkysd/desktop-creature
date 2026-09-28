#include "pch.h"
#include "RenderTarget.h"
#include "RenderTargetScope.h"

CRenderTargetScope::CRenderTargetScope(
    ID3D11DeviceContext* context,
    const CRenderTarget& target)
    : m_context(context)
{
    ID3D11RenderTargetView* prev = nullptr;

    m_context->OMGetRenderTargets(
        1,
        &prev,
        nullptr);

    m_prevRenderTarget.Attach(prev);

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

    m_context->OMSetRenderTargets(
        1,
        &rtv,
        nullptr);

    m_context->RSSetViewports(
        1,
        &m_prevViewport);
}
