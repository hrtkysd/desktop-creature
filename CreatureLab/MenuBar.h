#pragma once

class CLabController;

class CMenuBar
{
public:
    explicit CMenuBar(CLabController& labController);
public:
    void Draw(CEditorContext& context);
private:
    void HandleShortcutKey(CEditorContext& context);
private:
    CLabController& m_labController;
};
