#include "pch.h"
#include "CreatureTreeNodeEdit.h"

using namespace Creature;

void CCreatureTreeNodeEdit::BeginRenameCreature(const std::string& strText)
{
    m_eEditType = NodeEditType::RenameCreature;
    m_motionId = INVALID_MOTION_ID;
    m_strText = strText;
}

void CCreatureTreeNodeEdit::BeginCreateNewMotion(const std::string& strText)
{
    m_eEditType = NodeEditType::CreateMotion;
    m_motionId = INVALID_MOTION_ID;
    m_strText = strText;
}

void CCreatureTreeNodeEdit::BeginRenameMotion(MotionId motionId, const std::string& strText)
{
    m_eEditType = NodeEditType::RenameMotion;
    m_motionId = motionId;
    m_strText = strText;
}

void CCreatureTreeNodeEdit::EndEdit()
{
    m_eEditType = NodeEditType::None;
    m_motionId = INVALID_MOTION_ID;
    m_strText = "";
}

bool CCreatureTreeNodeEdit::IsEditing() const
{
    return m_eEditType != NodeEditType::None;
}

bool CCreatureTreeNodeEdit::IsEditing(NodeEditType eNodeEditType) const
{
    if (eNodeEditType == NodeEditType::None) return false;
    return m_eEditType == eNodeEditType;
}

std::string& CCreatureTreeNodeEdit::GetText()
{
    return m_strText;
}

const std::string& CCreatureTreeNodeEdit::GetText() const
{
    return m_strText;
}

MotionId CCreatureTreeNodeEdit::GetMotionId() const
{
    return m_motionId;
}
