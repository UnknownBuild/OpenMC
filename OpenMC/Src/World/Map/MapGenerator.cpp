#include <cstdint>

#include "MapGenerator.h"

MapGenerator::MapGenerator(int32_t seed) : noise(seed) {
    this->seed = seed;
}

Chunk* MapGenerator::GenChunk(int x, int z) {
    auto noise1 = [this](double x, double z) -> double {
        return this->noise.Noise(x, 0, z);
    };
    auto noise2 = [this](double x, double z, int32_t octaves) -> double {
        return this->noise.OctaveNoise(x, 0, z, octaves);
    };
    auto noise3 = [=](double x, double z, int32_t octaves) -> double {
        return noise1(x + noise2(x, z, octaves), z);
    };
    Chunk* chunk = new Chunk();
    x = x / 16 * 16;
    z = z / 16 * 16;

    // 创建高度图
    uint8_t waterLevel = 64;
    uint8_t heightMap[16][16];
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            const double heightLow = noise1((x + i) * 1.3, (z + j) * 1.3) / 6 - 4;
            const double heightHigh = noise2((x + i) * 1.3, (z + j) * 1.3, 6) / 5 + 6;
            double heightRes = (noise3((x + i), (z + j), 6) / 8 > 0 ? heightLow : (heightLow + heightHigh) / 2);
            heightRes /= 2;
            if (heightRes < 0) {
                heightRes *= 0.8;
            }
            heightMap[i][j] = static_cast<uint8_t>(heightRes);
        }
    }

    // 填充方块
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            const int dirtThickness = static_cast<int>(noise2(x + i, z + j, 8) / 24 - 4);
            const int dirtTransition = heightMap[x + i][z + j];
            const int stoneTransition = dirtTransition + dirtThickness;
            for (int y = 0; y <= dirtTransition; y++) {
                if (y == 0) {
                    chunk->SetBlock(x + i, y, z + j, BlockId::Bedrock);
                } else if (y <= stoneTransition) {
                    chunk->SetBlock(x + i, y, z + j, BlockId::Stone);
                } else if (y <= dirtTransition) {
                    chunk->SetBlock(x + i, y, z + j, BlockId::Dirt);
                }
            }
        }
    }

    return chunk;
}
