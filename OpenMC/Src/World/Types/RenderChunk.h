#pragma once
#include "Block.h"

// 渲染区块的大小为 (16, 16, 16)
// 渲染区块的范围为 [x, y, z] -> [x+15, y+15, z+15]
class RenderChunk final {
public:
    void Set(int32_t x, uint8_t y, int32_t z) {
        // TODO
        if (true) {
            throw "CoordinateError";
        }
    }

    void SetBlock(int x, int y, int z, BlockId id, BlockMeta meta = BlockMeta::None) {
        x = x - this->x;
        y = y - this->y;
        z = z - this->z;
        if (x < 0 || x >= 16 || y < 0 || y >= 16 || z < 0 || z >= 16) {
            throw "OutOfBound";
        }
        blocks[x][y][z].Set(id, meta);
    }

private:
    int32_t x;
    uint8_t y;
    int32_t z;
    Block blocks[16][16][16];
};
