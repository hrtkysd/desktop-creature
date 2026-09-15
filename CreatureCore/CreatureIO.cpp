#include "pch.h"
#include "Appearance.h"
#include "Creature.h"
#include "CreatureIO.h"
#include "Part.h"
#include "Skeleton.h"
#include "Transform2D.h"

#include <fstream>
#include <nlohmann/json.hpp>

using namespace Creature;
using namespace Creature::IO;
using namespace Creature::Math;

using json = nlohmann::json;

bool CCreatureIO::SaveAsFile(const CCreature& creature, const std::filesystem::path& path)
{
    json root;

    const auto& skeleton = creature.GetSkeleton();
    const auto& appearance = creature.GetAppearance();

    root["version"] = 1;
    root["parts"] = json::array();

    for (const auto& part : skeleton.Parts())
    {
        json partJson;

        partJson["id"] = part.id;
        partJson["name"] = part.strName;

        if (part.parentId == INVALID_PART_ID) partJson["parentId"] = nullptr;
        else partJson["parentId"] = part.parentId;

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

        root["parts"].push_back(std::move(partJson));
    }

    std::ofstream ofs(path);

    if (!ofs) return false;

    ofs << root.dump(4);

    return ofs.good();
}

bool CCreatureIO::LoadFromFile(const std::filesystem::path& path, CCreature& creature)
{
    std::ifstream ifs(path);

    if (!ifs) return false;

    json root;

    try
    {
        ifs >> root;
    }
    catch (...)
    {
        return false;
    }

    CCreature loadedCreature;

    for (const auto& partJson : root["parts"])
    {
        Part part;

        part.id = partJson["id"].get<PartId>();

        part.strName = partJson["name"].get<std::string>();

        if (partJson["parentId"].is_null())  part.parentId = INVALID_PART_ID;
        else part.parentId = partJson["parentId"].get<PartId>();

        const auto& transform = partJson["transform"];
        const auto& position = transform["position"];
        const auto& scale = transform["scale"];

        part.bindTransform =
            Math::CTransform2D(
                {
                    position[0].get<float>(),
                    position[1].get<float>()
                },
                {
                    scale[0].get<float>(),
                    scale[1].get<float>()
                },
                transform["rotation"].get<float>());

        const auto partId = part.id;
        loadedCreature.GetSkeleton().AddPartWithId(std::move(part));

        if (partJson.contains("texture"))
        {
            loadedCreature.GetAppearance().SetTexture(
                partId,
                partJson["texture"]
                .get<std::string>());
        }
    }

    creature = std::move(loadedCreature);

    return true;
}
