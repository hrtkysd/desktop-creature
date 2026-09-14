#pragma once

#include <memory>
#include <filesystem>

struct ID3D11Device;
struct ImageData;
class CTexture;

class CTextureLoader
{
public:
    static bool LoadImage(
        const std::filesystem::path& path,
        ImageData& image);
    static std::shared_ptr<CTexture> Load(
        ID3D11Device* device,
        const std::filesystem::path& path);
};
