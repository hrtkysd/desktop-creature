#include "pch.h"
#include "EditorContext.h"

using namespace Creature;

void CEditorContext::SelectPart(PartId partId)
{
    m_partId = partId;
}

PartId CEditorContext::GetPartId() const noexcept
{
    return m_partId;
}

void CEditorContext::SetEditMode(EditMode mode)
{
    m_eEditMode = mode;
}

EditMode CEditorContext::GetEditMode() const noexcept
{
    return m_eEditMode;
}

