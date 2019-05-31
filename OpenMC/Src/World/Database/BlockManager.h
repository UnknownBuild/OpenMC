#pragma once
#include "BlockData.h"
#include <vector>
#include "../../ResourceManager/ResourceManager.h"
#include "../../Systems/Window.h"
#include "../../Helpers/Singleton.h"

class BlockManager final {
public:
    bool Load();
    BlockData GetBlockData(BlockId id);

private:

    BlockData blocks[256];
};
