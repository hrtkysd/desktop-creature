#pragma once

namespace Creature
{
    class CSkeleton;

    struct CreaturePose;
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

            static Math::CMatrix3x2 BuildWorld(
                const Part& part,
                const CSkeleton& skeleton,
                const CreaturePose& pose);
        };
    } // namespace Math
} // namespace Creature
