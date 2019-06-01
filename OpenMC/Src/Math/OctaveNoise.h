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

    virtual void Get(std::vector<std::vector<std::vector<float>>>& noise, glm::vec3 offset, glm::vec3 scale) override {
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

private:
    TNoise innerNoise;
    int32_t octaves;
    float persistence;
};
