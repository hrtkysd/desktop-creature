#include "pch.h"
#include "Animation.h"
#include "AnimationEditor.h"
#include "AnimationProperty.h"
#include "AnimationTrack.h"
#include "AnimationTrackKey.h"
#include "Appearance.h"
#include "AppearanceEditor.h"
#include "Creature.h"
#include "CreatureEditor.h"
#include "Motion.h"
#include "MotionId.h"
#include "Skeleton.h"
#include "SkeletonEditor.h"

using namespace Creature;
using namespace Creature::Animation;
using namespace Creature::Editor;
using namespace Creature::Math;

namespace Creature
{
    // NOLINTBEGIN(bugprone-unchecked-optional-access)
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

    TEST(CreatureTest, CloneOwnsIndependentMotionState)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto motionId = editor.AddNewMotion("Idle");
        ASSERT_NE(motionId, INVALID_MOTION_ID);

        auto motionEditor = editor.MotionEditor(motionId);
        ASSERT_TRUE(motionEditor.has_value());

        const auto bodyId = motionEditor->SkeletonEditor().AddPart("Body");
        ASSERT_NE(bodyId, INVALID_PART_ID);

        auto clone = creature.Clone();
        CCreatureEditor cloneEditor(clone);

        auto cloneMotionEditor = cloneEditor.MotionEditor(motionId);
        ASSERT_TRUE(cloneMotionEditor.has_value());

        const auto headId = cloneMotionEditor->SkeletonEditor().AddPart("Head");
        ASSERT_NE(headId, INVALID_PART_ID);

        const auto originalMotion = creature.FindMotionById(motionId);
        const auto clonedMotion = clone.FindMotionById(motionId);

        ASSERT_NE(originalMotion, nullptr);
        ASSERT_NE(clonedMotion, nullptr);

