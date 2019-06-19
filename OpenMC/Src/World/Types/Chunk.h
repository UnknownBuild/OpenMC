#pragma once
#include "RenderChunk.h"

struct ChunkXZ {
    int32_t X;
    int32_t Z;
};

// 区块的大小为 (16, 256, 16)
// 区块的范围为 [x, :, z] -> [x+15, :, z+15]
class Chunk final {
public:
    Block& GetBlock(int32_t x, uint8_t y, int32_t z) {
        return blocks[x % 16][y][z % 16];
    }

    ChunkXZ GetXZ() {
        return ChunkXZ{x, z};
    }

    bool IsUpdate() {
        return isUpdate;
    }

    void SetBlock(int32_t x, uint8_t y, int32_t z, BlockId id, BlockMeta meta = BlockMeta::None) {
        blocks[(x%16+16)%16][y][(z%16+16)%16].Set(id, meta);
    }

    void SetXZ(int32_t x, int32_t z) {
        if (x % 16 || z % 16) {
            throw "CoordinateError";
        }
        this->x = x;
        this->z = z;
    }

private:
    int32_t x;
    int32_t z;
    bool isUpdate = false;
    Block blocks[16][256][16];
};
