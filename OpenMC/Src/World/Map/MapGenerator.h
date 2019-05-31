#pragma once
#include <cstdint>

#include "../../Math/PerlinNoise.h"
#include "../Types/Chunk.h"

class MapGenerator final {
public:
    MapGenerator(int32_t seed);

    Chunk* GenChunk(int x, int z);

private:
    PerlinNoise noise;
    int32_t seed;
};
