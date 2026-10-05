#pragma once

#include "MotionId.h"
#include "PartId.h"

#include <cstdint>
#include <string>

enum class NodeEditType : std::uint8_t
{
    None,
    RenameCreature,
    RenameMotion,
    CreateMotion,
    RenamePart,
    CreatePart
};

class CCreatureTreeNodeEdit
{
public:

    void BeginRenameCreature(const std::string& strText);
    void BeginCreateNewMotion(const std::string& strText);
    void BeginRenameMotion(Creature::MotionId motionId, const std::string& strText);
    void BeginCreatePart(
        Creature::MotionId motionId,
        Creature::PartId partId,
        const std::string& strText);
    void BeginRenamePart(
        Creature::MotionId motionId,
        Creature::PartId partId,
        const std::string& strText);

    void EndEdit();

    bool IsEditing() const;
    bool IsEditing(NodeEditType eNodeEditType) const;

    std::string& GetText();
    const std::string& GetText() const;

    Creature::MotionId GetMotionId() const;
    Creature::PartId GetParentPartId() const;
private:
    NodeEditType m_eEditType = NodeEditType::None;
    Creature::MotionId m_motionId
        = Creature::INVALID_MOTION_ID;
    Creature::PartId m_parentId
        = Creature::INVALID_PART_ID;
    std::string m_strText;
};
