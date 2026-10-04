#pragma once

#include "IMotionEditContext.h"
#include "PartId.h"

#include <string>

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
        public:
            const CMotion& GetMotion() const;
            void SetName(const std::string& strName);
            const std::string& GetName() const;
        private:
            bool RemovePart(PartId id) override;
        private:
            CMotion& m_motion;
        };
    }
}
