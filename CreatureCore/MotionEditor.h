#pragma once

#include "IMotionEditContext.h"
#include "PartId.h"

namespace Creature
{
    class CMotion;

    namespace Editor
    {
        class CAppearanceEditor;
        class CAnimationEditor;
        class CSkeletonEditor;

        class CMotionEditor : private IMotionEditContext
        {
        public:
            explicit CMotionEditor(CMotion& motion);
        public:
            CAppearanceEditor AppearanceEditor();
            CAnimationEditor AnimationEditor();
            CSkeletonEditor SkeletonEditor();
        private:
            bool RemovePart(PartId id) override;
        private:
            CMotion& m_motion;
        };
    }
}
