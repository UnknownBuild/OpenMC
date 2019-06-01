#pragma once
#include "../Database/BlockData.h"

enum class BlockMeta : uint8_t {
    None = 0
};

class Block final {
public:
    BlockId GetId() {
        return id;
    }

    void Set(BlockId id, BlockMeta meta = BlockMeta::None) {
        this->id = id;
        this->meta = meta;
    }

private:
    int32_t x;
    uint8_t y;
    int32_t z;
    BlockId id;
    BlockMeta meta;
};
