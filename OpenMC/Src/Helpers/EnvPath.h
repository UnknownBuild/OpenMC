#pragma once
#include <string>

namespace EnvPath {
    const std::string ResourceDir = "Resources/";
    const std::string TextureDir = ResourceDir + "Textures/";

    const std::string BlockInfoFile = ResourceDir + "Assets/blocks.json";
    const std::string ConfigFile = "OpenMC.json";

    // Resource
    const std::string GameTitleImage = TextureDir + "gui/minecraft_title.png";
    const std::string FocusImage = TextureDir + "gui/focus.png";

    const std::string GrassBlockImage = TextureDir + "gui/Grass_Block.png";
    const std::string SandImage = TextureDir + "gui/Sand.png";
    const std::string OakPlanksImage = TextureDir + "gui/Oak_Planks.png";
    const std::string OakLogImage = TextureDir + "gui/Oak_Log.png";
    const std::string GlassImage = TextureDir + "gui/Glass.png";
    const std::string SpruceLeavesImage = TextureDir + "gui/Spruce_Leaves.png";
    const std::string CraftingTableImage = TextureDir + "gui/Crafting_Table_TextureUpdate.png";
    const std::string LitFurnaceImage = TextureDir + "gui/Lit_Furnace.png";
    const std::string MelonImage = TextureDir + "gui/Melon.png";
    const std::string PumpkinImage = TextureDir + "gui/Pumpkin.png";
    const std::string StoneImage = TextureDir + "gui/Stone.png";
    const std::string CobblestoneImage = TextureDir + "gui/Cobblestone.png";
    const std::string QuartzImage = TextureDir + "gui/Block_of_Quartz.png";

    // Save
    const std::string SaveDir = "Saves/";
    const std::string SaveConfigFile = "config.json";
}
