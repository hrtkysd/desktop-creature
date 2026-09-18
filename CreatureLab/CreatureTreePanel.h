#pragma once

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
        const Creature::CCreature& creature,
        CCreatureEditor& editor,
        CEditorContext& context);
public:
    void Draw();
private:
    const Creature::CCreature& m_creature;
    CCreatureEditor& m_editor;
    CEditorContext& m_editorContext;

    char m_szRenameBuffer[256]{};
    bool m_isRenaming = false;
};
