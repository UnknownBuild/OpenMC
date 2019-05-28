#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "../../ResourceManager/Texture.h"

enum class BlockId : uint8_t {
    Air = 0,
    Stone = 1,
    Grass = 2,
    Dirt = 3,
    CobbleStone = 4,
};

enum class BlockType : uint8_t {
    None = 0,
    OneTexture = 1,
    ThreeTexture = 2,
    NoiseTexture = 3,
};

class BlockData final {
public:
    BlockId Id;
    std::string Name;
    BlockType Type;
};
