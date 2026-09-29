#include "pch.h"

#include "AnimationProperty.h"
#include "AnimationTrack.h"
#include "AnimationTrackKey.h"
#include "Interpolation.h"

using namespace Creature;
using namespace Creature::Animation;

namespace
{
    CAnimationTrack CreateTrack()
    {
        return CAnimationTrack
        {
            CAnimationTrackKey
            {
                1,
                AnimationProperty::PositionX
            }
        };
    }
}

namespace AnimationTrack
{
    TEST(AnimationTrackTests, EmptyTrackReturnsZero)
    {
        const auto track = CreateTrack();

        EXPECT_FLOAT_EQ(track.Sample(0.0f), 0.0f);
        EXPECT_FLOAT_EQ(track.Sample(1.0f), 0.0f);
    }

    TEST(AnimationTrackTests, KeyFramesAreSortedByTime)
    {
        auto track = CreateTrack();

        track.AddOrUpdateKeyFrame({ 1.0f, 10.0f });
        track.AddOrUpdateKeyFrame({ 0.0f, 0.0f });
        track.AddOrUpdateKeyFrame({ 0.5f, 5.0f });

        const auto& frames = track.GetKeyFrames();

        ASSERT_EQ(frames.size(), 3u);

        EXPECT_FLOAT_EQ(frames[0].fTime, 0.0f);
        EXPECT_FLOAT_EQ(frames[1].fTime, 0.5f);
        EXPECT_FLOAT_EQ(frames[2].fTime, 1.0f);
    }

    TEST(AnimationTrackTests, SameTimeKeyFrameUpdatesExistingFrame)
    {
        auto track = CreateTrack();

        track.AddOrUpdateKeyFrame({ 0.5f, 5.0f });
        track.AddOrUpdateKeyFrame(
            {
                0.5f,
                10.0f,
                Interpolation::Step
            });

        const auto& frames = track.GetKeyFrames();

        ASSERT_EQ(frames.size(), 1u);

        EXPECT_FLOAT_EQ(frames[0].fValue, 10.0f);
        EXPECT_EQ(
            frames[0].eInterpolationToNext,
            Interpolation::Step);
    }

    TEST(AnimationTrackTests, SampleBeforeFirstKeyReturnsFirstValue)
    {
        auto track = CreateTrack();

        track.AddOrUpdateKeyFrame({ 1.0f, 10.0f });
        track.AddOrUpdateKeyFrame({ 2.0f, 20.0f });

        EXPECT_FLOAT_EQ(track.Sample(0.0f), 10.0f);
    }

    TEST(AnimationTrackTests, SampleAfterLastKeyReturnsLastValue)
    {
        auto track = CreateTrack();

        track.AddOrUpdateKeyFrame({ 1.0f, 10.0f });
        track.AddOrUpdateKeyFrame({ 2.0f, 20.0f });

        EXPECT_FLOAT_EQ(track.Sample(3.0f), 20.0f);
    }

    TEST(AnimationTrackTests, LinearInterpolationReturnsExpectedValue)
    {
        auto track = CreateTrack();

        track.AddOrUpdateKeyFrame(
            {
                0.0f,
                0.0f,
                Interpolation::Linear
            });

        track.AddOrUpdateKeyFrame(
            {
                1.0f,
                10.0f
            });

        EXPECT_FLOAT_EQ(track.Sample(0.25f), 2.5f);
        EXPECT_FLOAT_EQ(track.Sample(0.50f), 5.0f);
        EXPECT_FLOAT_EQ(track.Sample(0.75f), 7.5f);
    }

    TEST(AnimationTrackTests, SmoothStepInterpolationReturnsExpectedValue)
    {
        auto track = CreateTrack();

        track.AddOrUpdateKeyFrame(
            {
                0.0f,
                0.0f,
                Interpolation::SmoothStep
            });

        track.AddOrUpdateKeyFrame(
            {
                1.0f,
                10.0f
            });

        // SmoothStep(0.25) = 0.15625
        EXPECT_NEAR(track.Sample(0.25f), 1.5625f, 0.0001f);

        // SmoothStep(0.75) = 0.84375
        EXPECT_NEAR(track.Sample(0.75f), 8.4375f, 0.0001f);
    }

    TEST(AnimationTrackTests, StepInterpolationKeepsPreviousValue)
    {
        auto track = CreateTrack();

        track.AddOrUpdateKeyFrame(
            {
                0.0f,
                10.0f,
                Interpolation::Step
            });

        track.AddOrUpdateKeyFrame(
            {
                1.0f,
                20.0f
            });

        EXPECT_FLOAT_EQ(track.Sample(0.25f), 10.0f);
        EXPECT_FLOAT_EQ(track.Sample(0.99f), 10.0f);
    }

    TEST(AnimationTrackTests, ExactKeyFrameTimeReturnsThatKeyFrameValue)
    {
        auto track = CreateTrack();

        track.AddOrUpdateKeyFrame(
            {
                0.0f,
                0.0f,
                Interpolation::Step
            });

        track.AddOrUpdateKeyFrame(
            {
                0.5f,
                10.0f,
                Interpolation::Step
            });

        track.AddOrUpdateKeyFrame(
            {
                1.0f,
                20.0f
            });

        EXPECT_FLOAT_EQ(track.Sample(0.499f), 0.0f);
        EXPECT_FLOAT_EQ(track.Sample(0.500f), 10.0f);
    }

    TEST(AnimationTrackTests, NegativeKeyFrameTimeIsRejected)
    {
        auto track = CreateTrack();

        EXPECT_FALSE(
            track.AddOrUpdateKeyFrame(
                { -0.1f, 10.0f }));

        EXPECT_TRUE(track.GetKeyFrames().empty());
    }

    TEST(AnimationTrackTests, RejectedKeyFrameDoesNotModifyTrack)
    {
        auto track = CreateTrack();

        ASSERT_TRUE(
            track.AddOrUpdateKeyFrame(
                { 0.5f, 10.0f }));

        EXPECT_FALSE(
            track.AddOrUpdateKeyFrame(
                { -0.1f, 20.0f }));

        const auto& frames = track.GetKeyFrames();

        ASSERT_EQ(frames.size(), 1u);
        EXPECT_FLOAT_EQ(frames[0].fTime, 0.5f);
        EXPECT_FLOAT_EQ(frames[0].fValue, 10.0f);
    }
}
