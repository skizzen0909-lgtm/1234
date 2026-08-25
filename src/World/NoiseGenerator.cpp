// World/NoiseGenerator.cpp
#include "NoiseGenerator.h"
#include <algorithm> // Для std::clamp
#include <numeric>   // Для std::iota

NoiseGenerator::NoiseGenerator(unsigned int seed) : gen(seed), dist(-1.0f, 1.0f), seedValue(seed) {
    // Инициализируем таблицу градиентов
    for (int i = 0; i < GRADIENT_TABLE_SIZE * 2; i += 2) {
        // Создаем случайный единичный вектор (градиент)
        float x = dist(gen);
        float y = dist(gen);
        float length = std::sqrt(x * x + y * y);
        if (length > 0) {
            gradientTable[i] = x / length;
            gradientTable[i + 1] = y / length;
        }
        else {
            gradientTable[i] = 1.0f; // На случай, если оба нули
            gradientTable[i + 1] = 0.0f;
        }
    }
}

int NoiseGenerator::hash(int x, int y) const {
    // Простая хэш-функция
    int hash_val = (x * 1619 + y * 31337) ^ (x * y * 19997);
    hash_val = hash_val * hash_val * hash_val * 15731 + 789221;
    hash_val = (hash_val >> 16) ^ hash_val;
    return std::abs(hash_val) % GRADIENT_TABLE_SIZE;
}

void NoiseGenerator::getGradient(int x, int y, float& gx, float& gy) const {
    int index = hash(x, y) * 2;
    gx = gradientTable[index];
    gy = gradientTable[index + 1];
}

float NoiseGenerator::fade(float t) const {
    // 6t^5 - 15t^4 + 10t^3
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float NoiseGenerator::lerp(float a, float b, float t) const {
    return a + t * (b - a);
}

float NoiseGenerator::bilinearInterpolation(float x, float y) const {
    int x0 = static_cast<int>(std::floor(x));
    int y0 = static_cast<int>(std::floor(y));
    int x1 = x0 + 1;
    int y1 = y0 + 1;
    float sx = x - x0;
    float sy = y - y0;
    float fx = fade(sx);
    float fy = fade(sy);
    float n0, n1, ix0, ix1, value;
    float g0x, g0y, d0x, d0y, dot0;
    d0x = x - x0; d0y = y - y0;
    getGradient(x0, y0, g0x, g0y);
    dot0 = d0x * g0x + d0y * g0y;
    n0 = dot0;
    float g1x, g1y, d1x, d1y, dot1;
    d1x = x - x1; d1y = y - y0;
    getGradient(x1, y0, g1x, g1y);
    dot1 = d1x * g1x + d1y * g1y;
    n1 = dot1;
    ix0 = lerp(n0, n1, fx);
    float g2x, g2y, d2x, d2y, dot2;
    d2x = x - x0; d2y = y - y1;
    getGradient(x0, y1, g2x, g2y);
    dot2 = d2x * g2x + d2y * g2y;
    n0 = dot2;
    float g3x, g3y, d3x, d3y, dot3;
    d3x = x - x1; d3y = y - y1;
    getGradient(x1, y1, g3x, g3y);
    dot3 = d3x * g3x + d3y * g3y;
    n1 = dot3;
    ix1 = lerp(n0, n1, fx);
    value = lerp(ix0, ix1, fy);
    return value;
}

float NoiseGenerator::noise(float x, float y) const {
    return bilinearInterpolation(x, y);
}

float NoiseGenerator::fractalNoise(float x, float y, int octaves, float persistence, float scale) const {
    float total = 0;
    float frequency = scale;
    float amplitude = 1.0f;
    float maxValue = 0; // Используется для нормализации результата
    for (int i = 0; i < octaves; i++) {
        total += noise(x * frequency, y * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= 2;
    }
    // Нормализуем результат в диапазон [-1, 1]
    if (maxValue > 0) {
        return total / maxValue;
    }
    else {
        return 0.0f; // На случай, если maxValue стал нулем
    }
}