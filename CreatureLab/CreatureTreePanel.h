#pragma once

#include "CreatureTreeNodeEdit.h"
#include "PartId.h"

namespace Creature
{
    class CCreature;
    class CSkeleton;
    namespace Editor
    {
        class CCreatureEditor;
    }
}

class CEditorContext;

class CCreatureTreePanel
{
public:
    explicit CCreatureTreePanel(
        Creature::Editor::CCreatureEditor& editor,
        CEditorContext& context);
public:
    void Draw();
    void DrawPart(
        const Creature::CSkeleton& skeleton,
        Creature::PartId partId);
private:
    Creature::Editor::CCreatureEditor& m_editor;
    CEditorContext& m_editorContext;
    CCreatureTreeNodeEdit m_nodeEdit;
};
