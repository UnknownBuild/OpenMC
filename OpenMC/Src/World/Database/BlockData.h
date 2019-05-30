#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "../../ResourceManager/Texture.h"

enum class BlockId : uint8_t {
    Air = 0,
    Stone = 1,
    Grass = 2,
    Dirt = 3,
    CobbleStone = 4,
};

enum class BlockType : uint8_t {
    None = 0,   // 无实体
    Liquid = 1, // 液体
    Soild = 2,  // 固体
    Face = 3,   // 单面
};

enum class RenderType : uint8_t {
    OneTexture = 0,         // 单纹理方块
    ThreeTexture = 1,       // 三纹理方块(上、四周、下)
    NoiseTexture = 2,       // 噪声 + 颜色
    SideTexture = 3,        // 上下噪声，四周纹理
    CenterCrossTexture = 4, // 中心交叉纹理
    TorchTexture = 5,       // 火把
    FireTexture = 6,        // 火焰
    CustomTexture = 7,      // 六面自定义方块 (前后左右上下)
    DirCustomTexture = 8,   // 具有方向的方块 （前、侧边、上下）
    DoorTexture = 9,        // 门
    GlassTexture = 10,      // 玻璃
    LiquidTexture = 11,     // 液体表面
    None = 12,              // 不渲染
};

class BlockData final {
public:
    BlockId Id;
    std::string Name;
    RenderType Type;                    // 渲染类型
    std::vector<std::string> Textures;  // 材质
    std::vector<glm::vec4> Colors;      // 颜色
    uint8_t light;                      // 自身亮度
};
