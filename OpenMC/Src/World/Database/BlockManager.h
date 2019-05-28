#pragma once
#include "BlockData.h"

class BlockManager final {
public:
    bool Load();
    BlockData GetBlockData(BlockId id);

private:

    BlockData blocks[256];
};
