#include "pch.h"
#include "GraphicsDevice.h"
#include "SpriteRenderDescription.h"
#include "SpriteRenderer.h"
#include "Texture.h"

#include <DirectXMath.h>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <vector>

using namespace Creature::Math;
using namespace Microsoft::WRL;

namespace
{
    struct SpriteConstants
    {
        DirectX::XMFLOAT4X4  transform{};
        float fOpacity = 1.0f;
        float fPadding[3]{};

        DirectX::XMFLOAT2 uvMin{ 0.0f, 0.0f };
        DirectX::XMFLOAT2 uvMax{ 1.0f, 1.0f };
    };

    DirectX::XMMATRIX ToXMMATRIX(
        const CMatrix3x2& matrix)
    {
        const auto m = matrix.ToArray();
        return DirectX::XMMATRIX(
            m[0], m[1], 0.0f, 0.0f,
            m[2], m[3], 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            m[4], m[5], 0.0f, 1.0f);
    }

    DirectX::XMMATRIX CreateSpriteMatrix(
        const CSpriteRenderDescription& desc,
        float viewportWidth,
        float viewportHeight)
    {
        const auto& texture = desc.GetTexture();
        if (!texture) return DirectX::XMMatrixIdentity();

        const auto& matrix = desc.GetMatrix();

        const auto& size = desc.GetSize();
        const auto spriteScale =
            DirectX::XMMatrixScaling(
                size.x,
                size.y,
                1.0f);

        const auto transform = ToXMMATRIX(matrix);

        const auto projection =
            DirectX::XMMatrixOrthographicOffCenterLH(
                0.0f,
                viewportWidth,
                viewportHeight,
                0.0f,
                0.0f,
                1.0f);

        return spriteScale
            * transform
            * projection;
    }

    struct SpriteVertex
    {
        float x = 0.0f;
        float y = 0.0f;
        float u = 0.0f;
        float v = 0.0f;
    };

    std::vector<std::byte> LoadBinaryFile(const std::filesystem::path& path)
    {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file) return {};

        const auto size = file.tellg();
        if (size <= 0) return {};

        std::vector<std::byte> data(static_cast<std::size_t>(size));

        file.seekg(0, std::ios::beg);

        if (!file.read(
            reinterpret_cast<char*>(data.data()),
            static_cast<std::streamsize>(data.size())))
        {
            return {};
        }

        return data;
    }
}

CSpriteRenderer::CSpriteRenderer(CGraphicsDevice& graphics)
    : m_graphicsDevice(graphics)
{
}

bool CSpriteRenderer::Initialize()
{
    if (!CreateVertexBuffer()) return false;
    if (!CreateVertexShader()) return false;
    if (!CreatePixelShader()) return false;
    if (!CreateConstantBuffer()) return false;
    if (!CreateSamplerState()) return false;
    if (!CreateBlendState()) return false;
    return true;
}

void CSpriteRenderer::Begin()
{
    auto context = m_graphicsDevice.GetContext();

    constexpr UINT stride = sizeof(SpriteVertex);
    constexpr UINT offset = 0;

    auto constantBuffer = m_constantBuffer.Get();

    context->VSSetConstantBuffers(0, 1, &constantBuffer);
    context->PSSetConstantBuffers(0, 1, &constantBuffer);

    auto vertexBuffer = m_vertexBuffer.Get();

    context->IASetVertexBuffers(
        0,
        1,
        &vertexBuffer,
        &stride,
        &offset);

    context->IASetInputLayout(m_inputLayout.Get());

    context->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    context->VSSetShader(
        m_vertexShader.Get(),
        nullptr,
        0);

    context->PSSetShader(
        m_pixelShader.Get(),
        nullptr,
        0);

    auto sampler = m_samplerState.Get();
    context->PSSetSamplers(0, 1, &sampler);

    context->OMSetBlendState(
        m_blendState.Get(),
        nullptr,
        0xffffffff);
}

