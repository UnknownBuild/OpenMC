#pragma once
#include <map>
#include <vector>

#include "../Types/Region.h"
#include "MapGenerator.h"

class MapManager final {
public:
    bool Init(std::string name, uint32_t seed);
    void Load(std::string name);
    std::vector<Chunk*> GetChunks(int32_t x, int32_t z, int vision);

private:
    Chunk* loadChunk(int32_t x, int32_t z);

    std::map<int, std::map<int, Chunk*>> chunks;

    MapGenerator* gen;
    std::string savePath;
    uint32_t mapSeed;
};
