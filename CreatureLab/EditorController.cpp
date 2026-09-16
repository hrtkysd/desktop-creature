#include "pch.h"
#include "AnimationPlayer.h"
#include "Creature.h"
#include "EditorCommand.h"
#include "EditorContext.h"
#include "EditorController.h"
#include "Skeleton.h"

using namespace Creature;
using namespace Creature::Animation;

CEditorController::CEditorController(CCreature& creature, CAnimationPlayer& animationPlayer, CEditorContext& context)
    : m_creature(creature)
    , m_animationPlayer(animationPlayer)
    , m_context(context)
{
}

void CEditorController::Execute(EditorCommand command)
{
    switch (command)
    {
    case EditorCommand::PlayAnimation:
        m_animationPlayer.Play();
        break;
    case EditorCommand::StopAnimation:
        m_animationPlayer.Stop();
        break;
    case EditorCommand::PauseAnimation:
        m_animationPlayer.Pause();
        break;
    case EditorCommand::DeletePart:
        m_creature.GetSkeleton().RemovePart(m_context.GetPartId());
        break;
    default:
        break;
    }
}
