#pragma once
#include "EnvPath.h"

class Config final {
public:
    bool Load();
    bool Save(bool, int, int, int);

    // Window
    bool IsFullScreen = false;
    int Width = 1366;
    int Height = 768;

    // Render
    int SightDistance;
};
