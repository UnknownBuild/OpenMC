#pragma once
#include "RenderChunk.h"

// 区块的大小为 (16, 256, 16)
// 区块的范围为 [x, :, z] -> [x+15, :, z+15]
class Chunk final {
public:
    void Set(int32_t x, int32_t z) {
        // TODO
        if (true) {
            throw "CoordinateError";
        }
    }

    void SetBlock(int x, uint8_t y, int z, BlockId id, BlockMeta meta = BlockMeta::None) {
        renderChunks[y / 16].SetBlock(x, y, z, id, meta);
    }

private:
    int32_t x;
    int32_t z;
    RenderChunk renderChunks[16];
};
