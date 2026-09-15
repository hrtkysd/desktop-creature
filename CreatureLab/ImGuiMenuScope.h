#pragma once

class CImGuiMenuScope
{
public:
    explicit CImGuiMenuScope(const char* name);
    virtual ~CImGuiMenuScope();

    explicit operator bool() const;

private:
    bool m_bOpen;
};
