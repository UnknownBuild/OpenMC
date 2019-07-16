#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <sstream>
#include <iostream>

#include "Config.h"

using namespace std;
using namespace rapidjson;

bool Config::Load() {
    ifstream ifs(EnvPath::ConfigFile);
    char c;
    string jsonString;
    while (ifs.get(c)) {
        jsonString += c;
    }
    // 判断去除bom头
    if (int(jsonString[0]) == -17 && int(jsonString[1]) == -69) {
        jsonString = jsonString.substr(3);
    }
    Document d;
    d.Parse(jsonString.c_str());
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
        else if (member.name == "window_sight_distance") {
            if (!member.value.IsInt()) return false;
            SightDistance = member.value.GetInt();
        }
    }
    return true;
}

bool Config::Save(bool isFullScreen, int Width, int Height, int sightDistance)
{
    ifstream ifs(EnvPath::ConfigFile);
    char c;
    string jsonString;
    while (ifs.get(c)) {
        jsonString += c;
    }
    // 判断去除bom头
    if (int(jsonString[0]) == -17 && int(jsonString[1]) == -69) {
        jsonString = jsonString.substr(3);
    }
    Document d;
    d.Parse(jsonString.c_str());
    if (d.HasParseError()) {
        return false;
    }
    else if (!d.IsObject()) {
        return false;
    }
    for (auto& member : d.GetObject()) {
        if (member.name == "window_full_screen") {
            if (!member.value.IsBool()) return false;
            member.value.SetBool(isFullScreen);
        }
        else if (member.name == "window_resolution") {
            if (!member.value.IsString()) return false;
            string s;
            s += to_string(Width);
            s += 'x';
            s += to_string(Height);
            char buffer[20];
            int len = sprintf_s(buffer, "%s", s.c_str());
            member.value.SetString(s.c_str(), len, d.GetAllocator());
        }
        else if (member.name == "window_sight_distance") {
            if (!member.value.IsInt()) return false;
            member.value.SetInt(sightDistance);
        }
    }
    ofstream ofs(EnvPath::ConfigFile);
    OStreamWrapper osw(ofs);
    Writer<OStreamWrapper> writer(osw);
    d.Accept(writer);
    return true;
}
