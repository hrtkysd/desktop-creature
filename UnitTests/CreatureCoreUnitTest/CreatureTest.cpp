#include "pch.h"

#include "Animation.h"
#include "AnimationEditor.h"
#include "AnimationProperty.h"
#include "Appearance.h"
#include "AppearanceEditor.h"
#include "Creature.h"
#include "CreatureEditor.h"
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

    TEST(CreatureEditorTest, AddNewAnimationAddsAnimation)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto id = editor.AddNewAnimation("Idle");

        ASSERT_NE(id, INVALID_ANIMATION_ID);

        const auto animation = creature.FindAnimationById(id);
        ASSERT_NE(animation, nullptr);

        EXPECT_EQ(animation->GetName(), "Idle");
    }

    TEST(CreatureEditorTest, AddNewAnimationGeneratesUniqueIds)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto idleId = editor.AddNewAnimation("Idle");
        const auto walkId = editor.AddNewAnimation("Walk");
        const auto sleepId = editor.AddNewAnimation("Sleep");

        EXPECT_NE(idleId, INVALID_ANIMATION_ID);
        EXPECT_NE(walkId, INVALID_ANIMATION_ID);
        EXPECT_NE(sleepId, INVALID_ANIMATION_ID);

        EXPECT_NE(idleId, walkId);
        EXPECT_NE(idleId, sleepId);
        EXPECT_NE(walkId, sleepId);
    }

    TEST(CreatureEditorTest, AddAnimationWithIdUpdatesNextGeneratedId)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        constexpr AnimationId explicitId = 100;

        const auto addedId = editor.AddAnimationWithId(explicitId, "Imported");
        ASSERT_EQ(addedId, explicitId);

        const auto generatedId = editor.AddNewAnimation("Idle");
        EXPECT_EQ(generatedId, explicitId + 1);
    }

    TEST(CreatureEditorTest, AddAnimationWithIdRejectsDuplicateId)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        constexpr AnimationId id = 10;

        ASSERT_EQ(editor.AddAnimationWithId(id, "Idle"), id);
        EXPECT_EQ(editor.AddAnimationWithId(id, "Walk"), INVALID_ANIMATION_ID);
        EXPECT_EQ(creature.GetAnimations().size(), 1u);
    }

    TEST(CreatureEditorTest, AddAnimationWithIdRejectsInvalidId)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto id = editor.AddAnimationWithId(INVALID_ANIMATION_ID, "Invalid");

        EXPECT_EQ(id, INVALID_ANIMATION_ID);
        EXPECT_TRUE(creature.GetAnimations().empty());
    }

    TEST(CreatureEditorTest, RemoveAnimationRemovesSpecifiedAnimation)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto idleId = editor.AddNewAnimation("Idle");
        const auto walkId = editor.AddNewAnimation("Walk");

        ASSERT_NE(idleId, INVALID_ANIMATION_ID);
        ASSERT_NE(walkId, INVALID_ANIMATION_ID);

        EXPECT_TRUE(editor.RemoveAnimation(idleId));

        EXPECT_EQ(creature.FindAnimationById(idleId), nullptr);
        EXPECT_NE(creature.FindAnimationById(walkId), nullptr);

        EXPECT_EQ(creature.GetAnimations().size(), 1u);
    }

    TEST(CreatureEditorTest, RemoveAnimationReturnsFalseForUnknownId)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        EXPECT_FALSE(editor.RemoveAnimation(12345));
    }

    TEST(CreatureTest, CloneCreatesIndependentCopy)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        editor.SetName("Original");

        const auto animationId = editor.AddNewAnimation("Idle");

        ASSERT_NE(animationId, INVALID_ANIMATION_ID);

        auto clone = creature.Clone();

        EXPECT_EQ(clone.GetName(), "Original");
        EXPECT_NE(clone.FindAnimationById(animationId), nullptr);

        CCreatureEditor cloneEditor(clone);
        cloneEditor.SetName("Clone");

        EXPECT_EQ(creature.GetName(), "Original");
        EXPECT_EQ(clone.GetName(), "Clone");
    }

    TEST(CreatureTest, CloneOwnsIndependentAnimations)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        const auto id = editor.AddNewAnimation("Idle");
        ASSERT_NE(id, INVALID_ANIMATION_ID);

        auto clone = creature.Clone();
        CCreatureEditor cloneEditor(clone);

        auto animationEditor = cloneEditor.GetAnimationEditor();
        ASSERT_TRUE(animationEditor.SetName(id, "Changed"));

        ASSERT_NE(creature.FindAnimationById(id), nullptr);
        ASSERT_NE(clone.FindAnimationById(id), nullptr);

        EXPECT_EQ(creature.FindAnimationById(id)->GetName(), "Idle");

        EXPECT_EQ(clone.FindAnimationById(id)->GetName(), "Changed");
    }

    TEST(CreatureEditorTest, ChildEditorUsesCurrentStateAfterMoveAssignment)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        CCreature replacement;
        CCreatureEditor replacementEditor(replacement);

        const auto id = replacementEditor.AddNewAnimation("Idle");
        ASSERT_NE(id, INVALID_ANIMATION_ID);

        creature = std::move(replacement);

        auto animationEditor = editor.GetAnimationEditor();
        EXPECT_TRUE(animationEditor.SetName(id, "Walk"));

        const auto animation = creature.FindAnimationById(id);
        ASSERT_NE(animation, nullptr);
        EXPECT_EQ(animation->GetName(), "Walk");
    }

    TEST(CreatureEditorTest, RemovePartRemovesAllReferencesToPart)
    {
        CCreature creature;
        CCreatureEditor editor(creature);

        // Arrange: Skeleton
        auto skeletonEditor = editor.GetSkeletonEditor();
        const auto bodyId = skeletonEditor.AddPart("Body");
        const auto headId = skeletonEditor.AddPart("Head", bodyId);

        ASSERT_NE(bodyId, INVALID_PART_ID);
        ASSERT_NE(headId, INVALID_PART_ID);

        // Arrange: Appearance
        auto appearanceEditor = editor.GetAppearanceEditor();
        appearanceEditor.SetTexture(bodyId, L"assets/body.png");
        appearanceEditor.SetTexture(headId, L"assets/head.png");

        // Arrange: Animation
        const auto animationId = editor.AddNewAnimation("Idle");
        ASSERT_NE(animationId, INVALID_ANIMATION_ID);

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

        auto animationEditor = editor.GetAnimationEditor();
        ASSERT_TRUE(animationEditor.AddTrack(
            animationId,
            CAnimationTrack{ bodyTrackKey }));

        ASSERT_TRUE(animationEditor.AddTrack(
            animationId,
            CAnimationTrack{ headTrackKey }));

        // Preconditions
        const auto& skeleton = creature.GetSkeleton();
        ASSERT_NE(skeleton.FindPartById(bodyId), nullptr);
        ASSERT_NE(skeleton.FindPartById(headId), nullptr);

        const auto& appearance = creature.GetAppearance();
        ASSERT_NE(appearance.FindByPartId(bodyId), nullptr);
        ASSERT_NE(appearance.FindByPartId(headId), nullptr);

        const auto animationBefore = creature.FindAnimationById(animationId);
        ASSERT_NE(animationBefore, nullptr);
        ASSERT_NE(animationBefore->FindAnimationTrack(bodyTrackKey), nullptr);
        ASSERT_NE(animationBefore->FindAnimationTrack(headTrackKey), nullptr);

        // Act
        ASSERT_TRUE(skeletonEditor.RemovePart(headId));

        // Assert: Skeleton
        EXPECT_NE(skeleton.FindPartById(bodyId), nullptr);

        EXPECT_EQ(skeleton.FindPartById(headId), nullptr);

        // Assert: Appearance
        EXPECT_NE(appearance.FindByPartId(bodyId), nullptr);
        EXPECT_EQ(appearance.FindByPartId(headId), nullptr);

        // Assert: Animation
        const auto animationAfter = creature.FindAnimationById(animationId);
        ASSERT_NE(animationAfter, nullptr);

        EXPECT_NE(animationAfter->FindAnimationTrack(bodyTrackKey), nullptr);
        EXPECT_EQ(animationAfter->FindAnimationTrack(headTrackKey), nullptr);
    }

    TEST(SkeletonEditorTest, SetPartTransformUpdatesTransform)
    {
        CCreature creature;
        CCreatureEditor creatureEditor(creature);

        auto editor = creatureEditor.GetSkeletonEditor();
        const auto id = editor.AddPart("Body");

        CTransform2D transform;
        transform.SetPosition({ 10.0f, 20.0f });
        transform.SetScale({ 2.0f, 3.0f });
        transform.SetRotation(0.5f);

        ASSERT_TRUE(editor.SetPartTransform(id, transform));

        const auto part = creature.GetSkeleton().FindPartById(id);
        ASSERT_NE(part, nullptr);

        EXPECT_EQ(part->bindTransform.GetPosition(), Vec2({ 10.0f, 20.0f }));
        EXPECT_EQ(part->bindTransform.GetScale(), Vec2({ 2.0f, 3.0f }));
        EXPECT_FLOAT_EQ(part->bindTransform.GetRotation(), 0.5f);
    }

    TEST(SkeletonEditorTest, SetPartPositionUpdatesOnlyPosition)
    {
        CCreature creature;
        CCreatureEditor creatureEditor(creature);

        auto editor = creatureEditor.GetSkeletonEditor();
        const auto id = editor.AddPart("Body");

        ASSERT_TRUE(editor.SetPartRotation(id, 0.5f));
        ASSERT_TRUE(editor.SetPartScale(id, { 2.0f, 3.0f }));
        ASSERT_TRUE(editor.SetPartPivot(id, { 0.25f, 0.75f }));

        ASSERT_TRUE(editor.SetPartPosition(id, { 10.0f, 20.0f }));

        const auto* part = creature.GetSkeleton().FindPartById(id);
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

        auto editor = creatureEditor.GetSkeletonEditor();

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

        auto editor = creatureEditor.GetSkeletonEditor();
        const auto id = editor.AddPart("Body");

        ASSERT_NE(id, INVALID_PART_ID);

        const Vec2 pivot{ 0.25f, 0.75f };

        CTransform2D transform;
        transform.SetPosition({ 10.0f, 20.0f });
        transform.SetScale({ 2.0f, 3.0f });
        transform.SetRotation(0.5f);

        ASSERT_TRUE(editor.SetPartPivotAndTransform(id, pivot, transform));

        const auto part = creature.GetSkeleton().FindPartById(id);

        ASSERT_NE(part, nullptr);
        EXPECT_EQ(part->pivot, pivot);
        EXPECT_EQ(part->bindTransform.GetPosition(), transform.GetPosition());
        EXPECT_EQ(part->bindTransform.GetScale(), transform.GetScale());
        EXPECT_FLOAT_EQ(part->bindTransform.GetRotation(), transform.GetRotation());
    }
}
