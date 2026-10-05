#pragma once

#include "PartAppearance.h"
#include "PartId.h"
#include "ZOrder.h"

#include <filesystem>
#include <vector>

namespace Creature
{
    namespace Editor
    {
        class CAppearanceEditor;
    }

    class CAppearance
    {
        friend class Editor::CAppearanceEditor;
    public:
        CAppearance();
    private:
        explicit CAppearance(std::vector<PartAppearance>&& vecPart);
    public:
        const std::vector<PartAppearance>& Parts() const;
        const PartAppearance* FindByPartId(PartId partId) const;
        bool CanMoveForward(PartId partId) const;
        bool CanMoveBackward(PartId partId) const;

    private:
        ZOrder NextZOrder();
        bool MoveForward(PartId partId);
        bool MoveBackward(PartId partId);
        void AddPart(PartId partId, const std::filesystem::path& path);
        bool RemovePart(PartId partId);

    private:
        std::vector<PartAppearance> m_vecPart;
    };
}
