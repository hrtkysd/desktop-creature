#pragma once

#include "CreatureTreeNodeEdit.h"
#include "MotionId.h"
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
class CLabController;

class CCreatureTreePanel
{
public:
    explicit CCreatureTreePanel(
        Creature::Editor::CCreatureEditor& editor,
        CLabController& labController,
        CEditorContext& context);
public:
    void Draw();
    void DrawPart(
        Creature::MotionId motionId,
        const Creature::CSkeleton& skeleton,
        Creature::PartId partId);
private:
    Creature::Editor::CCreatureEditor& m_editor;
    CLabController& m_labController;
    CEditorContext& m_editorContext;
    CCreatureTreeNodeEdit m_nodeEdit;
};
