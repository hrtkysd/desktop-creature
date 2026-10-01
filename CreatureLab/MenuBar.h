#pragma once

class CDocumentContext;
class CEditorContext;
class CLabController;

class CMenuBar
{
public:
    explicit CMenuBar(CLabController& labController);
public:
    void Draw(
        const CDocumentContext& documentContext,
        CEditorContext& editorContext);
private:
    void HandleShortcutKey(CEditorContext& context);
private:
    CLabController& m_labController;
};
