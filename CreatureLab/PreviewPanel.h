#pragma once

namespace Creature
{
    struct Genome;
}

class CTextureCache;

class CPreviewPanel
{
public:
    static void Draw(const Creature::Genome& genome, const std::shared_ptr<CTextureCache>& textureCache);
};
