// World/NoiseGenerator.h
#pragma once
#include <random>
#include <cmath>
#include <vector>
class NoiseGenerator {
private:
    std::mt19937 gen;
    std::uniform_real_distribution<float> dist;
    unsigned int seedValue; // Добавляем переменную для хранения seed
    // Увеличиваем размер таблицы градиентов для лучшего качества
    static const int GRADIENT_TABLE_SIZE = 256;
    float gradientTable[GRADIENT_TABLE_SIZE * 2]; // x, y градиенты
    // Хэш-функция для псевдослучайности
    int hash(int x, int y) const;
    // Получить градиент по координатам решетки
    void getGradient(int x, int y, float& gx, float& gy) const;
    // Функция сглаживания
    float fade(float t) const;
    // Линейная интерполяция
    float lerp(float a, float b, float t) const;
    // Билинейная интерполяция шума
    float bilinearInterpolation(float x, float y) const;
public:
    // Конструктор, принимающий seed
    explicit NoiseGenerator(unsigned int seed = 0);
    // Генерирует значение шума Перлина в точке (x, y)
    float noise(float x, float y) const;
    // Генерирует значение шума с_octaves октавами
    float fractalNoise(float x, float y, int octaves = 4, float persistence = 0.5f, float scale = 1.0f) const;

    // Добавляем метод getSeed
    unsigned int getSeed() const {
        return seedValue;
    }
};