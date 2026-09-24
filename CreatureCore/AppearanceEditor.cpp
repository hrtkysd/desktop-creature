#include "pch.h"
#include "Appearance.h"
#include "AppearanceEditor.h"

using namespace Creature;
using namespace Creature::Editor;

CAppearanceEditor::CAppearanceEditor(CAppearance& apperance)
    : m_appearance(apperance)
{
}

const CAppearance& CAppearanceEditor::GetAppearance() const
{
    return m_appearance;
}

void CAppearanceEditor::SetTexture(PartId partId, const std::filesystem::path& path)
{
    m_appearance.SetTexture(partId, path);
}

bool CAppearanceEditor::RemovePart(PartId partId)
{
    return m_appearance.RemovePart(partId);
}
