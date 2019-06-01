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
    densityMap(5, std::vector<std::vector<float>>(33, std::vector<float>(5))) {
}

Chunk* MapGenerator::GenChunk(int32_t x, int32_t z) {
    Chunk* chunk = new Chunk();
    chunk->Set(x, z);
    genBasicTerrain(chunk, x, z);
    return chunk;
}

void MapGenerator::genBasicTerrain(Chunk* chunk, int32_t x, int32_t z) {
    genDensityMap(x, 0, z);
    for (int xHigh = 0; xHigh < 4; xHigh++) {
        for (int zHigh = 0; zHigh < 4; zHigh++) {
            for (int yHigh = 0; yHigh < 32; yHigh++) {
                double yPart111 = densityMap[xHigh][yHigh][zHigh];
                double yPart121 = densityMap[xHigh][yHigh][zHigh + 1];
                double yPart211 = densityMap[xHigh + 1][yHigh][zHigh];
                double yPart221 = densityMap[xHigh + 1][yHigh][zHigh + 1];
                double yDensityStep11 = (densityMap[xHigh][yHigh + 1][zHigh] - yPart111) * 0.125;
                double yDensityStep12 = (densityMap[xHigh][yHigh + 1][zHigh + 1] - yPart121) * 0.125;
                double yDensityStep21 = (densityMap[xHigh + 1][yHigh + 1][zHigh] - yPart211) * 0.125;
                double yDensityStep22 = (densityMap[xHigh + 1][yHigh + 1][zHigh + 1] - yPart221) * 0.125;
                for (int yLow = 0; yLow < 8; yLow++) {
                    double density111 = yPart111;
                    double density121 = yPart121;
                    double xDensityStep11 = (yPart211 - yPart111) * 0.25;
                    double xDensityStep21 = (yPart221 - yPart121) * 0.25;
                    for (int xLow = 0; xLow < 4; xLow++) {
                        double zDensityStep11 = (density121 - density111) * 0.25;
                        double blockValue = density111 - zDensityStep11;
                        for (int zLow = 0; zLow < 4; zLow++) {
                            int posX = xHigh * 4 + xLow;
                            int posY = yHigh * 8 + yLow;
                            int posZ = zHigh * 4 + zLow;
                            if (posY == 0) {
                                chunk->SetBlock(x + posX, posY, z + posZ, BlockId::Bedrock);
                            } else if ((blockValue += zDensityStep11) > 0.0) {
                                chunk->SetBlock(x + posX, posY, z + posZ, BlockId::Stone);
                            } else if (posY < 64) {
                                // chunk->SetBlock(x + posX, posY, z + posZ, BlockId::Water);
                            } else {
                                chunk->SetBlock(x + posX, posY, z + posZ, BlockId::Air);
                            }
                        }
                        density111 += xDensityStep11;
                        density121 += xDensityStep21;
                    }
                    yPart111 += yDensityStep11;
                    yPart121 += yDensityStep12;
                    yPart211 += yDensityStep21;
                    yPart221 += yDensityStep22;
                }
            }
        }
    }
}

void MapGenerator::genDensityMap(int32_t xOffset, int32_t yOffset, int32_t zOffset) {
    depthNoise.Get(densityMap, glm::vec3(xOffset + 0.1f, 0.0f, zOffset + 0.1f), glm::vec3(1.0f));
}
