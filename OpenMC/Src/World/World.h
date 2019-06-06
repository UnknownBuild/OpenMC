#pragma once
#include <vector>

#include "../SpriteRenderer/SpriteRenderer.h"
#include "Characters/Character.h"
#include "Types/Region.h"

class World final {
public:
    void Draw(SpriteRenderer* renderer);

private:
    std::vector<Character*> characters;
};
