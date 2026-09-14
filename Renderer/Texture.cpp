#include "pch.h"

#include "Texture.h"

ID3D11ShaderResourceView* CTexture::GetShaderResourceView() const noexcept
{
    return m_shaderResourceView.Get();
}

std::uint32_t CTexture::GetWidth() const noexcept
{
    return m_uWidth;
}

std::uint32_t CTexture::GetHeight() const noexcept
{
    return m_uHeight;
}
