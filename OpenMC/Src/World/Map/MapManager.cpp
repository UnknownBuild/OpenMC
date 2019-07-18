#include <direct.h>
#include <fstream>
#include <io.h>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <iostream>

#include "../../Helpers/EnvPath.h"
#include "MapManager.h"

#ifdef _MSC_VER
#undef GetObject
#endif

bool MapManager::Init(std::string name, uint32_t seed) {
    // 判断存档文件夹是否存在，若不存在则创建
    if (_access(EnvPath::SaveDir.c_str(), 00) == -1) {
        if (_mkdir(EnvPath::SaveDir.c_str()) == -1) {
            return false;
        }
    }
    // 判断该存档是否存在
    if (_access((EnvPath::SaveDir + name).c_str(), 00) == 0) {
        return false;
    }
    // 创建存档
    if (_mkdir((EnvPath::SaveDir + name).c_str()) == -1) {
        return false;
    }
    rapidjson::Document d;
    rapidjson::Value vName, vSeed;
    d.SetObject();
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
    vName.SetString(name.c_str(), allocator);
    vSeed.SetUint(seed);
    d.AddMember("name", vName, allocator);
    d.AddMember("seed", vSeed, allocator);

    std::ofstream ofs(EnvPath::SaveDir + name + "/" + EnvPath::SaveConfigFile, std::ios::out);
    rapidjson::OStreamWrapper osw(ofs);
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
    d.Accept(writer);
    ofs.close();

    this->savePath = EnvPath::SaveDir + name + "/";
    this->mapSeed = seed;
    if (this->gen) delete this->gen;
    this->gen = new MapGenerator(this->mapSeed);
    chunks.clear();

    for (int x = -16; x < 16; x++) {
        for (int z = -16; z < 16; z++) {
            this->gen->GenChunk(x * 16, z * 16);
        }
    }
    // TODO: Generate map

    return true;
}

bool MapManager::Load(std::string name) {
    std::ifstream ifs(EnvPath::SaveDir + name + "/" + EnvPath::SaveConfigFile);
    if (!ifs.is_open()) {
        return false;
    }
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document d;
    d.ParseStream(isw);
    if (d.HasParseError()) {
        return false;
    } else if (!d.IsObject()) {
        return false;
    }
    for (auto& member : d.GetObject()) {
        if (member.name == "name") {
            if (!member.value.IsString()) return false;
            if (name != member.value.GetString()) return false;
        } else if (member.name == "seed") {
            if (!member.value.IsInt()) return false;
            this->mapSeed = member.value.GetInt();
        }
    }
    this->savePath = EnvPath::SaveDir + name + "/";
    if (this->gen) delete this->gen;
    this->gen = new MapGenerator(this->mapSeed);
    chunks.clear();
    return true;
}

std::vector<Chunk*> MapManager::GetActiveChunks(int32_t x, int32_t z, int vision) {
    std::vector<Chunk*> v;
    x = x / 16;
    z = z / 16;
    if (chunks.size() > 20) {
        unloadChunk(x, z, vision);
    }
    for (int32_t x0 = x - vision + 1; x0 < x + vision; x0++) {
        for (int32_t z0 = z - vision + 1; z0 < z + vision; z0++) {
            if (chunks.count({ x0, z0 })) {
                Chunk* chunk = chunks[{ x0, z0 }];
                if (chunk->IsUpdate()) {
                    v.push_back(chunk);
                }
            } else {
                Chunk* chunk = loadChunk(x0, z0);
                chunks[{ x0, z0 }] = chunk;
                v.push_back(chunk);
            }
        }
    }
    return v;
}

Chunk* MapManager::loadChunk(int32_t x, int32_t z) {
    return gen->GenChunk(x * 16, z * 16);
}

void MapManager::unloadChunk(int32_t x, int32_t z, int vision) {
    std::cout << "start unload chunks" << std::endl;
    std::map<std::pair<int, int>, Chunk*>::iterator iter;
    int count = 0;
    for (iter = chunks.begin(); iter != chunks.end(); ) {
        int cx = iter->first.first;
        int cz = iter->first.second;
        std::map<std::pair<int, int>, Chunk*>::iterator tempIter = iter;
        iter++;
        if (!(cx >= x - vision + 1 && cx < x + vision &&
            cz >= z - vision + 1 && cz < z + vision)) {
            count++;
            chunks.erase(tempIter);
        }
    }
    std::cout << "unload chunks: " << count << std::endl;
}
