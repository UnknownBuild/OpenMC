#include <random>

#include "PerlinNoise.h"

PerlinNoise::PerlinNoise(int seed) {
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<uint8_t> distribution(0, 255);
    for (int i = 0; i < 256; i++) {
        p[i + 256] = p[i] = distribution(generator);
    }
}
6
