#include <direct.h>
#include <fstream>
#include <io.h>
#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

#include "../../Helpers/EnvPath.h"
#include "MapManager.h"

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
    this->gen = new MapGenerator(this->mapSeed);

    for (int x = -16; x < 16; x++) {
        for (int z = -16; z < 16; z++) {
            this->gen->GenChunk(x * 16, z * 16);
        }
    }
    // TODO: Generate map

    return true;
}

std::vector<Chunk*> MapManager::GetChunks(int32_t x, int32_t z, int vision) {
    std::vector<Chunk*> v;
    x = x / 16;
    z = z / 16;
    for (int32_t i = -vision; i <= vision; i++) {
        for (int32_t j = -vision; j <= vision; j++) {
            if (chunks.count(x + i) && chunks[x + i].count(z + j)) {
                v.push_back(chunks[x + i][z + j]);
            } else {
                Chunk* chunk = loadChunk(x, z);
                chunks[x][z] = chunk;
                v.push_back(chunk);
            }
        }
    }
    return v;
}

Chunk* MapManager::loadChunk(int32_t x, int32_t z) {
    return gen->GenChunk(x, z);
}
