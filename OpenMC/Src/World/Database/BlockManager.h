#pragma once
#include "BlockData.h"
#include <vector>
#include "../../ResourceManager/ResourceManager.h"

class BlockManager final {
public:
    bool Load();
    BlockData GetBlockData(BlockId id);

private:

    BlockData blocks[256];
};
