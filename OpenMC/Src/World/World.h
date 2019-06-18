#pragma once
#include <vector>

#include "../SpriteRenderer/SpriteRenderer.h"
#include "Characters/Character.h"
#include "Types/Region.h"

class World final {
public:
    bool Init(std::string name);
    bool Init(std::string name, uint32_t seed);
    void Draw(SpriteRenderer* renderer);

private:
    // std::vector<Character> characters;
};
