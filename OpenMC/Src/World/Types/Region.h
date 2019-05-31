#pragma once
#include "Chunk.h"

// 区域的大小为 (32, 32) 区块
// 区域的范围为 [x-16, z-16] -> [x+15, z+15] 区块
class Region final {
private:
    // x, z轴为区块编号，并非实际坐标
    uint32_t x;
    uint32_t z;
};
