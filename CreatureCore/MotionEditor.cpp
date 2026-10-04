#include "pch.h"
#include "AnimationEditor.h"
#include "AppearanceEditor.h"
#include "Motion.h"
#include "MotionEditor.h"
#include "SkeletonEditor.h"

using namespace Creature;
using namespace Creature::Editor;

CMotionEditor::CMotionEditor(CMotion& motion)
    : m_motion(motion)
{
}

CAppearanceEditor CMotionEditor::AppearanceEditor()
{
    return CAppearanceEditor{ m_motion.MutableAppearance() };
}

CAnimationEditor CMotionEditor::AnimationEditor()
{
    return CAnimationEditor{ m_motion.MutableAnimation() };
}

CSkeletonEditor CMotionEditor::SkeletonEditor()
{
    return CSkeletonEditor{ m_motion.MutableSkeleton(), *this };
}

const CMotion& CMotionEditor::GetMotion() const
{
    return m_motion;
}

void CMotionEditor::SetName(const std::string& strName)
{
    m_motion.SetName(strName);
}

const std::string& CMotionEditor::GetName() const
{
    return m_motion.GetName();
}

bool CMotionEditor::RemovePart(PartId id)
{
    const auto parts = SkeletonEditor().RemovePartCore(id);
    if (parts.empty()) return false;
    for (const auto part : parts)
    {
        AnimationEditor().RemovePart(part);
        AppearanceEditor().RemovePart(part);
    }
    return true;
}
