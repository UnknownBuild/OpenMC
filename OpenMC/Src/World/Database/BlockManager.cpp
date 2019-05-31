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
    Window* window = Singleton<Window>::GetInstance();
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
                if (!member.value.IsString()) {
                    window->Dialog("Read Json Error", "In id" + std::to_string(id) + ": 'name' is not string");
                    return false;
                }
                blocks[id].Name = member.value.GetString();
            } else if (member.name == "type") {
                // blocks[id].Type = static_cast<BlockType>(member.value.GetInt());
                if (!member.value.IsNumber()) {
                    window->Dialog("Read Json Error", "In id" + std::to_string(id) + ": 'type' is not a number");
                    return false;
                }
                blocks[id].Type = static_cast<BlockType>(member.value.GetInt());
            }
            else if (member.name == "textures") {
                // blocks[id].Textures = member.value.GetArray()
                if (!member.value.IsArray()) {
                    window->Dialog("Read Json Error", "In id" + std::to_string(id) + ": 'textures' is not array");
                    return false;
                }
                // 预加载贴图
                for (auto& t : member.value.GetArray()) {
                    if (!t.IsString()) {
                        window->Dialog("Read Json Error", "In id" + std::to_string(id) + ": member in 'textures' is not string");
                        return false;
                    }
                    std::string texture = t.GetString();
                    if (texture[0] == '*') { // 需要分割的贴图
                        blocks[id].Textures = ResourceManager::LoadSplitTexture(EnvPath::TextureDir + "/blocks/"+ texture.substr(1), texture);
                        break;
                    } else {
                        blocks[id].Textures.push_back(ResourceManager::LoadTexture(EnvPath::TextureDir + "/blocks/" + texture, texture));
                    }
                }
            }
            else if (member.name == "render") {
                if (!member.value.IsNumber()) {
                    window->Dialog("Read Json Error", "In id" + std::to_string(id) + ": 'render' is not a number");
                    return false;
                }
                blocks[id].Render = static_cast<RenderType>(member.value.GetInt());
            }
            else if (member.name == "colors") {
                if (!member.value.IsArray()) {
                    window->Dialog("Read Json Error", "In id" + std::to_string(id) + ": 'colors' is not array");
                    return false;
                }
                for (auto& colorVec : member.value.GetArray()) {
                    if (!colorVec.IsArray()) {
                        window->Dialog("Read Json Error", "In id" + std::to_string(id) + ": member in 'colors' is not vec4");
                        return false;
                    }
                    if (colorVec.GetArray().Size() != 4) {
                        window->Dialog("Read Json Error", "In id" + std::to_string(id) + ": member in 'colors' is not vec4");
                        return false;
                    }
                    glm::vec4 c(colorVec.GetArray()[0].GetFloat(), colorVec.GetArray()[1].GetFloat(), colorVec.GetArray()[2].GetFloat(), colorVec.GetArray()[3].GetFloat());
                    blocks[id].Colors.push_back(c);
                }
            }
            else if (member.name == "light") {
                if (!member.value.IsNumber()) {
                    window->Dialog("Read Json Error", "In id" + std::to_string(id) + ": 'light' is not a number");
                    return false;
                }
                blocks[id].Light = member.value.GetInt();
            }
            else if (member.name == "animation") {
                if (!member.value.IsNumber()) {
                    window->Dialog("Read Json Error", "In id" + std::to_string(id) + ": 'animation' is not a number");
                    return false;
                }
                blocks[id].Animation = member.value.GetInt();
            }
        }
    }
    return true;
}

BlockData BlockManager::GetBlockData(BlockId id) {
    return blocks[static_cast<size_t>(id)];
}
