#pragma once
#include <cstdint>

#include "../../Math/OctaveNoise.h"
#include "../../Math/PerlinNoise.h"
#include "../Types/Chunk.h"

class MapGenerator final {
public:
    MapGenerator(uint32_t seed);

    Chunk* GenChunk(int32_t x, int32_t z);

private:
    void genBasicTerrain(Chunk* chunk, int32_t x, int32_t z);
    void genDensityMap(int32_t xOffset, int32_t yOffset, int32_t zOffset);

    OctaveNoise<PerlinNoise> depthNoise;
    OctaveNoise<PerlinNoise> mainNoise;
    OctaveNoise<PerlinNoise> maxNoise;
    OctaveNoise<PerlinNoise> minNoise;
    OctaveNoise<PerlinNoise> surfaceNoise;

    std::default_random_engine engine;
    std::uniform_int_distribution<uint32_t> rand;
    uint32_t seed;
};
