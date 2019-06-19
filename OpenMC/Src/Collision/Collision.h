#pragma once

#include "../Helpers/Singleton.h"
#include "../SpriteRenderer/SpriteRenderer.h"

class Collision
{
public:
    Collision();
    ~Collision();

    glm::vec3 adjustPosition(glm::vec3);
    bool checkDown(glm::vec3);
    bool checkUp(glm::vec3);
    bool checkPositiveZ(glm::vec3);
    bool checkNegativeZ(glm::vec3);
    bool checkPositiveX(glm::vec3);
    bool checkNegativeX(glm::vec3);

private:
    bool checkPosition(glm::vec3);
};
