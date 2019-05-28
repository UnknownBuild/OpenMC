#include <iostream>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include "../../Helpers/EnvPath.h"
#include "BlockManager.h"

#ifdef _MSC_VER
#undef GetObject
#endif

bool BlockManager::Load() {
    std::ifstream ifs(EnvPath::BlockInfoFile);
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document d;
    d.ParseStream(isw);
    if (d.HasParseError()) {
        std::cout << "Parse file '" + EnvPath::BlockInfoFile << "' failed." << std::endl;
        std::cout << "Error Code: " << d.GetParseError() << std::endl;
        return false;
    } else if (!d.IsArray()) {
        std::cout << "Block info must be array." << std::endl;
        return false;
    }
    for (auto& item : d.GetArray()) {
        if (!item.IsObject()) {
            return false;
        }
        auto idIt = item.FindMember("id");
        if (idIt == item.MemberEnd()) {
            return false;
        } else if (!idIt->value.IsNumber()) {
            return false;
        }
        int id = idIt->value.GetInt();
        if (id < 0 || id > 255) {
            return false;
        }
        for (auto& member : item.GetObject()) {
            if (member.name == "id") {
                blocks[id].Id = static_cast<BlockId>(id);
            } else if (member.name == "name") {
                blocks[id].Name = member.value.GetString();
            } else if (member.name == "type") {
                blocks[id].Type = static_cast<BlockType>(member.value.GetInt());
            }
        }
    }
    return true;
}

BlockData BlockManager::GetBlockData(BlockId id) {
    return blocks[static_cast<size_t>(id)];
}
