#include "pch.h"
#include "UndoBuffer.h"
#include "UndoScope.h"

using namespace Creature;

struct CUndoBuffer::SnapshotImpl
{
    SnapshotImpl(CCreature&& state, Revision revision)
        : state(std::move(state))
        , revision(revision)
    {
    }
    CCreature state;
    Revision revision = 0;
};

CUndoBuffer::CUndoBuffer(CCreature& creature, std::size_t maxHistory)
    : m_creature(creature)
    , m_maxHistoryCount(maxHistory)
{
}

CUndoBuffer::~CUndoBuffer() = default;

CUndoScope CUndoBuffer::CreateScope()
{
    return CUndoScope{ *this, m_creature.Clone() };
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

    auto& snapshot = m_vecUndo.back();
    m_vecRedo.emplace_back(m_creature.Clone(), m_currentRevision);

    const auto revision = snapshot.revision;
    m_creature = std::move(snapshot.state);
    m_vecUndo.pop_back();

    SetCurrentRevision(revision, true);
}

void CUndoBuffer::Redo()
{
    if (!CanRedo()) return;

    auto& snapshot = m_vecRedo.back();
    m_vecUndo.emplace_back(m_creature.Clone(), m_currentRevision);

    const auto revision = snapshot.revision;
    m_creature = std::move(snapshot.state);
    m_vecRedo.pop_back();

    SetCurrentRevision(revision, true);
}

void CUndoBuffer::ResetHistory()
{
    m_vecUndo.clear();
    m_vecRedo.clear();
    SetCurrentRevision(m_nextRevision++, false);
}

Revision CUndoBuffer::GetCurrentRevision() const noexcept
{
    return m_currentRevision;
}

void CUndoBuffer::SetListener(IUndoBufferListener* listener) noexcept
{
    m_listener = listener;
}

void CUndoBuffer::Push(CCreature&& state)
{
    m_vecRedo.clear();

    if (m_maxHistoryCount > 0)
    {
        if (m_vecUndo.size() >= m_maxHistoryCount)
        {
            m_vecUndo.erase(m_vecUndo.begin());
        }
        m_vecUndo.emplace_back(std::move(state), m_currentRevision);
    }

    SetCurrentRevision(m_nextRevision++, true);
}

void CUndoBuffer::SetCurrentRevision(
    Revision revision,
    bool isNotify)
{
    if (m_currentRevision == revision) return;

    m_currentRevision = revision;

    if (!isNotify || !m_listener) return;
    m_listener->OnRevisionChanged(revision);
}
