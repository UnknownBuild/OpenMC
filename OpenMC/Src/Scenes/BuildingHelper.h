#pragma once
#include "../Camera/Camera.h"
#include "../SpriteRenderer/SpriteRenderer.h"
#include "../World/World.h"
#include "Scene.h"

class BuildingHelper {
public:
    BuildingHelper();
    void buildTree(glm::vec3 pos, int height);
    void buildHouse(glm::vec3 pos);
private:
    SpriteRenderer* renderer;
};
