#include "pch.h"

#include "Animation.h"
#include "AnimationEditor.h"
#include "AnimationProperty.h"
#include "Appearance.h"
#include "AppearanceEditor.h"
#include "Creature.h"
#include "CreatureEditor.h"
#include "Motion.h"
#include "MotionEditor.h"
#include "MotionId.h"
#include "Skeleton.h"
#include "SkeletonEditor.h"
#include "Transform2D.h"
#include "Vec2.h"

using namespace Creature;
using namespace Creature::Animation;
using namespace Creature::Editor;
using namespace Creature::Math;

namespace Creature
{
    TEST(CreatureEditorTest, SetNameUpdatesCreatureName)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        editor.SetName("TestCreature");

        EXPECT_EQ(creature.GetName(), "TestCreature");
    }

    TEST(CreatureEditorTest, AddNewMotionAddsMotion)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto id = editor.AddNewMotion("Idle");

        ASSERT_NE(id, INVALID_MOTION_ID);

        const auto motion = creature.FindMotionById(id);
        ASSERT_NE(motion, nullptr);

        EXPECT_EQ(motion->GetName(), "Idle");
    }

    TEST(CreatureEditorTest, AddNewMotionGeneratesUniqueIds)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto idleId = editor.AddNewMotion("Idle");
        const auto walkId = editor.AddNewMotion("Walk");
        const auto sleepId = editor.AddNewMotion("Sleep");

        EXPECT_NE(idleId, INVALID_MOTION_ID);
        EXPECT_NE(walkId, INVALID_MOTION_ID);
        EXPECT_NE(sleepId, INVALID_MOTION_ID);

        EXPECT_NE(idleId, walkId);
        EXPECT_NE(idleId, sleepId);
        EXPECT_NE(walkId, sleepId);
    }

    TEST(CreatureEditorTest, AddMotionWithIdUpdatesNextGeneratedId)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        constexpr MotionId explicitId = 100;

        const auto addedId = editor.AddMotionWithId(explicitId, "Imported");
        ASSERT_EQ(addedId, explicitId);

        const auto generatedId = editor.AddNewMotion("Idle");
        EXPECT_EQ(generatedId, explicitId + 1);
    }

    TEST(CreatureEditorTest, AddMotionWithIdRejectsDuplicateId)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        constexpr MotionId id = 10;

        ASSERT_EQ(editor.AddMotionWithId(id, "Idle"), id);
        EXPECT_EQ(editor.AddMotionWithId(id, "Walk"), INVALID_MOTION_ID);
        EXPECT_EQ(creature.GetMotions().size(), 1u);
    }

    TEST(CreatureEditorTest, AddMotionWithIdRejectsInvalidId)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto id = editor.AddMotionWithId(INVALID_MOTION_ID, "Invalid");

        EXPECT_EQ(id, INVALID_MOTION_ID);
        EXPECT_TRUE(creature.GetMotions().empty());
    }

    TEST(CreatureEditorTest, RemoveMotionRemovesSpecifiedMotion)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto idleId = editor.AddNewMotion("Idle");
        const auto walkId = editor.AddNewMotion("Walk");

        ASSERT_NE(idleId, INVALID_MOTION_ID);
        ASSERT_NE(walkId, INVALID_MOTION_ID);

        EXPECT_TRUE(editor.RemoveMotion(idleId));

        EXPECT_EQ(creature.FindMotionById(idleId), nullptr);
        EXPECT_NE(creature.FindMotionById(walkId), nullptr);

        EXPECT_EQ(creature.GetMotions().size(), 1u);
    }

    TEST(CreatureEditorTest, RemoveMotionReturnsFalseForUnknownId)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        EXPECT_FALSE(editor.RemoveMotion(12345));
    }

    TEST(CreatureTest, CloneCreatesIndependentCopy)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        editor.SetName("Original");

        const auto motionId = editor.AddNewMotion("Idle");

        ASSERT_NE(motionId, INVALID_MOTION_ID);

        auto clone = creature.Clone();

        EXPECT_EQ(clone.GetName(), "Original");
        EXPECT_NE(clone.FindMotionById(motionId), nullptr);

        CCreatureEditor cloneEditor(clone);
        cloneEditor.SetName("Clone");

        EXPECT_EQ(creature.GetName(), "Original");
        EXPECT_EQ(clone.GetName(), "Clone");
    }

    TEST(CreatureTest, CloneOwnsIndependentAnimations)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto id = editor.AddNewMotion("Idle");
        ASSERT_NE(id, INVALID_MOTION_ID);

        auto clone = creature.Clone();
        CCreatureEditor cloneEditor(clone);

        auto motionEditor = cloneEditor.MotionEditor(id);
        ASSERT_TRUE(motionEditor.has_value());
        auto animationEditor = motionEditor->AnimationEditor();
        ASSERT_TRUE(animationEditor.SetDuration(2.0f));

        ASSERT_NE(creature.FindMotionById(id), nullptr);
        ASSERT_NE(clone.FindMotionById(id), nullptr);

        EXPECT_FLOAT_EQ(creature.FindMotionById(id)->GetAnimation().GetDuration(), 0.0f);
        EXPECT_FLOAT_EQ(clone.FindMotionById(id)->GetAnimation().GetDuration(), 2.0f);
    }

    TEST(CreatureEditorTest, ChildEditorUsesCurrentStateAfterMoveAssignment)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        CCreature replacement;
        CCreatureEditor replacementEditor(replacement);

        const auto id = replacementEditor.AddNewMotion("Idle");
        ASSERT_NE(id, INVALID_MOTION_ID);

        creature = std::move(replacement);

        auto motionEditor = editor.MotionEditor(id);
        ASSERT_TRUE(motionEditor.has_value());
        auto animationEditor = motionEditor->AnimationEditor();
        EXPECT_TRUE(animationEditor.SetDuration(2.0f));

        const auto motion = creature.FindMotionById(id);
        ASSERT_NE(motion, nullptr);
        EXPECT_FLOAT_EQ(motion->GetAnimation().GetDuration(), 2.0f);
    }

    TEST(CreatureEditorTest, RemovePartRemovesAllReferencesToPart)
    {
        CCreature creature;
        CCreatureEditor editor(creature);
        const auto motionId = editor.AddNewMotion("Idle");
        ASSERT_NE(motionId, INVALID_MOTION_ID);

        auto motionEditor = editor.MotionEditor(motionId);
        ASSERT_TRUE(motionEditor.has_value());
        auto skeletonEditor = motionEditor->SkeletonEditor();
        const auto bodyId = skeletonEditor.AddPart("Body");
        const auto headId = skeletonEditor.AddPart("Head", bodyId);

        ASSERT_NE(bodyId, INVALID_PART_ID);
        ASSERT_NE(headId, INVALID_PART_ID);

        auto appearanceEditor = motionEditor->AppearanceEditor();
        appearanceEditor.SetTexture(bodyId, L"assets/body.png");
        appearanceEditor.SetTexture(headId, L"assets/head.png");

        const CAnimationTrackKey bodyTrackKey
        {
            bodyId,
            AnimationProperty::PositionX
        };

        const CAnimationTrackKey headTrackKey
        {
            headId,
            AnimationProperty::Rotation
        };

        auto animationEditor = motionEditor->AnimationEditor();
        ASSERT_TRUE(animationEditor.AddTrack(CAnimationTrack{ bodyTrackKey }));
        ASSERT_TRUE(animationEditor.AddTrack(CAnimationTrack{ headTrackKey }));

        const auto motion = creature.FindMotionById(motionId);
        ASSERT_NE(motion, nullptr);
        const auto& skeleton = motion->GetSkeleton();
        ASSERT_NE(skeleton.FindPartById(bodyId), nullptr);
        ASSERT_NE(skeleton.FindPartById(headId), nullptr);

        const auto& appearance = motion->GetAppearance();
        ASSERT_NE(appearance.FindByPartId(bodyId), nullptr);
        ASSERT_NE(appearance.FindByPartId(headId), nullptr);

        const auto& animationBefore = motion->GetAnimation();
        ASSERT_NE(animationBefore.FindAnimationTrack(bodyTrackKey), nullptr);
        ASSERT_NE(animationBefore.FindAnimationTrack(headTrackKey), nullptr);

        ASSERT_TRUE(skeletonEditor.RemovePart(headId));

        EXPECT_NE(skeleton.FindPartById(bodyId), nullptr);
        EXPECT_EQ(skeleton.FindPartById(headId), nullptr);

        EXPECT_NE(appearance.FindByPartId(bodyId), nullptr);
        EXPECT_EQ(appearance.FindByPartId(headId), nullptr);

        const auto& animationAfter = creature.FindMotionById(motionId)->GetAnimation();
        EXPECT_NE(animationAfter.FindAnimationTrack(bodyTrackKey), nullptr);
        EXPECT_EQ(animationAfter.FindAnimationTrack(headTrackKey), nullptr);
    }

    TEST(SkeletonEditorTest, SetPartTransformUpdatesTransform)
    {
        CCreature creature;
        CCreatureEditor creatureEditor(creature);
        const auto motionId = creatureEditor.AddNewMotion("Idle");
        ASSERT_NE(motionId, INVALID_MOTION_ID);

        auto motionEditor = creatureEditor.MotionEditor(motionId);
        ASSERT_TRUE(motionEditor.has_value());
        auto editor = motionEditor->SkeletonEditor();
        const auto id = editor.AddPart("Body");

        CTransform2D transform;
        transform.SetPosition({ 10.0f, 20.0f });
        transform.SetScale({ 2.0f, 3.0f });
        transform.SetRotation(0.5f);

        ASSERT_TRUE(editor.SetPartTransform(id, transform));

        const auto part = creature.FindMotionById(motionId)->GetSkeleton().FindPartById(id);
        ASSERT_NE(part, nullptr);

        EXPECT_EQ(part->bindTransform.GetPosition(), Vec2({ 10.0f, 20.0f }));
        EXPECT_EQ(part->bindTransform.GetScale(), Vec2({ 2.0f, 3.0f }));
        EXPECT_FLOAT_EQ(part->bindTransform.GetRotation(), 0.5f);
    }

    TEST(SkeletonEditorTest, SetPartPositionUpdatesOnlyPosition)
    {
        CCreature creature;
        CCreatureEditor creatureEditor(creature);
        const auto motionId = creatureEditor.AddNewMotion("Idle");
        ASSERT_NE(motionId, INVALID_MOTION_ID);

        auto motionEditor = creatureEditor.MotionEditor(motionId);
        ASSERT_TRUE(motionEditor.has_value());
        auto editor = motionEditor->SkeletonEditor();
        const auto id = editor.AddPart("Body");

        ASSERT_TRUE(editor.SetPartRotation(id, 0.5f));
        ASSERT_TRUE(editor.SetPartScale(id, { 2.0f, 3.0f }));
        ASSERT_TRUE(editor.SetPartPivot(id, { 0.25f, 0.75f }));

        ASSERT_TRUE(editor.SetPartPosition(id, { 10.0f, 20.0f }));

        const auto* part = creature.FindMotionById(motionId)->GetSkeleton().FindPartById(id);
        ASSERT_NE(part, nullptr);

        EXPECT_EQ(part->bindTransform.GetPosition(), Vec2({ 10.0f, 20.0f }));
        EXPECT_FLOAT_EQ(part->bindTransform.GetRotation(), 0.5f);
        EXPECT_EQ(part->bindTransform.GetScale(), Vec2({ 2.0f, 3.0f }));
        EXPECT_EQ(part->pivot, Vec2({ 0.25f, 0.75f }));
    }

    TEST(SkeletonEditorTest, SetOperationsRejectUnknownPart)
    {
        CCreature creature;
        CCreatureEditor creatureEditor(creature);
        const auto motionId = creatureEditor.AddNewMotion("Idle");
        ASSERT_NE(motionId, INVALID_MOTION_ID);

        auto motionEditor = creatureEditor.MotionEditor(motionId);
        ASSERT_TRUE(motionEditor.has_value());
        auto editor = motionEditor->SkeletonEditor();

        constexpr PartId unknownId = 12345;

        EXPECT_FALSE(editor.SetPartPosition(unknownId, { 1.0f, 2.0f }));
        EXPECT_FALSE(editor.SetPartRotation(unknownId, 0.5f));
        EXPECT_FALSE(editor.SetPartScale(unknownId, { 2.0f, 2.0f }));
        EXPECT_FALSE(editor.SetPartPivot(unknownId, { 0.5f, 0.5f }));

        CTransform2D transform;
        EXPECT_FALSE(editor.SetPartTransform(unknownId, transform));
        EXPECT_FALSE(editor.SetPartPivotAndTransform(unknownId, { 0.5f, 0.5f }, transform));
    }

    TEST(SkeletonEditorTest, SetPartPivotAndTransformUpdatesBoth)
    {
        CCreature creature;
        CCreatureEditor creatureEditor(creature);
        const auto motionId = creatureEditor.AddNewMotion("Idle");
        ASSERT_NE(motionId, INVALID_MOTION_ID);

        auto motionEditor = creatureEditor.MotionEditor(motionId);
        ASSERT_TRUE(motionEditor.has_value());
        auto editor = motionEditor->SkeletonEditor();
        const auto id = editor.AddPart("Body");

        ASSERT_NE(id, INVALID_PART_ID);

        const Vec2 pivot{ 0.25f, 0.75f };

        CTransform2D transform;
        transform.SetPosition({ 10.0f, 20.0f });
        transform.SetScale({ 2.0f, 3.0f });
        transform.SetRotation(0.5f);

        ASSERT_TRUE(editor.SetPartPivotAndTransform(id, pivot, transform));

        const auto part = creature.FindMotionById(motionId)->GetSkeleton().FindPartById(id);

        ASSERT_NE(part, nullptr);
        EXPECT_EQ(part->pivot, pivot);
        EXPECT_EQ(part->bindTransform.GetPosition(), transform.GetPosition());
        EXPECT_EQ(part->bindTransform.GetScale(), transform.GetScale());
        EXPECT_FLOAT_EQ(part->bindTransform.GetRotation(), transform.GetRotation());
    }

    TEST(CreatureEditorTest, RemoveParentPartRemovesDescendantsAndTheirReferences)
    {
        CCreature creature;
        CCreatureEditor creatureEditor(creature);
        const auto motionId = creatureEditor.AddNewMotion("Idle");
        ASSERT_NE(motionId, INVALID_MOTION_ID);

        auto motionEditor = creatureEditor.MotionEditor(motionId);
        ASSERT_TRUE(motionEditor.has_value());
        auto skeletonEditor = motionEditor->SkeletonEditor();

        const auto bodyId = skeletonEditor.AddPart("Body");
        const auto headId = skeletonEditor.AddPart("Head", bodyId);
        const auto eyesId = skeletonEditor.AddPart("Eyes", headId);
        const auto earId = skeletonEditor.AddPart("Ear", headId);

        ASSERT_NE(bodyId, INVALID_PART_ID);
        ASSERT_NE(headId, INVALID_PART_ID);
        ASSERT_NE(eyesId, INVALID_PART_ID);
        ASSERT_NE(earId, INVALID_PART_ID);

        auto appearanceEditor = motionEditor->AppearanceEditor();
        appearanceEditor.SetTexture(headId, L"head.png");
        appearanceEditor.SetTexture(eyesId, L"eyes.png");
        appearanceEditor.SetTexture(earId, L"ear.png");

        const CAnimationTrackKey headTrack{
            headId,
            AnimationProperty::Rotation
        };

        const CAnimationTrackKey eyesTrack{
            eyesId,
            AnimationProperty::PositionY
        };

        const CAnimationTrackKey earTrack{
            earId,
            AnimationProperty::Rotation
        };

        auto animationEditor = motionEditor->AnimationEditor();
        ASSERT_TRUE(animationEditor.AddTrack(CAnimationTrack{ headTrack }));
        ASSERT_TRUE(animationEditor.AddTrack(CAnimationTrack{ eyesTrack }));
        ASSERT_TRUE(animationEditor.AddTrack(CAnimationTrack{ earTrack }));

        ASSERT_TRUE(skeletonEditor.RemovePart(headId));

        const auto motion = creature.FindMotionById(motionId);
        ASSERT_NE(motion, nullptr);
        EXPECT_NE(motion->GetSkeleton().FindPartById(bodyId), nullptr);
        EXPECT_EQ(motion->GetSkeleton().FindPartById(headId), nullptr);
        EXPECT_EQ(motion->GetSkeleton().FindPartById(eyesId), nullptr);
        EXPECT_EQ(motion->GetSkeleton().FindPartById(earId), nullptr);

        EXPECT_EQ(motion->GetAppearance().FindByPartId(headId), nullptr);
        EXPECT_EQ(motion->GetAppearance().FindByPartId(eyesId), nullptr);
        EXPECT_EQ(motion->GetAppearance().FindByPartId(earId), nullptr);

        const auto& animation = motion->GetAnimation();
        EXPECT_EQ(animation.FindAnimationTrack(headTrack), nullptr);
        EXPECT_EQ(animation.FindAnimationTrack(eyesTrack), nullptr);
        EXPECT_EQ(animation.FindAnimationTrack(earTrack), nullptr);
    }
}
