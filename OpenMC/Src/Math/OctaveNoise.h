#pragma once
#include <cstdint>
#include <type_traits>

#include "Noise.h"

template<typename TNoise, typename std::enable_if<std::is_base_of<Noise, TNoise>::value>::type* = nullptr>
class OctaveNoise final : public Noise {
public:
    OctaveNoise(TNoise innerNoise, int32_t octaves, float persistence, float lacunarity = 2.0f) {
        this->innerNoise = innerNoise;
        this->octaves = octaves;
        this->persistence = persistence;
        this->lacunarity = lacunarity;
    }

    virtual float Get(float x, float y) override {
        float freq = 1.0f;
        float amp = 1.0f;
        float max = 1.0f;
        float total = innerNoise.Get(x, y);
        for (int i = 1; i < octaves; i++) {
            freq *= lacunarity;
            amp *= persistence;
            max += amp;
            total += innerNoise.Get(x * freq, y * freq) * amp;
        }
        return (1 + total / max) / 2;
    }

    virtual float Get(float x, float y, float z) override {
        float freq = 1.0f;
        float amp = 1.0f;
        float max = 1.0f;
        float total = innerNoise.Get(x, y, z);
        for (int i = 1; i < octaves; ++i) {
            freq *= lacunarity;
            amp *= persistence;
            max += amp;
            total += innerNoise.Get(x * freq, y * freq, z * freq) * amp;
        }
        return (1 + total / max) / 2;
    }

private:
    TNoise innerNoise;
    int32_t octaves;
    float persistence;
    float lacunarity;
};
