#pragma once

#include <Windows.h>

class CComInitializer
{
public:
    CComInitializer();
    ~CComInitializer();

    bool Succeeded() const;

private:
    HRESULT m_hResult = E_FAIL;
};