void CSpriteRenderer::Draw(const CSpriteRenderDescription& desc)
{
    const auto& texture = desc.GetTexture();
    if (!texture) return;

    D3D11_MAPPED_SUBRESOURCE mapped{};

    auto context = m_graphicsDevice.GetContext();

    const auto hr = context->Map(
        m_constantBuffer.Get(),
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mapped);
    if (FAILED(hr)) return;

    SpriteConstants constant
    {
        {},
        desc.GetOpacity(),
        {},
        {},
        {}
    };

    const auto viewportWidth
        = static_cast<float>(m_graphicsDevice.GetViewportWidth());
    const auto viewportHeight
        = static_cast<float>(m_graphicsDevice.GetViewportHeight());

    const auto spriteMatrix = CreateSpriteMatrix(
        desc,
        viewportWidth,
        viewportHeight);

    DirectX::XMStoreFloat4x4(
        &constant.transform,
        spriteMatrix);

    const auto& minUv = desc.GetMinUV();
    constant.uvMin = { minUv.x, minUv.y };
    const auto& maxUv = desc.GetMaxUV();
    constant.uvMax = { maxUv.x, maxUv.y };

    memcpy_s(mapped.pData, sizeof(SpriteConstants), &constant, sizeof(constant));

    context->Unmap(
        m_constantBuffer.Get(),
        0);

    auto textureView = texture->GetShaderResourceView();
    context->PSSetShaderResources(0, 1, &textureView);

    context->Draw(6, 0);
}

bool CSpriteRenderer::CreateVertexBuffer()
{
    constexpr SpriteVertex vertices[] =
    {
        { -0.5f, -0.5f, 0.0f, 0.0f },
        {  0.5f, -0.5f, 1.0f, 0.0f },
        { -0.5f,  0.5f, 0.0f, 1.0f },

        { -0.5f,  0.5f, 0.0f, 1.0f },
        {  0.5f, -0.5f, 1.0f, 0.0f },
        {  0.5f,  0.5f, 1.0f, 1.0f },
    };

    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth = sizeof(vertices);
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initialData{};
    initialData.pSysMem = vertices;

    const auto hr = m_graphicsDevice.GetDevice()->CreateBuffer(
        &desc,
        &initialData,
        m_vertexBuffer.GetAddressOf());

    return SUCCEEDED(hr);
}

bool CSpriteRenderer::CreateVertexShader()
{
    std::vector<std::byte> byteCode = LoadBinaryFile("shaders\\SpriteVS.cso");
    if (byteCode.empty()) return false;

    auto hr = m_graphicsDevice.GetDevice()->CreateVertexShader(
        byteCode.data(),
        byteCode.size(),
        nullptr,
        m_vertexShader.GetAddressOf());

    if (FAILED(hr)) return false;

    constexpr D3D11_INPUT_ELEMENT_DESC inputDesc[] =
    {
        {
            "POSITION",
            0,
            DXGI_FORMAT_R32G32_FLOAT,
            0,
            0,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        },
        {
            "TEXCOORD",
            0,
            DXGI_FORMAT_R32G32_FLOAT,
            0,
            sizeof(float) * 2,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        },
    };

    hr = m_graphicsDevice.GetDevice()->CreateInputLayout(
        inputDesc,
        static_cast<UINT>(std::size(inputDesc)),
        byteCode.data(),
        byteCode.size(),
        m_inputLayout.GetAddressOf());

    return SUCCEEDED(hr);
}

bool CSpriteRenderer::CreatePixelShader()
{
    std::vector<std::byte> byteCode = LoadBinaryFile("shaders\\SpritePS.cso");
    if (byteCode.empty()) return false;

    auto hr = m_graphicsDevice.GetDevice()->CreatePixelShader(
        byteCode.data(),
        byteCode.size(),
        nullptr,
        m_pixelShader.GetAddressOf());
    return SUCCEEDED(hr);
}

bool CSpriteRenderer::CreateConstantBuffer()
{
    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth = sizeof(SpriteConstants);
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    const auto hr = m_graphicsDevice.GetDevice()->CreateBuffer(
        &desc,
        nullptr,
        m_constantBuffer.GetAddressOf());

    return SUCCEEDED(hr);
}

bool CSpriteRenderer::CreateSamplerState()
{
    D3D11_SAMPLER_DESC desc{};
    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.MaxLOD = D3D11_FLOAT32_MAX;

    const auto hr = m_graphicsDevice.GetDevice()->CreateSamplerState(
        &desc,
        m_samplerState.GetAddressOf());

    return SUCCEEDED(hr);
}

bool CSpriteRenderer::CreateBlendState()
{
    D3D11_BLEND_DESC desc{};

    auto& target = desc.RenderTarget[0];
    target.BlendEnable = TRUE;
    target.SrcBlend = D3D11_BLEND_SRC_ALPHA;
    target.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    target.BlendOp = D3D11_BLEND_OP_ADD;
    target.SrcBlendAlpha = D3D11_BLEND_ONE;
    target.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    target.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    target.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    const auto hr = m_graphicsDevice.GetDevice()->CreateBlendState(
        &desc,
        m_blendState.GetAddressOf());

    return SUCCEEDED(hr);
}
