#pragma once
#include <vector>

#include "../Types/Region.h"

class MapManager final {
public:
    bool Init(std::string name, uint32_t seed);
    void Load(std::string name);
    std::vector<RenderChunk> GetRenderChunks(int x, int z);

private:
    std::string savePath;
    uint32_t mapSeed;
    uint8_t vision;
};
