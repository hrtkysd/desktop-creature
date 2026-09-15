#pragma once

#include "Part.h"

namespace Creature
{
    class CCreature;
}

class CRenderPartItem;
class CTextureCache;

struct ImVec2;

class CPreviewPanel
{
public:
    void Draw(Creature::CCreature& creature, CTextureCache& textureCache);
private:
    std::vector<CRenderPartItem> BuildPartViews(Creature::CCreature& creature, CTextureCache& textureCache, const ImVec2& center, float scale);
    void HandleInput(Creature::CCreature& creature, const std::vector<CRenderPartItem>& vecRenderPart);
private:
    Creature::PartId m_selectedPartId = Creature::INVALID_PART_ID;
};
