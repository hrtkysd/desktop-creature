#pragma once

#include "CreatureTreeNodeEdit.h"
#include "PartId.h"

namespace Creature
{
    class CCreature;
    class CSkeleton;
}

class CCreatureEditor;
class CEditorContext;

class CCreatureTreePanel
{
public:
    explicit CCreatureTreePanel(
        CCreatureEditor& editor,
        CEditorContext& context);
public:
    void Draw();
    void DrawPart(
        const Creature::CSkeleton& skeleton,
        Creature::PartId partId);
private:
    CCreatureEditor& m_editor;
    CEditorContext& m_editorContext;
    CCreatureTreeNodeEdit m_nodeEdit;
};
