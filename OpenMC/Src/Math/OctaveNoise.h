#pragma once
#include <cstdint>
#include <type_traits>

#include "Noise.h"

template<typename TNoise, typename std::enable_if<std::is_base_of<Noise, TNoise>::value>::type* = nullptr>
class OctaveNoise final : public Noise {
public:
    OctaveNoise(TNoise innerNoise, int32_t octaves, float persistence) {
        this->innerNoise = innerNoise;
        this->octaves = octaves;
        this->persistence = persistence;
    }

    virtual float Get(float x, float y, float z) override {
        double total = 0, amplitude = 1, maxValue = 0;
        int frequency = 1;
        for (int i = 0; i < octaves; i++) {
            total += innerNoise.Get(x * frequency, y * frequency, z * frequency) * amplitude;
            maxValue += amplitude;
            amplitude *= persistence;
            frequency *= 2;
        }
        return static_cast<float>(total / maxValue);
    }

private:
    TNoise innerNoise;
    int32_t octaves;
    float persistence;
};
