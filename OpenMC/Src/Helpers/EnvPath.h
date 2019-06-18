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

    // Save
    const std::string SaveDir = "Saves/";
    const std::string SaveConfigFile = "config.json";
}
