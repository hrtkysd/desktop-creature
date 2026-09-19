#pragma once

#include "CreatureTreeNodeEdit.h"

namespace Creature
{
    class CCreature;
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
private:
    CCreatureEditor& m_editor;
    CEditorContext& m_editorContext;
    CCreatureTreeNodeEdit m_nodeEdit;
};
