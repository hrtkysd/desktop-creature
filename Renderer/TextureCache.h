#pragma once

#include <filesystem>
#include <memory>
#include <unordered_map>

struct ID3D11Device;

class CTexture;

class CTextureCache
{
public:
    explicit CTextureCache(ID3D11Device* device);
public:
    std::shared_ptr<CTexture> Load(
        const std::filesystem::path& path);
private:

    ID3D11Device* m_device;
    std::unordered_map<
        std::filesystem::path,
        std::shared_ptr<CTexture>> m_textures;
};
