#pragma once

namespace Creature
{
    class CCreaturePose;
    class CSkeleton;

    struct Part;

    namespace Math
    {
        class CMatrix3x2;
        class CTransform2D;

        class CPartTransformBuilder
        {
        public:
            static Math::CMatrix3x2 BuildLocal(
                const Part& part,
                const Math::CTransform2D& transform);

            static CMatrix3x2 BuildWorld(
                const Part& part,
                const CSkeleton& skeleton);

            static Math::CMatrix3x2 BuildWorld(
                const Part& part,
                const CSkeleton& skeleton,
                const CCreaturePose& pose);
        };
    } // namespace Math
} // namespace Creature
