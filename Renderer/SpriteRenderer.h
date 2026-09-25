#pragma once

#include <d3d11.h>
#include <wrl/client.h>

class CGraphicsDevice;
class CSpriteRenderDescription;

class CSpriteRenderer
{
public:
    explicit CSpriteRenderer(CGraphicsDevice& graphics);
public:
    bool Initialize();
    void Begin();
    void Draw(const CSpriteRenderDescription& desc);

private:
    bool CreateVertexBuffer();
    bool CreateVertexShader();
    bool CreatePixelShader();
    bool CreateConstantBuffer();
    bool CreateSamplerState();
    bool CreateBlendState();

private:
    CGraphicsDevice& m_graphicsDevice;

    Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
    Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState;
};
