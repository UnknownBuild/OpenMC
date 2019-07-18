#pragma once
#include <map>
#include <vector>

#include "../Types/Region.h"
#include "MapGenerator.h"

class MapManager final {
public:
    bool Init(std::string name, uint32_t seed);
    bool Load(std::string name);
    std::vector<Chunk*> GetActiveChunks(int32_t x, int32_t z, int vision);

private:
    Chunk* loadChunk(int32_t x, int32_t z);
    void unloadChunk(int32_t x, int32_t z, int vision);

    std::map<std::pair<int, int>, Chunk*> chunks;

    MapGenerator* gen = nullptr;
    std::string savePath;
    uint32_t mapSeed;
};
