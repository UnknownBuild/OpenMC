#include <cstdint>
#include <iostream>

#include "MapGenerator.h"

MapGenerator::MapGenerator(uint32_t seed) :
    seed(seed),
    engine(seed),
    depthNoise1(PerlinNoise(rand(engine)), 4, 0.5f),
    depthNoise2(PerlinNoise(rand(engine)), 2, 0.9f),
    thicknessNoise(PerlinNoise(rand(engine)), 8, 0.5f),
    grassNoise(PerlinNoise(rand(engine)), 4, 0.8f),
    flowerNoise(PerlinNoise(rand(engine)), 4, 0.8f),
    treeNoise(PerlinNoise(rand(engine)), 6, 0.5f) {
}

Chunk* MapGenerator::GenChunk(int32_t x, int32_t z) {
    Chunk* chunk = new Chunk();
    chunk->SetXZ(x, z);
    genBasicTerrain(chunk, x, z);
    return chunk;
}

void MapGenerator::genBasicTerrain(Chunk* chunk, int32_t x, int32_t z) {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            // 生成高度图
            float noise1 = depthNoise1.Get((x + i + 0.1f) * 0.01f, (z + j + 0.1f) * 0.01f);
            float noise2 = depthNoise2.Get((x + i + 0.1f) * -0.01f, (z + j + 0.1f) * -0.01f);
            int height = static_cast<int>(noise1 * (noise2 * 32 + 16));
            if (height < 16) {
                height = (height + 16) / 2;
            }
            // 生成厚度图
            float noise3 = thicknessNoise.Get((x + i + 0.1f) * 0.01f, (z + j + 0.1f) * 0.01f);
            int thickness = static_cast<int>(noise3 * 4 + 2);
            for (int k = 0; k < height; k++) {
                if (k < height - thickness) {
                    chunk->SetBlock(x + i, k, z + j, BlockId::Stone);
                } else if (k < height - 1) {
                    chunk->SetBlock(x + i, k, z + j, BlockId::Dirt);
                } else {
                    chunk->SetBlock(x + i, k, z + j, BlockId::GrassBlock);
                }
            }
            // 生成草、花
            if (grassNoise.Get((x + i) * 1.0f, (z + j) * 1.0f) > 0.7) {
                chunk->SetBlock(x + i, height, z + j, BlockId::Grass);
            }
            if (grassNoise.Get((x + i) * 0.1f, (z + j) * 0.1f) > 0.75) {
                chunk->SetBlock(x + i, height, z + j, BlockId::BlueOrchid);
            }
            // 生成树
            if (i >= 2 && i <= 13 && j >= 2 && j <= 13) {
                if (treeNoise.Get(x + i, z + j) > 0.8) {
                    for (int k = 0; k <= 5; k++) {
                        chunk->SetBlock(x + i, height + k, z + j, BlockId::OakLog);
                    }
                    for (int k = 4, step = 2; k <= 6; k++) {
                        if (k == 6) {
                            step = 1;
                        }
                        for (int x0 = i - step; x0 <= i + step; x0++) {
                            for (int z0 = j - step; z0 <= j + step; z0++) {
                                chunk->SetBlock(x + x0, height + k, z + z0, BlockId::OakLeaves);
                            }
                        }
                    }
                }
            }
        }
    }
}