        EXPECT_EQ(originalMotion->GetSkeleton().Parts().size(), 1u);
        EXPECT_EQ(clonedMotion->GetSkeleton().Parts().size(), 2u);
    }

    TEST(CreatureTest, ClonePreservesNextMotionId)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        constexpr MotionId explicitId = 100;
        ASSERT_EQ(editor.AddMotionWithId(explicitId, "Imported"), explicitId);

        auto clone = creature.Clone();
        CCreatureEditor cloneEditor(clone);
        EXPECT_EQ(cloneEditor.AddNewMotion("Walk"), explicitId + 1);
    }

    TEST(MotionEditorTest, RemovePartRemovesAllReferencesToPart)
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

        const auto motionBefore = creature.FindMotionById(motionId);

        ASSERT_NE(motionBefore, nullptr);

        ASSERT_NE(motionBefore->GetSkeleton().FindPartById(bodyId), nullptr);
        ASSERT_NE(motionBefore->GetSkeleton().FindPartById(headId), nullptr);

        ASSERT_NE(motionBefore->GetAppearance().FindByPartId(bodyId), nullptr);
        ASSERT_NE(motionBefore->GetAppearance().FindByPartId(headId), nullptr);

        ASSERT_NE(motionBefore->GetAnimation().FindAnimationTrack(bodyTrackKey), nullptr);
        ASSERT_NE(motionBefore->GetAnimation().FindAnimationTrack(headTrackKey), nullptr);

        ASSERT_TRUE(skeletonEditor.RemovePart(headId));

        const auto motionAfter = creature.FindMotionById(motionId);
        ASSERT_NE(motionAfter, nullptr);

        EXPECT_NE(motionAfter->GetSkeleton().FindPartById(bodyId), nullptr);
        EXPECT_EQ(motionAfter->GetSkeleton().FindPartById(headId), nullptr);
        EXPECT_NE(motionAfter->GetAppearance().FindByPartId(bodyId), nullptr);
        EXPECT_EQ(motionAfter->GetAppearance().FindByPartId(headId), nullptr);
        EXPECT_NE(motionAfter->GetAnimation().FindAnimationTrack(bodyTrackKey), nullptr);
        EXPECT_EQ(motionAfter->GetAnimation().FindAnimationTrack(headTrackKey), nullptr);
    }

    TEST(MotionEditorTest, RemoveParentPartRemovesDescendantsAndReferences)
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

        const CAnimationTrackKey headTrack
        {
            headId,
            AnimationProperty::Rotation
        };

        const CAnimationTrackKey eyesTrack
        {
            eyesId,
            AnimationProperty::PositionY
        };

        const CAnimationTrackKey earTrack
        {
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
        EXPECT_EQ(motion->GetAnimation().FindAnimationTrack(headTrack), nullptr);
        EXPECT_EQ(motion->GetAnimation().FindAnimationTrack(eyesTrack), nullptr);
        EXPECT_EQ(motion->GetAnimation().FindAnimationTrack(earTrack), nullptr);
    }

    TEST(SkeletonEditorTest, SetPartTransformUpdatesTransform)
    {
        CCreature creature;
        CCreatureEditor creatureEditor(creature);

        const auto motionId = creatureEditor.AddNewMotion("Idle");
        ASSERT_NE(motionId, INVALID_MOTION_ID);

        auto motionEditor = creatureEditor.MotionEditor(motionId);
        ASSERT_TRUE(motionEditor.has_value());

        auto skeletonEditor = motionEditor->SkeletonEditor();
        const auto id = skeletonEditor.AddPart("Body");

        CTransform2D transform;
        transform.SetPosition({ 10.0f, 20.0f });
        transform.SetScale({ 2.0f, 3.0f });
        transform.SetRotation(0.5f);

        ASSERT_TRUE(skeletonEditor.SetPartTransform(id, transform));

        const auto part = skeletonEditor.GetSkeleton().FindPartById(id);
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

        auto skeletonEditor = motionEditor->SkeletonEditor();
        const auto id = skeletonEditor.AddPart("Body");

        ASSERT_TRUE(skeletonEditor.SetPartRotation(id, 0.5f));
        ASSERT_TRUE(skeletonEditor.SetPartScale(id, { 2.0f, 3.0f }));
        ASSERT_TRUE(skeletonEditor.SetPartPivot(id, { 0.25f, 0.75f }));

        ASSERT_TRUE(skeletonEditor.SetPartPosition(id, { 10.0f, 20.0f }));

        const auto part = skeletonEditor.GetSkeleton().FindPartById(id);
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

        auto skeletonEditor = motionEditor->SkeletonEditor();

        constexpr PartId unknownId = 12345;

        EXPECT_FALSE(skeletonEditor.SetPartPosition(unknownId, { 1.0f, 2.0f }));
        EXPECT_FALSE(skeletonEditor.SetPartRotation(unknownId, 0.5f));
        EXPECT_FALSE(skeletonEditor.SetPartScale(unknownId, { 2.0f, 2.0f }));
        EXPECT_FALSE(skeletonEditor.SetPartPivot(unknownId, { 0.5f, 0.5f }));

        CTransform2D transform;
        EXPECT_FALSE(skeletonEditor.SetPartTransform(unknownId, transform));
        EXPECT_FALSE(skeletonEditor.SetPartPivotAndTransform(unknownId, { 0.5f, 0.5f }, transform));
    }

    TEST(SkeletonEditorTest, SetPartPivotAndTransformUpdatesBoth)
    {
        CCreature creature;
        CCreatureEditor creatureEditor(creature);

        const auto motionId = creatureEditor.AddNewMotion("Idle");
        ASSERT_NE(motionId, INVALID_MOTION_ID);

        auto motionEditor = creatureEditor.MotionEditor(motionId);
        ASSERT_TRUE(motionEditor.has_value());

        auto skeletonEditor = motionEditor->SkeletonEditor();
        const auto id = skeletonEditor.AddPart("Body");

        ASSERT_NE(id, INVALID_PART_ID);

        const Vec2 pivot{ 0.25f, 0.75f };

        CTransform2D transform;
        transform.SetPosition({ 10.0f, 20.0f });
        transform.SetScale({ 2.0f, 3.0f });
        transform.SetRotation(0.5f);

        ASSERT_TRUE(skeletonEditor.SetPartPivotAndTransform(id, pivot, transform));

        const auto part = skeletonEditor.GetSkeleton().FindPartById(id);
        ASSERT_NE(part, nullptr);
        EXPECT_EQ(part->pivot, pivot);
        EXPECT_EQ(part->bindTransform.GetPosition(), transform.GetPosition());
        EXPECT_EQ(part->bindTransform.GetScale(), transform.GetScale());
        EXPECT_FLOAT_EQ(part->bindTransform.GetRotation(), transform.GetRotation());
    }
    // NOLINTEND(bugprone-unchecked-optional-access)
}
