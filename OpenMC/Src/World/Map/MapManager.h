#pragma once
#include <vector>

#include "../Types/Region.h"

class MapManager final {
public:
    void Load();
    std::vector<RenderChunk> GetRenderChunks(int x, int z);

private:
    long long int mapSeed;
    uint8_t visionLength;
};
