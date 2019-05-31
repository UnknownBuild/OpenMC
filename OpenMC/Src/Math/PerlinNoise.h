#pragma once
#include <cstdint>

#include "Noise.h"

class PerlinNoise : public Noise {
public:
    PerlinNoise(int seed);

private:
    uint8_t p[512];
};
