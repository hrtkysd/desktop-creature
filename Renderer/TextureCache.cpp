#include "pch.h"
#include "TextureCache.h"
#include "TextureLoader.h"

namespace
{
    std::filesystem::path NormalizePath(const std::filesystem::path& path)
    {
        std::error_code err;
        const auto normalized = std::filesystem::absolute(path, err);
        return err
            ? path.lexically_normal()
            : normalized.lexically_normal();
    }
}

CTextureCache::CTextureCache(ID3D11Device* device)
    : m_device(device) {}

std::shared_ptr<CTexture> CTextureCache::Load(const std::filesystem::path& path)
{
    const auto cache = m_textures.find(path);
    if (cache != m_textures.cend()) return cache->second;

    if (!m_device) return nullptr;
    auto texture = CTextureLoader::Load(m_device, path);
    if (!texture) return nullptr;
    m_textures.emplace(path, texture);
    return texture;
}
