#include "pch.h"

#include "Matrix3x2.h"
#include "Vec2.h"

using namespace Creature::Math;

namespace
{
    constexpr float PI = 3.14159265358979323846f;
}

namespace Matrix
{

    TEST(Matrix3x2Tests, DefaultMatrixIsIdentity)
    {
        const CMatrix3x2 matrix;

        const Vec2 point{ 10.0f, 20.0f };
        const auto result = matrix.TransformPoint(point);

        EXPECT_FLOAT_EQ(result.x, 10.0f);
        EXPECT_FLOAT_EQ(result.y, 20.0f);
    }

    TEST(Matrix3x2Tests, TranslationTransformsPoint)
    {
        const auto matrix =
            CMatrix3x2::CreateTranslation({ 10.0f, 20.0f });

        const auto result = matrix.TransformPoint({ 5.0f, 3.0f });

        EXPECT_FLOAT_EQ(result.x, 15.0f);
        EXPECT_FLOAT_EQ(result.y, 23.0f);
    }

    TEST(Matrix3x2Tests, PositiveHalfPiRotationRotatesXAxisToYAxis)
    {
        const auto matrix =
            CMatrix3x2::CreateRotation(
                PI * 0.5f);

        const auto result =
            matrix.TransformPoint({ 1.0f, 0.0f });

        EXPECT_NEAR(result.x, 0.0f, 1.0e-6f);
        EXPECT_NEAR(result.y, 1.0f, 1.0e-6f);
    }

    TEST(Matrix3x2Tests, ScaleTransformsPoint)
    {
        const auto matrix =
            CMatrix3x2::CreateScale({ 2.0f, 3.0f });

        const auto result =
            matrix.TransformPoint({ 4.0f, 5.0f });

        EXPECT_FLOAT_EQ(result.x, 8.0f);
        EXPECT_FLOAT_EQ(result.y, 15.0f);
    }

    TEST(Matrix3x2Tests, MultiplicationAppliesLeftTransformThenRightTransform)
    {
        const auto scale =
            CMatrix3x2::CreateScale({ 2.0f, 2.0f });

        const auto translation =
            CMatrix3x2::CreateTranslation({ 10.0f, 0.0f });

        const auto matrix = scale * translation;

        const auto result =
            matrix.TransformPoint({ 1.0f, 0.0f });

        // (1,0) -> scale -> (2,0) -> translation -> (12,0)
        EXPECT_FLOAT_EQ(result.x, 12.0f);
        EXPECT_FLOAT_EQ(result.y, 0.0f);
    }

    TEST(Matrix3x2Tests, InverseRestoresOriginalPoint)
    {
        const auto matrix =
            CMatrix3x2::CreateScale({ 2.0f, 3.0f }) *
            CMatrix3x2::CreateRotation(0.5f) *
            CMatrix3x2::CreateTranslation({ 100.0f, 50.0f });

        CMatrix3x2 inverse;
        ASSERT_TRUE(matrix.TryInverse(inverse));

        const Vec2 source{ 12.0f, 34.0f };

        const auto transformed = matrix.TransformPoint(source);
        const auto restored = inverse.TransformPoint(transformed);

        EXPECT_NEAR(restored.x, source.x, 1.0e-5f);
        EXPECT_NEAR(restored.y, source.y, 1.0e-5f);
    }

    TEST(Matrix3x2Tests, SingularMatrixCannotBeInverted)
    {
        const auto matrix =
            CMatrix3x2::CreateScale({ 0.0f, 1.0f });

        CMatrix3x2 inverse;

        EXPECT_FALSE(matrix.TryInverse(inverse));
    }

    TEST(Matrix3x2Tests, ToArrayUsesExpectedElementOrder)
    {
        const CMatrix3x2 matrix(
            1.0f, 2.0f,
            3.0f, 4.0f,
            5.0f, 6.0f);

        EXPECT_EQ(
            matrix.ToArray(),
            (std::array<float, 6>{
            1.0f, 2.0f,
                3.0f, 4.0f,
                5.0f, 6.0f
        }));
    }
}
