#include "ComInitializer.h"

CComInitializer::CComInitializer()
    : m_hResult(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED))
{
}

CComInitializer::~CComInitializer()
{
    if (FAILED(m_hResult)) return;
    ::CoUninitialize();
}

bool CComInitializer::Succeeded() const
{
    return SUCCEEDED(m_hResult);
}
