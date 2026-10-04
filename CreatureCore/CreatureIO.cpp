#include "pch.h"
#include "Animation.h"
#include "AnimationEditor.h"
#include "AnimationProperty.h"
#include "AnimationTrack.h"
#include "AnimationTrackKey.h"
#include "Appearance.h"
#include "AppearanceEditor.h"
#include "Creature.h"
#include "CreatureEditor.h"
#include "CreatureIO.h"
#include "Motion.h"
#include "MotionEditor.h"
#include "Part.h"
#include "Skeleton.h"
#include "SkeletonEditor.h"
#include "Transform2D.h"

#include <fstream>
#include <nlohmann/json.hpp>

using namespace Creature;
using namespace Creature::Animation;
using namespace Creature::Editor;
using namespace Creature::IO;
using namespace Creature::Math;

using json = nlohmann::json;

namespace
{
    json SerializePart(
        const Part& part,
        const CAppearance& appearance)
    {
        json partJson;

        partJson["id"] = part.id;
        partJson["name"] = part.strName;

        if (part.parentId == INVALID_PART_ID)
        {
            partJson["parentId"] = nullptr;
        }
        else
        {
            partJson["parentId"] = part.parentId;
        }

        const auto& position = part.bindTransform.GetPosition();
        const auto& scale = part.bindTransform.GetScale();

        partJson["transform"] =
        {
            {
                "position",
                { position.x, position.y }
            },
            {
                "scale",
                { scale.x, scale.y }
            },
            {
                "rotation",
                part.bindTransform.GetRotation()
            }
        };

        if (const auto partAppearance = appearance.FindByPartId(part.id))
        {
            partJson["texture"] = partAppearance->texturePath.generic_string();
        }

        return partJson;
    }

    json SerializeAnimation(const CAnimation& animation)
    {
        json animationJson;

        animationJson["duration"] = animation.GetDuration();
        animationJson["tracks"] = json::array();

        for (const auto& track : animation.GetAnimationTracks())
        {
            const auto& key = track.GetKey();

            if (!key.IsValid()) continue;

            json trackJson;

            trackJson["partId"] = key.GetPartId();
            trackJson["property"] = static_cast<int>(key.GetProperty());
            trackJson["keyFrames"] = json::array();

            for (const auto& keyFrame : track.GetKeyFrames())
            {
                json keyFrameJson;

                keyFrameJson["time"] = keyFrame.fTime;
                keyFrameJson["value"] = keyFrame.fValue;
                keyFrameJson["interpolation"] = static_cast<int>(keyFrame.eInterpolationToNext);

                trackJson["keyFrames"].push_back(std::move(keyFrameJson));
            }

            animationJson["tracks"].push_back(std::move(trackJson));
        }

        return animationJson;
    }

    bool DeserializePart(
        const json& partJson,
        CSkeletonEditor& skeletonEditor,
        CAppearanceEditor& appearanceEditor)
    {
        Part part;
        part.id = partJson.at("id").get<PartId>();
        part.strName = partJson.at("name").get<std::string>();

        if (partJson.at("parentId").is_null())
        {
            part.parentId = INVALID_PART_ID;
        }
        else
        {
            part.parentId = partJson.at("parentId").get<PartId>();
        }

        const auto& transform = partJson.at("transform");
        const auto& position = transform.at("position");
        const auto& scale = transform.at("scale");

        part.bindTransform =
            CTransform2D(
                {
                    position.at(0).get<float>(),
                    position.at(1).get<float>()
                },
                {
                    scale.at(0).get<float>(),
                    scale.at(1).get<float>()
                },
                transform.at("rotation").get<float>());

        const auto partId = part.id;

        if (!skeletonEditor.AddPartWithId(std::move(part))) return false;

        if (partJson.contains("texture"))
        {
            appearanceEditor.SetTexture(partId, partJson.at("texture").get<std::string>());
        }

        return true;
    }

    bool DeserializeAnimation(
        const json& animationJson,
        CAnimationEditor& animationEditor)
    {
        if (!animationEditor.SetDuration(animationJson.at("duration").get<float>()))
        {
            return false;
        }

        for (const auto& trackJson : animationJson.at("tracks"))
        {
            const auto partId = trackJson.at("partId").get<PartId>();
            const auto property =
                static_cast<AnimationProperty>(
                    trackJson.at("property").get<int>());

            CAnimationTrack track
            {
                CAnimationTrackKey
                {
                    partId,
                    property
                }
            };

            for (const auto& keyFrameJson : trackJson.at("keyFrames"))
            {
                FloatKeyFrame keyFrame;

                keyFrame.fTime = keyFrameJson.at("time").get<float>();
                keyFrame.fValue = keyFrameJson.at("value").get<float>();
                keyFrame.eInterpolationToNext =
                    static_cast<Interpolation>(
                        keyFrameJson
                        .at("interpolation")
                        .get<int>());

                if (!track.AddOrUpdateKeyFrame(keyFrame)) return false;
            }

            if (!animationEditor.AddTrack(std::move(track))) return false;
        }

        return true;
    }
}

bool CCreatureIO::SaveAsFile(
    const CCreature& creature,
    const std::filesystem::path& path)
{
    json root;

    root["version"] = 1;
    root["motions"] = json::array();

    for (const auto& motion : creature.GetMotions())
    {
        const auto& skeleton = motion.GetSkeleton();
        const auto& appearance = motion.GetAppearance();
        const auto& animation = motion.GetAnimation();

        json motionJson;

        motionJson["id"] = motion.GetMotionId();
        motionJson["name"] = motion.GetName();
        motionJson["parts"] = json::array();

        for (const auto& part : skeleton.Parts())
        {
            motionJson["parts"].push_back(
                SerializePart(
                    part,
                    appearance));
        }

        motionJson["animation"] = SerializeAnimation(animation);

        root["motions"].push_back(std::move(motionJson));
    }

    std::ofstream ofs(path);

    if (!ofs) return false;

    ofs << root.dump(4);

    return ofs.good();
}

bool CCreatureIO::LoadFromFile(
    const std::filesystem::path& path,
    CCreature& creature)
{
    std::ifstream ifs(path);

    if (!ifs) return false;

    json root;

    try
    {
        ifs >> root;

        if (!root.contains("version") || root.at("version").get<int>() != 1) return false;
        if (!root.contains("motions") || !root.at("motions").is_array()) return false;

        CCreature loadedCreature;
        CCreatureEditor creatureEditor{ loadedCreature };

        for (const auto& motionJson : root.at("motions"))
        {
            const auto motionId = motionJson.at("id").get<MotionId>();
            const auto strMotionName = motionJson.at("name").get<std::string>();

            const auto addedId = creatureEditor.AddMotionWithId(motionId, strMotionName);
            if (addedId == INVALID_MOTION_ID) return false;

            auto motionEditor = creatureEditor.MotionEditor(motionId);
            if (!motionEditor) return false;

            auto skeletonEditor = motionEditor->SkeletonEditor();
            auto appearanceEditor = motionEditor->AppearanceEditor();
            auto animationEditor = motionEditor->AnimationEditor();

            for (const auto& partJson : motionJson.at("parts"))
            {
                if (!DeserializePart(partJson, skeletonEditor, appearanceEditor)) return false;
            }

            if (!DeserializeAnimation(motionJson.at("animation"), animationEditor)) return false;
        }

        creature = std::move(loadedCreature);
    }
    catch (const json::exception&)
    {
        return false;
    }
    catch (const std::exception&)
    {
        return false;
    }

    return true;
}
