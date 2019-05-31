#pragma once
#include <vector>

#include "Characters/Character.h"
#include "Types/Region.h"

class World final {
public:
    void Draw();

private:
    std::vector<Character*> characters;
};
