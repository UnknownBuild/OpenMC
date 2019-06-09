#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include <random>
#include <vector>

#include "Noise.h"

class PerlinNoise final : public Noise {
public:
    PerlinNoise(uint32_t seed = std::default_random_engine::default_seed);

    virtual float Get(float x, float y) override;
    virtual float Get(float x, float y, float z) override;

private:
    uint8_t p[512];
};
