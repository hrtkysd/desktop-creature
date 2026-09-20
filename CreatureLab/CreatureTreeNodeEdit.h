#pragma once

#include "AnimationId.h"

#include <cstdint>
#include <string>

enum class NodeEditType : std::uint8_t
{
    None,
    RenameCreature,
    RenameAnimation,
    CreateAnimation
};

class CCreatureTreeNodeEdit
{
public:

    void BeginRenameCreature(const std::string& strText);
    void BeginCreateNewAnimation(const std::string& strText);
    void BeginRenameAnimation(Creature::Animation::AnimationId animationId, const std::string& strText);

    void EndEdit();

    bool IsEditing() const;
    bool IsEditing(NodeEditType eNodeEditType) const;

    std::string& GetText();
    const std::string& GetText() const;

private:
    NodeEditType m_eEditType = NodeEditType::None;
    Creature::Animation::AnimationId m_animationId
        = Creature::Animation::INVALID_ANIMATION_ID;
    std::string m_strText;
};
