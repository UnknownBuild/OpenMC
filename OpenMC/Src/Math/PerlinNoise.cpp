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

double PerlinNoise::Noise(double x) {
    return Noise(x, 0, 0);
}

double PerlinNoise::Noise(double x, double y) {
    return Noise(x, y, 0);
}

double PerlinNoise::Noise(double x, double y, double z) {
    const int32_t X = static_cast<int32_t>(std::floor(x)) & 255;
    const int32_t Y = static_cast<int32_t>(std::floor(y)) & 255;
    const int32_t Z = static_cast<int32_t>(std::floor(z)) & 255;

    x -= std::floor(x);
    y -= std::floor(y);
    z -= std::floor(z);

    const double u = fade(x);
    const double v = fade(y);
    const double w = fade(z);

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

double PerlinNoise::OctaveNoise(double x, int32_t octaves) {
    double result = 0.0;
    double amp = 1.0;
    for (int32_t i = 0; i < octaves; ++i) {
        result += Noise(x) * amp;
        x *= 2.0;
        amp *= 0.5;
    }
    return result;
}

double PerlinNoise::OctaveNoise(double x, double y, int32_t octaves) {
    double result = 0.0;
    double amp = 1.0;
    for (int32_t i = 0; i < octaves; ++i) {
        result += Noise(x, y) * amp;
        x *= 2.0;
        y *= 2.0;
        amp *= 0.5;
    }
    return result;
}

double PerlinNoise::OctaveNoise(double x, double y, double z, int32_t octaves) {
    double result = 0.0;
    double amp = 1.0;
    for (int32_t i = 0; i < octaves; ++i) {
        result += Noise(x, y, z) * amp;
        x *= 2.0;
        y *= 2.0;
        z *= 2.0;
        amp *= 0.5;
    }
    return result;
}

double PerlinNoise::fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::grad(int32_t hash, double x, double y, double z) {
    const int32_t h = hash & 15;
    const double u = h < 8 ? x : y;
    const double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

double PerlinNoise::lerp(double t, double a, double b) {
    return a + t * (b - a);
}
