#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <sstream>

#include "Config.h"

bool Config::Load() {
    std::ifstream ifs(EnvPath::ConfigFile);
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document d;
    d.ParseStream(isw);
    if (d.HasParseError()) {
        return false;
    } else if (!d.IsObject()) {
        return false;
    }
    for (auto& member : d.GetObject()) {
        if (member.name == "window_full_screen") {
            if (!member.value.IsBool()) return false;
            IsFullScreen = member.value.GetBool();
        } else if (member.name == "window_resolution") {
            if (!member.value.IsString()) return false;
            std::stringstream ss(member.value.GetString());
            ss >> Width;
            ss.get();
            ss >> Height;
        }
    }
    return true;
}
