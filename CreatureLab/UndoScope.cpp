#include "pch.h"
#include "UndoBuffer.h"
#include "UndoScope.h"

using namespace Creature;

CUndoScope::CUndoScope(CUndoBuffer& buffer, CCreature&& before)
    : m_buffer(buffer)
    , m_before(std::move(before))
{
}

CUndoScope::~CUndoScope()
{
    if (!m_isActive) return;
    m_buffer.Push(std::move(m_before));
}

CUndoScope::CUndoScope(CUndoScope&& rhs)
    : m_buffer(rhs.m_buffer)
    , m_before(std::move(rhs.m_before))
    , m_isActive(rhs.m_isActive)
{
    rhs.m_isActive = false;
}

void CUndoScope::Cancel()
{
    m_isActive = false;
}
