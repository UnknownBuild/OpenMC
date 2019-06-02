#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include <random>
#include <vector>

#include "Noise.h"

class PerlinNoise final : public Noise {
public:
    PerlinNoise(uint32_t seed = std::default_random_engine::default_seed);

    virtual float Get(float x, float y, float z) override;
    virtual void Get(std::vector<std::vector<std::vector<float>>>& noise, glm::vec3 offset, glm::vec3 scale) override;

private:
    static float fade(float t);
    static float grad(int32_t hash, float x, float y, float z);
    static float lerp(float t, float a, float b);

    uint32_t p[512];
};
