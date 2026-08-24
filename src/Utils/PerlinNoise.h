// Utils/PerlinNoise.h
#pragma once
#include "World/NoiseGenerator.h"
#include <memory>

class PerlinNoise {
private:
    std::unique_ptr<NoiseGenerator> noiseGen;
public:
    explicit PerlinNoise(unsigned int seed = 0)
        : noiseGen(std::make_unique<NoiseGenerator>(seed)) {
    }

    float fractalNoise(float x, float y, int octaves = 4, float persistence = 0.5f, float scale = 1.0f) const {
        return noiseGen->fractalNoise(x, y, octaves, persistence, scale);
    }

    unsigned int getSeed() const {
        return noiseGen->getSeed();
    }
};