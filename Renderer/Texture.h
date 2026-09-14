#pragma once

#include <cstdint>

struct ID3D11ShaderResourceView;

class CTexture
{
public:
    ID3D11ShaderResourceView* GetShaderResourceView() const noexcept;
    std::uint32_t GetWidth() const noexcept;
    std::uint32_t GetHeight() const noexcept;

private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
    std::uint32_t m_uWidth = 0;
    std::uint32_t m_uHeight = 0;

    friend class CTextureLoader;
};
