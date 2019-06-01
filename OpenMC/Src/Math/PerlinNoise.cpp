#include <random>

#include "PerlinNoise.h"

PerlinNoise::PerlinNoise(uint32_t seed) {
    for (size_t i = 0; i < 256; i++) {
        p[i] = i;
    }
    std::shuffle(std::begin(p), std::begin(p) + 256, std::default_random_engine(seed));
    for (size_t i = 0; i < 256; ++i) {
        p[256 + i] = p[i];
    }
}

float PerlinNoise::Get(float x, float y, float z) {
    const int32_t X = static_cast<int32_t>(std::floor(x)) & 255;
    const int32_t Y = static_cast<int32_t>(std::floor(y)) & 255;
    const int32_t Z = static_cast<int32_t>(std::floor(z)) & 255;

    x -= std::floor(x);
    y -= std::floor(y);
    z -= std::floor(z);

    const float u = fade(x);
    const float v = fade(y);
    const float w = fade(z);

    const int32_t A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
    const int32_t B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

    return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
        grad(p[BA], x - 1, y, z)),
        lerp(u, grad(p[AB], x, y - 1, z),
            grad(p[BB], x - 1, y - 1, z))),
        lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
            grad(p[BA + 1], x - 1, y, z - 1)),
            lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
                grad(p[BB + 1], x - 1, y - 1, z - 1))));
}

void PerlinNoise::Get(std::vector<std::vector<std::vector<float>>>& noise, glm::vec3 offset, glm::vec3 scale) {
    for (size_t x = 0; x < noise.size(); x++) {
        float xOffset = offset.x + x * scale.x;
        for (size_t y = 0; y < noise[0].size(); y++) {
            float yOffset = offset.y + y * scale.y;
            for (size_t z = 0; z < noise[0][0].size(); z++) {
                float zOffset = offset.z + z * scale.z;
                noise[x][y][z] = Get(xOffset, yOffset, zOffset);
            }
        }
    }
}

float PerlinNoise::fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float PerlinNoise::grad(int32_t hash, float x, float y, float z) {
    const int32_t h = hash & 15;
    const double u = h < 8 ? x : y;
    const double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float PerlinNoise::lerp(float t, float a, float b) {
    return a + t * (b - a);
}
