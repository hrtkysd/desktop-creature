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
#include "MotionEditor.h"
#include "MotionId.h"
#include "Skeleton.h"
#include "SkeletonEditor.h"

using namespace Creature;
using namespace Creature::Animation;
using namespace Creature::Editor;

namespace Creature
{
    // NOLINTBEGIN(bugprone-unchecked-optional-access)

    TEST(MotionEditorTest, MotionEditorRejectsInvalidAndUnknownIds)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        EXPECT_FALSE(editor.MotionEditor(INVALID_MOTION_ID).has_value());
        EXPECT_FALSE(editor.MotionEditor(12345).has_value());
    }

    TEST(MotionEditorTest, MotionEditorsMutateTheirMotion)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto motionId = editor.AddNewMotion("Idle");
        ASSERT_NE(motionId, INVALID_MOTION_ID);

        auto motionEditor = editor.MotionEditor(motionId);
        ASSERT_TRUE(motionEditor.has_value());

        auto skeletonEditor = motionEditor->SkeletonEditor();
        const auto partId = skeletonEditor.AddPart("Body");
        ASSERT_NE(partId, INVALID_PART_ID);

        motionEditor->AppearanceEditor().AddPart(partId, L"body.png");

        const CAnimationTrackKey trackKey
        {
            partId,
            AnimationProperty::PositionX
        };

        auto animationEditor = motionEditor->AnimationEditor();
        ASSERT_TRUE(animationEditor.SetDuration(2.0f));
        ASSERT_TRUE(animationEditor.AddTrack(CAnimationTrack{ trackKey }));

        const auto motion = creature.FindMotionById(motionId);
        ASSERT_NE(motion, nullptr);

        EXPECT_NE(motion->GetSkeleton().FindPartById(partId), nullptr);
        EXPECT_NE(motion->GetAppearance().FindByPartId(partId), nullptr);
        EXPECT_EQ(motion->GetAnimation().GetAnimationTracks().size(), 1u);
        EXPECT_FLOAT_EQ(motion->GetAnimation().GetDuration(), 2.0f);
    }

    TEST(MotionEditorTest, MotionsHaveIndependentSkeletonsAndAnimations)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto firstId = editor.AddNewMotion("First");
        const auto secondId = editor.AddNewMotion("Second");

        ASSERT_NE(firstId, INVALID_MOTION_ID);
        ASSERT_NE(secondId, INVALID_MOTION_ID);

        auto firstMotionEditor = editor.MotionEditor(firstId);
        auto secondMotionEditor = editor.MotionEditor(secondId);

        ASSERT_TRUE(firstMotionEditor.has_value());
        ASSERT_TRUE(secondMotionEditor.has_value());

        const auto firstPartId = firstMotionEditor->SkeletonEditor().AddPart("Body");
        ASSERT_NE(firstPartId, INVALID_PART_ID);

        const CAnimationTrackKey trackKey
        {
            firstPartId,
            AnimationProperty::Rotation
        };

        ASSERT_TRUE(firstMotionEditor->AnimationEditor().AddTrack(CAnimationTrack{ trackKey }));

        const auto firstMotion = creature.FindMotionById(firstId);
        const auto secondMotion = creature.FindMotionById(secondId);

        ASSERT_NE(firstMotion, nullptr);
        ASSERT_NE(secondMotion, nullptr);

        EXPECT_EQ(firstMotion->GetSkeleton().Parts().size(), 1u);
        EXPECT_TRUE(secondMotion->GetSkeleton().Parts().empty());
        EXPECT_EQ(firstMotion->GetAnimation().GetAnimationTracks().size(), 1u);
        EXPECT_TRUE(secondMotion->GetAnimation().GetAnimationTracks().empty());
    }

    TEST(MotionEditorTest, RemovePartRemovesAllReferencesToPart)
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
        appearanceEditor.AddPart(bodyId, L"assets/body.png");
        appearanceEditor.AddPart(headId, L"assets/head.png");

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

        ASSERT_TRUE(skeletonEditor.RemovePart(headId));

        const auto motion = creature.FindMotionById(motionId);
        ASSERT_NE(motion, nullptr);

        EXPECT_NE(motion->GetSkeleton().FindPartById(bodyId), nullptr);
        EXPECT_EQ(motion->GetSkeleton().FindPartById(headId), nullptr);

        EXPECT_NE(motion->GetAppearance().FindByPartId(bodyId), nullptr);
        EXPECT_EQ(motion->GetAppearance().FindByPartId(headId), nullptr);

        EXPECT_NE(motion->GetAnimation().FindAnimationTrack(bodyTrackKey), nullptr);
        EXPECT_EQ(motion->GetAnimation().FindAnimationTrack(headTrackKey), nullptr);
    }

    TEST(MotionEditorTest, RemoveParentPartRemovesDescendantsAndReferences)
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
        const auto eyesId = skeletonEditor.AddPart("Eyes", headId);
        const auto earId = skeletonEditor.AddPart("Ear", headId);

        ASSERT_NE(bodyId, INVALID_PART_ID);
        ASSERT_NE(headId, INVALID_PART_ID);
        ASSERT_NE(eyesId, INVALID_PART_ID);
        ASSERT_NE(earId, INVALID_PART_ID);

        auto appearanceEditor = motionEditor->AppearanceEditor();
        appearanceEditor.AddPart(headId, L"head.png");
        appearanceEditor.AddPart(eyesId, L"eyes.png");
        appearanceEditor.AddPart(earId, L"ear.png");

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

    TEST(MotionEditorTest, RemovingPartCascadesOnlyWithinItsMotion)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto firstId = editor.AddNewMotion("First");
        const auto secondId = editor.AddNewMotion("Second");

        ASSERT_NE(firstId, INVALID_MOTION_ID);
        ASSERT_NE(secondId, INVALID_MOTION_ID);

        auto firstMotionEditor = editor.MotionEditor(firstId);
        auto secondMotionEditor = editor.MotionEditor(secondId);

        ASSERT_TRUE(firstMotionEditor.has_value());
        ASSERT_TRUE(secondMotionEditor.has_value());

        auto firstSkeletonEditor = firstMotionEditor->SkeletonEditor();
        const auto firstParentId = firstSkeletonEditor.AddPart("Body");
        const auto firstChildId = firstSkeletonEditor.AddPart("Head", firstParentId);

        auto secondSkeletonEditor = secondMotionEditor->SkeletonEditor();
        const auto secondParentId = secondSkeletonEditor.AddPart("Body");
        const auto secondChildId = secondSkeletonEditor.AddPart("Head", secondParentId);

        ASSERT_NE(firstParentId, INVALID_PART_ID);
        ASSERT_NE(firstChildId, INVALID_PART_ID);
        ASSERT_NE(secondParentId, INVALID_PART_ID);
        ASSERT_NE(secondChildId, INVALID_PART_ID);

        ASSERT_EQ(firstParentId, secondParentId);
        ASSERT_EQ(firstChildId, secondChildId);

        const CAnimationTrackKey firstTrack
        {
            firstChildId,
            AnimationProperty::PositionX
        };

        const CAnimationTrackKey secondTrack
        {
            secondChildId,
            AnimationProperty::PositionX
        };

        firstMotionEditor->AppearanceEditor().AddPart(firstParentId, L"first-body.png");
        firstMotionEditor->AppearanceEditor().AddPart(firstChildId, L"first-head.png");

        secondMotionEditor->AppearanceEditor().AddPart(secondParentId, L"second-body.png");
        secondMotionEditor->AppearanceEditor().AddPart(secondChildId, L"second-head.png");

        ASSERT_TRUE(firstMotionEditor->AnimationEditor().AddTrack(CAnimationTrack{ firstTrack }));
        ASSERT_TRUE(secondMotionEditor->AnimationEditor().AddTrack(CAnimationTrack{ secondTrack }));

        ASSERT_TRUE(firstSkeletonEditor.RemovePart(firstParentId));

        const auto firstMotion = creature.FindMotionById(firstId);
        const auto secondMotion = creature.FindMotionById(secondId);

        ASSERT_NE(firstMotion, nullptr);
        ASSERT_NE(secondMotion, nullptr);

        EXPECT_EQ(firstMotion->GetSkeleton().FindPartById(firstParentId), nullptr);
        EXPECT_EQ(firstMotion->GetSkeleton().FindPartById(firstChildId), nullptr);
        EXPECT_EQ(firstMotion->GetAppearance().FindByPartId(firstParentId), nullptr);
        EXPECT_EQ(firstMotion->GetAppearance().FindByPartId(firstChildId), nullptr);
        EXPECT_EQ(firstMotion->GetAnimation().FindAnimationTrack(firstTrack), nullptr);

        EXPECT_NE(secondMotion->GetSkeleton().FindPartById(secondParentId), nullptr);
        EXPECT_NE(secondMotion->GetSkeleton().FindPartById(secondChildId), nullptr);
        EXPECT_NE(secondMotion->GetAppearance().FindByPartId(secondParentId), nullptr);
        EXPECT_NE(secondMotion->GetAppearance().FindByPartId(secondChildId), nullptr);
        EXPECT_NE(secondMotion->GetAnimation().FindAnimationTrack(secondTrack), nullptr);
    }

    // NOLINTEND(bugprone-unchecked-optional-access)
}
