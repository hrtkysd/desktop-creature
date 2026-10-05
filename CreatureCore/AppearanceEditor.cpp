#include "pch.h"
#include "Appearance.h"
#include "AppearanceEditor.h"

using namespace Creature;
using namespace Creature::Editor;

CAppearanceEditor::CAppearanceEditor(CAppearance& appearance)
    : m_appearance(appearance)
{
}

const CAppearance& CAppearanceEditor::GetAppearance() const
{
    return m_appearance;
}

void CAppearanceEditor::AddPart(PartId partId, const std::filesystem::path& path)
{
    m_appearance.AddPart(partId, path);
}

bool CAppearanceEditor::RemovePart(PartId partId)
{
    return m_appearance.RemovePart(partId);
}

bool CAppearanceEditor::MoveForward(PartId partId)
{
    return m_appearance.MoveForward(partId);
}

bool CAppearanceEditor::MoveBackward(PartId partId)
{
    return m_appearance.MoveBackward(partId);
}

void CAppearanceEditor::SwapAppearance(std::vector<PartAppearance>&& vecPartAppearance)
{
    m_appearance = CAppearance{ std::move(vecPartAppearance) };
}
