#pragma once

class CImGuiMainMenuBarScope final
{
public:
    CImGuiMainMenuBarScope();
    ~CImGuiMainMenuBarScope();

    explicit operator bool() const;

private:
    bool m_bOpened = false;
};
