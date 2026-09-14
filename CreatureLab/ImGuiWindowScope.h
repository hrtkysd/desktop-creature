#pragma once

class CImGuiWindowScope
{
public:
    explicit CImGuiWindowScope(const char* name);
    virtual ~CImGuiWindowScope();

    explicit operator bool() const;

private:
    bool m_bVisible;
};
