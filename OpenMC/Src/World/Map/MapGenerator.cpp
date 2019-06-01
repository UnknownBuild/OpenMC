#include <cstdint>
#include <iostream>

#include "MapGenerator.h"

MapGenerator::MapGenerator(uint32_t seed) :
    seed(seed),
    engine(seed),
    depthNoise(PerlinNoise(rand(engine)), 8, 0.5f),
    mainNoise(PerlinNoise(rand(engine)), 8, 0.5f),
    maxNoise(PerlinNoise(rand(engine)), 8, 0.5f),
    minNoise(PerlinNoise(rand(engine)), 8, 0.5f),
    surfaceNoise(PerlinNoise(rand(engine)), 8, 0.5f) {
}

Chunk* MapGenerator::GenChunk(int32_t x, int32_t z) {
    PerlinNoise noise;
    std::cout << noise.Get(10000000000, 20000000000, 3000000000) << std::endl;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            // std::cout << depthNoise.Get(x + i, 0, z + j) << std::endl;
        }
    }
    return nullptr;
}

void MapGenerator::genBasicTerrain(Chunk* chunk, int32_t x, int32_t z) {

}
