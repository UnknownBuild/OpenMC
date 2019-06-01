#pragma once
#include <glm/glm.hpp>
#include <vector>

class Noise {
public:
    virtual float Get(float x, float y, float z) = 0;
    virtual void Get(std::vector<std::vector<std::vector<float>>>& noise, glm::vec3 offset, glm::vec3 scale) = 0;
};
