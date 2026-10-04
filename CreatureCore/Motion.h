#pragma once

#include "MotionId.h"

#include <memory>
#include <string>

namespace Creature
{
    class CAppearance;
    class CCreature;
    class CSkeleton;

    namespace Animation
    {
        class CAnimation;
    }

    namespace Editor
    {
        class CMotionEditor;
    }

    class CMotion
    {
        friend class CCreature;
        friend class Editor::CMotionEditor;
    private:
        struct MotionImpl;
    public:
        ~CMotion();

        CMotion(const CMotion&) = delete;
        CMotion& operator=(const CMotion&) = delete;

        CMotion(CMotion&&) noexcept;
        CMotion& operator=(CMotion&&) noexcept;
    private:
        explicit CMotion(std::unique_ptr<MotionImpl> impl) noexcept;
        explicit CMotion(MotionId id, const std::string& strName) noexcept;
    public:
        const CSkeleton& GetSkeleton() const;
        const CAppearance& GetAppearance() const;
        const Animation::CAnimation& GetAnimation() const;
        const std::string& GetName() const;
        MotionId GetMotionId() const;

        CMotion Clone() const;
    private:
        Creature::CSkeleton& MutableSkeleton();
        Creature::CAppearance& MutableAppearance();
        Creature::Animation::CAnimation& MutableAnimation();
    private:
        static CMotion NewMotion(MotionId id, const std::string& strName);
    private:
        void SetMotionId(MotionId id);
    private:
        std::unique_ptr<MotionImpl> m_impl;
    };
}
