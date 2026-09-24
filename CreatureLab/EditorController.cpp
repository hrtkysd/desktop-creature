#include "pch.h"
#include "AnimationPlayer.h"
#include "CreatureEditor.h"
#include "EditorCommand.h"
#include "EditorContext.h"
#include "EditorController.h"

using namespace Creature;
using namespace Creature::Animation;

CEditorController::CEditorController(
    Creature::Editor::CCreatureEditor& editor,
    CAnimationPlayer& animationPlayer,
    CEditorContext& context)
    : m_editor(editor)
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
        m_editor.GetSkeletonEditor().RemovePart(m_context.GetPartId());
        break;
    default:
        break;
    }
}
