#include <io.h>
#include <iostream>

#include "../../Helpers/EnvPath.h"
#include "MapManager.h"

bool MapManager::Init(std::string name, uint32_t seed) {
    // 判断存档文件夹是否存在
    if (_access((EnvPath::SaveDir + name).c_str(), 00) == 0) {
        std::cout << FindFirstFileA(name.c_str(), NULL) << std::endl;
    }
    return true;
}
