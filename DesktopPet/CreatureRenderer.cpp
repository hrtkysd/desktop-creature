#include "CreatureRenderer.h"
#include "CreaturePose.h"
#include "Genome.h"

#include <cmath>
#include <iterator>

using namespace Creature;

namespace
{
    int GetLegCount(Legs legs)
    {
        switch (legs)
        {
        case Legs::None:
            return 0;

        case Legs::Two:
            return 2;

        case Legs::Four:
            return 4;
        }

        return 0;
    }
}

void CCreatureRenderer::Draw(HDC hdc, const RECT& client, const Genome& genome, const CCreaturePose& pose)
{
    //    const float centerX =
    //        (client.right - client.left) * 0.5f;
    //
    //    const float centerY =
    //        (client.bottom - client.top) * 0.5f;
    //
    //    //
    //    // Genome上の体格
    //    //
    //    float bodyWidth = 44.0f * genome.scale;
    //    float bodyHeight = 38.0f * genome.scale;
    //
    //    switch (genome.body)
    //    {
    //    case Body::Circle:
    //        bodyWidth *= 1.0f;
    //        bodyHeight *= 1.0f;
    //        break;
    //
    //    case Body::Oval:
    //        bodyWidth *= 1.15f;
    //        bodyHeight *= 0.85f;
    //        break;
    //
    //    case Body::Blob:
    //        bodyWidth *= 1.1f;
    //        bodyHeight *= 0.95f;
    //        break;
    //    }
    //
    //    //
    //    // Animationによる変形
    //    //
    //    bodyWidth *= pose.scaleX;
    //    bodyHeight *= pose.scaleY;
    //
    //    const float drawCenterX =
    //        centerX + pose.offsetX;
    //
    //    const float drawCenterY =
    //        centerY + pose.offsetY;
    //
    //    const int left =
    //        static_cast<int>(
    //            drawCenterX - bodyWidth * 0.5f);
    //
    //    const int right =
    //        static_cast<int>(
    //            drawCenterX + bodyWidth * 0.5f);
    //
    //    const int top =
    //        static_cast<int>(
    //            drawCenterY - bodyHeight * 0.5f);
    //
    //    const int bottom =
    //        static_cast<int>(
    //            drawCenterY + bodyHeight * 0.5f);
    //
    //    //
    //    // とりあえずSeedから個体色
    //    //
    //    const BYTE red =
    //        static_cast<BYTE>(
    //            100 + (genome.seed * 37) % 120);
    //
    //    const BYTE green =
    //        static_cast<BYTE>(
    //            100 + (genome.seed * 53) % 120);
    //
    //    const BYTE blue =
    //        static_cast<BYTE>(
    //            100 + (genome.seed * 71) % 120);
    //
    //    HBRUSH bodyBrush =
    //        CreateSolidBrush(
    //            RGB(red, green, blue));
    //
    //    HPEN outlinePen =
    //        CreatePen(
    //            PS_SOLID,
    //            2,
    //            RGB(50, 50, 50));
    //
    //    HGDIOBJ oldBrush =
    //        SelectObject(hdc, bodyBrush);
    //
    //    HGDIOBJ oldPen =
    //        SelectObject(hdc, outlinePen);
    //
    //    //
    //    // 脚
    //    //
    //    const int legCount =
    //        GetLegCount(genome.legs);
    //
    //    if (legCount > 0)
    //    {
    //        const float spacing =
    //            bodyWidth / (legCount + 1);
    //
    //        for (int i = 0; i < legCount; ++i)
    //        {
    //            const int legX =
    //                static_cast<int>(
    //                    left + spacing * (i + 1));
    //
    //            const int legTop =
    //                bottom - 3;
    //
    //            const int legBottom =
    //                bottom + 8;
    //
    //            MoveToEx(
    //                hdc,
    //                legX,
    //                legTop,
    //                nullptr);
    //
    //            LineTo(
    //                hdc,
    //                legX,
    //                legBottom);
    //        }
    //    }
    //
    //    //
    //    // Body
    //    //
    //    switch (genome.body)
    //    {
    //    case Body::Circle:
    //    case Body::Oval:
    //        Ellipse(
    //            hdc,
    //            left,
    //            top,
    //            right,
    //            bottom);
    //        break;
    //
    //    case Body::Blob:
    //    {
    //        POINT points[]
    //        {
    //            { left + 4, top + bodyHeight / 2 },
    //            { left + 8, top + 5 },
    //            { static_cast<LONG>(drawCenterX), top },
    //            { right - 6, top + 7 },
    //            { right, static_cast<LONG>(drawCenterY) },
    //            { right - 5, bottom - 3 },
    //            { left + 9, bottom },
    //            { left, static_cast<LONG>(drawCenterY) }
    //        };
    //
    //        Polygon(
    //            hdc,
    //            points,
    //            static_cast<int>(std::size(points)));
    //
    //        break;
    //    }
    //    }
    //
    //    //
    //    // Eyes
    //    //
    //    HBRUSH eyeBrush =
    //        CreateSolidBrush(
    //            RGB(255, 255, 255));
    //
    //    HBRUSH pupilBrush =
    //        CreateSolidBrush(
    //            RGB(30, 30, 30));
    //
    //    SelectObject(
    //        hdc,
    //        eyeBrush);
    //
    //    int eyeCount = 1;
    //
    //    switch (genome.eyes)
    //    {
    //    case Eyes::Mono:
    //        eyeCount = 1;
    //        break;
    //
    //    case Eyes::Dual:
    //        eyeCount = 2;
    //        break;
    //
    //    case Eyes::Tri:
    //        eyeCount = 3;
    //        break;
    //    }
    //
    //    const float eyeSpacing = 11.0f;
    //
    //    for (int i = 0; i < eyeCount; ++i)
    //    {
    //        const float relative =
    //            static_cast<float>(i)
    //            - static_cast<float>(eyeCount - 1) * 0.5f;
    //
    //        const int eyeCenterX =
    //            static_cast<int>(
    //                drawCenterX
    //                + relative * eyeSpacing);
    //
    //        const int eyeCenterY =
    //            static_cast<int>(
    //                drawCenterY
    //                - bodyHeight * 0.12f);
    //
    //        //
    //        // 瞬き
    //        //
    //        if (pose.blink)
    //        {
    //            MoveToEx(
    //                hdc,
    //                eyeCenterX - 4,
    //                eyeCenterY,
    //                nullptr);
    //
    //            LineTo(
    //                hdc,
    //                eyeCenterX + 4,
    //                eyeCenterY);
    //
    //            continue;
    //        }
    //
    //        constexpr int EYE_W = 9;
    //        constexpr int EYE_H = 11;
    //
    //        SelectObject(
    //            hdc,
    //            eyeBrush);
    //
    //        Ellipse(
    //            hdc,
    //            eyeCenterX - EYE_W / 2,
    //            eyeCenterY - EYE_H / 2,
    //            eyeCenterX + EYE_W / 2,
    //            eyeCenterY + EYE_H / 2);
    //
    //        //
    //        // 黒目
    //        //
    //        SelectObject(
    //            hdc,
    //            pupilBrush);
    //
    //        const int pupilX =
    //            static_cast<int>(
    //                eyeCenterX
    //                + pose.eyeOffsetX);
    //
    //        const int pupilY =
    //            static_cast<int>(
    //                eyeCenterY
    //                + pose.eyeOffsetY);
    //
    //        constexpr int PUPIL = 4;
    //
    //        Ellipse(
    //            hdc,
    //            pupilX - PUPIL / 2,
    //            pupilY - PUPIL / 2,
    //            pupilX + PUPIL / 2,
    //            pupilY + PUPIL / 2);
    //    }
    //
    //    SelectObject(hdc, oldBrush);
    //    SelectObject(hdc, oldPen);
    //
    //    DeleteObject(bodyBrush);
    //    DeleteObject(eyeBrush);
    //    DeleteObject(pupilBrush);
    //    DeleteObject(outlinePen);
}
