#include "pch.h"
#include "Animation.h"
#include "AnimationEditor.h"
#include "AnimationPlayer.h"
#include "AnimationProperty.h"
#include "AnimationTrack.h"
#include "AnimationTrackKey.h"
#include "Creature.h"
#include "CreatureEditor.h"
#include "Motion.h"
#include "MotionEditor.h"
#include "MotionId.h"
#include "Skeleton.h"
#include "SkeletonEditor.h"
#include "Transform2D.h"

using namespace Creature;
using namespace Creature::Animation;
using namespace Creature::Editor;

namespace AnimationPlayer
{
    // NOLINTBEGIN(bugprone-unchecked-optional-access)
    class AnimationPlayerTests : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            m_motionId = m_creatureEditor.AddNewMotion("Idle");
            ASSERT_NE(m_motionId, INVALID_MOTION_ID);

            auto motionEditor = m_creatureEditor.MotionEditor(m_motionId);
            ASSERT_TRUE(motionEditor.has_value());

            m_partId = motionEditor->SkeletonEditor().AddPart("Body");
            ASSERT_NE(m_partId, INVALID_PART_ID);

            ASSERT_TRUE(motionEditor->AnimationEditor().SetDuration(1.0f));
        }

        void AddConstantTrack(
            AnimationProperty property,
            float value)
        {
            auto motionEditor = m_creatureEditor.MotionEditor(m_motionId);
            ASSERT_TRUE(motionEditor.has_value());

            auto editor = motionEditor->AnimationEditor();

            const CAnimationTrackKey key
            {
                m_partId,
                property
            };

            ASSERT_TRUE(editor.AddTrack(CAnimationTrack{ key }));

            ASSERT_TRUE(editor.AddOrUpdateKeyFrame(key, { 0.0f, value }));
        }

        const CMotion& Motion() const
        {
            const auto motion = m_creature.FindMotionById(m_motionId);
            EXPECT_NE(motion, nullptr);
            return *motion;
        }

        const CAnimation& Animation() const
        {
            return Motion().GetAnimation();
        }

        const CSkeleton& Skeleton() const
        {
            return Motion().GetSkeleton();
        }

    protected:
        CCreature m_creature;
        CCreatureEditor m_creatureEditor{ m_creature };
        CAnimationPlayer m_player;

        MotionId m_motionId = INVALID_MOTION_ID;
        PartId m_partId = INVALID_PART_ID;
    };

    TEST_F(AnimationPlayerTests, UpdateDoesNotAdvanceWhileStopped)
    {
        m_player.Update(0.5f);

        EXPECT_FLOAT_EQ(m_player.GetCurrentAnimationTime(), 0.0f);
    }

    TEST_F(AnimationPlayerTests, UpdateAdvancesWhilePlaying)
    {
        m_player.Play();
        m_player.Update(0.5f);

        EXPECT_FLOAT_EQ(m_player.GetCurrentAnimationTime(), 0.5f);
    }

    TEST_F(AnimationPlayerTests, PauseStopsTimeAdvancement)
    {
        m_player.Play();
        m_player.Update(0.25f);

        m_player.Pause();
        m_player.Update(0.5f);

        EXPECT_FLOAT_EQ(m_player.GetCurrentAnimationTime(), 0.25f);
    }

    TEST_F(AnimationPlayerTests, PlayAfterPauseResumesCurrentTime)
    {
        m_player.Play();
        m_player.Update(0.25f);

        m_player.Pause();
        m_player.Play();
        m_player.Update(0.25f);

        EXPECT_FLOAT_EQ(m_player.GetCurrentAnimationTime(), 0.5f);
    }

    TEST_F(AnimationPlayerTests, StopResetsCurrentTime)
    {
        m_player.Play();
        m_player.Update(0.5f);

        m_player.Stop();

        EXPECT_FLOAT_EQ(m_player.GetCurrentAnimationTime(), 0.0f);
    }

    TEST_F(AnimationPlayerTests, SamplePoseMapsAllProperties)
    {
        AddConstantTrack(AnimationProperty::PositionX, 10.0f);
        AddConstantTrack(AnimationProperty::PositionY, 20.0f);
        AddConstantTrack(AnimationProperty::Rotation, 0.5f);
        AddConstantTrack(AnimationProperty::ScaleX, 2.0f);
        AddConstantTrack(AnimationProperty::ScaleY, 3.0f);

        m_player.SamplePose(Animation(), Skeleton());

        const auto& transforms = m_player.GetPose().GetPartTransform();
        ASSERT_EQ(transforms.size(), 1u);

        const auto& transform = transforms.front();
        EXPECT_FLOAT_EQ(transform.GetPosition().x, 10.0f);
        EXPECT_FLOAT_EQ(transform.GetPosition().y, 20.0f);
        EXPECT_FLOAT_EQ(transform.GetRotation(), 0.5f);
        EXPECT_FLOAT_EQ(transform.GetScale().x, 2.0f);
        EXPECT_FLOAT_EQ(transform.GetScale().y, 3.0f);
    }

    TEST_F(AnimationPlayerTests, SamplePoseWrapsCurrentTimeAtDuration)
    {
        m_player.SetCurrentAnimationTime(1.25f);
        m_player.SamplePose(Animation(), Skeleton());
        EXPECT_NEAR(m_player.GetCurrentAnimationTime(), 0.25f, 0.0001f);
    }
    // NOLINTEND(bugprone-unchecked-optional-access)
}
