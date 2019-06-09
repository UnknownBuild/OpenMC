#pragma once
#include <glm/glm.hpp>
#include <vector>

class Noise {
public:
    virtual float Get(float x, float y) = 0;
    virtual float Get(float x, float y, float z) = 0;
};
