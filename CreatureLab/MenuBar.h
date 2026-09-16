#pragma once

class CLabController;

class CMenuBar
{
public:
    explicit CMenuBar(CLabController& labController);
public:
    void Render(CEditorContext& context);
private:
    CLabController& m_labController;
};
