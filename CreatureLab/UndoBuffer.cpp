#include "pch.h"
#include "UndoBuffer.h"
#include "UndoScope.h"

using namespace Creature;

CUndoBuffer::CUndoBuffer(CCreature& creature, std::size_t maxHistory)
    : m_creature(creature)
    , m_maxHistoryCount(maxHistory)
{
}

CUndoScope CUndoBuffer::CreateScope()
{
    return { *this, m_creature.Clone() };
}

bool CUndoBuffer::CanUndo() const noexcept
{
    return !m_vecUndo.empty();
}

bool CUndoBuffer::CanRedo() const noexcept
{
    return !m_vecRedo.empty();
}

void CUndoBuffer::Undo()
{
    if (!CanUndo()) return;

    m_vecRedo.push_back(m_creature.Clone());

    m_creature = std::move(m_vecUndo.back());
    m_vecUndo.pop_back();
}

void CUndoBuffer::Redo()
{
    if (!CanRedo()) return;

    m_vecUndo.push_back(m_creature.Clone());

    m_creature = std::move(m_vecRedo.back());
    m_vecRedo.pop_back();
}

void CUndoBuffer::Clear()
{
    m_vecUndo.clear();
    m_vecRedo.clear();
}

void CUndoBuffer::Push(CCreature&& state)
{
    m_vecRedo.clear();

    if (m_maxHistoryCount == 0) return;

    if (m_vecUndo.size() >= m_maxHistoryCount)
    {
        m_vecUndo.erase(m_vecUndo.begin());
    }
    m_vecUndo.push_back(std::move(state));
}
