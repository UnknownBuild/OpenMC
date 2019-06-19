#include "Collision.h"

#include <iostream>
#include <math.h>
using namespace std;

Collision::Collision()
{
}

Collision::~Collision()
{
}

glm::vec3 Collision::adjustPosition(glm::vec3 position)
{
    position += 0.5;
    return floor(position);
}

bool Collision::checkDown(glm::vec3 postion)
{
    postion.y -= 2.0f;
    return checkPosition(postion);
}

bool Collision::checkUp(glm::vec3 postion)
{
    postion.y += 0.5f;
    return checkPosition(postion);
}

bool Collision::checkPositiveZ(glm::vec3 postion)
{
    postion.z += 0.5f;
    return checkPosition(postion) || checkPosition(glm::vec3(postion.x, postion.y - 1.0f, postion.z));
}

bool Collision::checkNegativeZ(glm::vec3 postion)
{
    postion.z -= 0.5f;
    return checkPosition(postion) || checkPosition(glm::vec3(postion.x, postion.y - 1.0f, postion.z));
}

bool Collision::checkPositiveX(glm::vec3 postion)
{
    postion.x += 0.5f;
    return checkPosition(postion) || checkPosition(glm::vec3(postion.x, postion.y - 1.0f, postion.z));
}

bool Collision::checkNegativeX(glm::vec3 postion)
{
    postion.x -= 0.5f;
    return checkPosition(postion) || checkPosition(glm::vec3(postion.x, postion.y - 1.0f, postion.z));
}

bool Collision::checkPosition(glm::vec3 postion)
{
    postion = adjustPosition(postion);
    BlockData data = Singleton<SpriteRenderer>::GetInstance()->GetBlock(postion);
    return !(data.Type == BlockType::None || data.Type  == BlockType::Liquid);
}


