#include "pch.h"
#include "ContinuousEditUndoScope.h"
#include "EditorContext.h"
#include "UndoScope.h"

#include "imgui.h"

CContinuousEditUndoScope::CContinuousEditUndoScope(
    CEditorContext& context,
    std::optional<CUndoScope>& undoScope,
    bool bEdited)
    : m_undoScope(undoScope)
    , m_bEndEdit(ImGui::IsItemDeactivatedAfterEdit())
{
    if (!bEdited || m_undoScope) return;
    m_undoScope.emplace(context.CreateUndoScope());
}

CContinuousEditUndoScope::~CContinuousEditUndoScope()
{
    if (!m_bEndEdit) return;
    m_undoScope.reset();
}
