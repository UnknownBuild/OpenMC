﻿#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "../../ResourceManager/Texture.h"

enum class BlockId : uint8_t {
    Air = 0,
    Stone = 1,
    GrassBlock = 2,
    Dirt = 3,
    CobbleStone = 4,
    Water = 5,
    Grass = 6,
    CrackedStoneBricks = 7,
    BrownMushroom = 8,
    BlueOrchid = 9,
    HornCoral = 10,
    Dandelion = 11,
    SeaLantern = 12,
    Sand = 13,
    OakPlanks = 14,
    Glass = 15,
    Torch = 16,
    CraftingTable = 17,
    Furnace = 18,
    OakLog = 19,
    Cactus = 20,
    OakLeaves = 21,
    IronDoor = 22,
    OakDoor = 23,
    BlueStainedGlassPane = 24,
    Fire = 51,
    Beetroot = 26,
    Select = 255,
    Pumpkin = 27,
    Melon = 28,
    Quartz = 29,
};

enum class BlockType : uint8_t {
    None = 0,           // 无实体
    Liquid = 1,         // 液体
    Solid = 2,          // 固体
    Face = 3,           // 单面
    TransSolid = 4,     // 透明固体
    TransFace = 5,      // 透明单面
};

enum class RenderType : uint8_t {
    None = 0,              // 不渲染
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
    OneTexture = 12,         // 单纹理方块
    OffsetTexture = 13,
    OffsetSideTexture = 14,
    Select = 15,
};

class BlockData final {
public:
    BlockId Id;
    std::string Name;
    BlockType Type;
    RenderType Render;                  // 渲染类型
    std::vector<Texture2D> Textures;    // 材质
    std::vector<glm::vec4> Colors;      // 颜色
    uint8_t Light;                      // 自身亮度
    uint8_t Animation;                  // 动画速度 (0-5)
};
