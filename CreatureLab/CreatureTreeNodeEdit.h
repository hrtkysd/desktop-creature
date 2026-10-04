#pragma once

#include "MotionId.h"

#include <cstdint>
#include <string>

enum class NodeEditType : std::uint8_t
{
    None,
    RenameCreature,
    RenameMotion,
    CreateMotion
};

class CCreatureTreeNodeEdit
{
public:

    void BeginRenameCreature(const std::string& strText);
    void BeginCreateNewMotion(const std::string& strText);
    void BeginRenameMotion(Creature::MotionId motionId, const std::string& strText);

    void EndEdit();

    bool IsEditing() const;
    bool IsEditing(NodeEditType eNodeEditType) const;

    std::string& GetText();
    const std::string& GetText() const;

private:
    NodeEditType m_eEditType = NodeEditType::None;
    Creature::MotionId m_motionId
        = Creature::INVALID_MOTION_ID;
    std::string m_strText;
};
