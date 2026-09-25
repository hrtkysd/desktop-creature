#include "pch.h"
#include "AnimationPlayer.h"
#include "CreatureEditor.h"
#include "EditorCommand.h"
#include "EditorContext.h"
#include "EditorController.h"
#include "UndoScope.h"

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

bool CEditorController::Execute(EditorCommand command)
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
    {
        auto scope = m_context.CreateUndoScope();

        auto& skeletonEditor = m_editor.GetSkeletonEditor();
        if (!skeletonEditor.RemovePart(m_context.GetPartId()))
        {
            scope.Cancel();
            return false;
        }
    }
    break;
    case EditorCommand::Undo:
        m_context.Undo();
        break;
    case EditorCommand::Redo:
        m_context.Redo();
        break;
    default:
        break;
    }
    return true;
}
