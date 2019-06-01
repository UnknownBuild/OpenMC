#pragma once
#include "RenderChunk.h"

// 区块的大小为 (16, 256, 16)
// 区块的范围为 [x, :, z] -> [x+15, :, z+15]
class Chunk final {
public:
    Block GetBlock(int32_t x, uint8_t y, int32_t z) {
        return renderChunks[y / 16].GetBlock(x, y, z);
    }

    void Set(int32_t x, int32_t z) {
        if (x % 16 || z % 16) {
            throw "CoordinateError";
        }
        this->x = x;
        this->z = z;
        for (int i = 0; i < 16; i++) {
            renderChunks[i].Set(x, i * 16, z);
        }
    }

    void SetBlock(int32_t x, uint8_t y, int32_t z, BlockId id, BlockMeta meta = BlockMeta::None) {
        renderChunks[y / 16].SetBlock(x, y, z, id, meta);
    }

private:
    int32_t x;
    int32_t z;
    RenderChunk renderChunks[16];
};
