#include "pch.h"

#include "Transform2D.h"

using namespace Creature::Math;

namespace
{
    constexpr float PI = 3.14159265358979323846f;
}

namespace Transform2D
{
    TEST(Transform2DTests, ToMatrixAppliesScaleRotationThenTranslation)
    {
        const CTransform2D transform(
            { 10.0f, 20.0f },
            { 2.0f, 2.0f },
            PI * 0.5f);

        const auto matrix = transform.ToMatrix();

        const auto result =
            matrix.TransformPoint({ 1.0f, 0.0f });

        // Scale:
        // (1,0) -> (2,0)
        //
        // Rotate +90°:
        // (2,0) -> (0,2)
        //
        // Translate:
        // (0,2) -> (10,22)

        EXPECT_NEAR(result.x, 10.0f, 1.0e-5f);
        EXPECT_NEAR(result.y, 22.0f, 1.0e-5f);
    }
}
