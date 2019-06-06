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
    surfaceNoise(PerlinNoise(rand(engine)), 8, 0.5f),
    heightMap(16, std::vector<std::vector<float>>(1, std::vector<float>(16))),
    densityMap(5, std::vector<std::vector<float>>(33, std::vector<float>(5))),
    depthMap(5, std::vector<std::vector<float>>(1, std::vector<float>(5))),
    mainNoiseMap(5, std::vector<std::vector<float>>(33, std::vector<float>(5))),
    maxLimitMap(5, std::vector<std::vector<float>>(33, std::vector<float>(5))),
    minLimitMap(5, std::vector<std::vector<float>>(33, std::vector<float>(5))) {
}

Chunk* MapGenerator::GenChunk(int32_t x, int32_t z) {
    Chunk* chunk = new Chunk();
    chunk->Set(x, z);
    genBasicTerrain(chunk, x, z);
    return chunk;
}

void MapGenerator::genBasicTerrain(Chunk* chunk, int32_t x, int32_t z) {
    srand(2019);
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            // heightMap[i][0][j] = simplex3((x + i + 0.1f) * 0.001f, 0.0f, (z + j + 0.1f) * 0.001f, 8, 0.5, 2);
            // std::cout << heightMap[i][0][j] << std::endl;
        }
    }
    depthNoise.Get(heightMap, glm::vec3((x + 0.1f) * 0.001f, 0.0f, (z + 0.1f) * 0.001f), glm::vec3(1.0f));
    for (int i = 0; i < 16; i++) {
        for (int k = 0; k < 16; k++) {
            int height = static_cast<int>((heightMap[i][0][k] + 0.5) * 62);
            // std::cout << height << std::endl;
            for (int j = 0; j < height; j++) {
                chunk->SetBlock(x + i, j, z + k, BlockId::Stone);
            }
        }
    }
}
