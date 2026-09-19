#include "pch.h"
#include "CreatureTreeNodeEdit.h"

using namespace Creature;
using namespace Creature::Animation;


void CCreatureTreeNodeEdit::BeginRenameCreature(const std::string& strText)
{
    m_eEditType = NodeEditType::RenameCreature;
    m_animationId = INVALID_ANIMATION_ID;
    m_strText = strText;
}

void CCreatureTreeNodeEdit::BeginCreateNewAnimation(const std::string& strText)
{
    m_eEditType = NodeEditType::CreateAnimation;
    m_animationId = INVALID_ANIMATION_ID;
    m_strText = strText;
}

void CCreatureTreeNodeEdit::BeginRenameAnimation(AnimationId animationId, const std::string& strText)
{
    m_eEditType = NodeEditType::RenameCreature;
    m_animationId = animationId;
    m_strText = strText;
}

void CCreatureTreeNodeEdit::EndEdit()
{
    m_eEditType = NodeEditType::None;
    m_animationId = INVALID_ANIMATION_ID;
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
