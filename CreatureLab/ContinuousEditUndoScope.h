#pragma once

#include <optional>

class CEditorContext;
class CUndoScope;

class CContinuousEditUndoScope
{
public:
    CContinuousEditUndoScope(
        CEditorContext& context,
        std::optional<CUndoScope>& undoScope,
        bool bEdited);

    ~CContinuousEditUndoScope();

private:
    std::optional<CUndoScope>& m_undoScope;
    bool m_bEndEdit;
};
