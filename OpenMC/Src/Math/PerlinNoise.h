#pragma once
#include <cstdint>
#include <random>

class PerlinNoise {
public:
    PerlinNoise(uint32_t seed = std::default_random_engine::default_seed);

    double Noise(double x);
    double Noise(double x, double y);
    double Noise(double x, double y, double z);
    double OctaveNoise(double x, int32_t octaves);
    double OctaveNoise(double x, double y, int32_t octaves);
    double OctaveNoise(double x, double y, double z, int32_t octaves);

private:
    static double fade(double t);
    static double grad(int32_t hash, double x, double y, double z);
    static double lerp(double t, double a, double b);

    uint32_t p[512];
};
