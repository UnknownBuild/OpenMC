#pragma once
#include <cstdint>
#include <random>

#include "Noise.h"

class PerlinNoise final : public Noise {
public:
    PerlinNoise(uint32_t seed = std::default_random_engine::default_seed);

    virtual float Get(float x, float y, float z) override;

private:
    static float fade(float t);
    static float grad(int32_t hash, float x, float y, float z);
    static float lerp(float t, float a, float b);

    uint32_t p[512];
};
