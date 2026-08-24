#!/bin/bash
echo "Building Eternal Wanderer AI RPG..."

# Создаем директорию для объектных файлов
mkdir -p build

# Компилируем все cpp файлы
find src -name "*.cpp" | while read file; do
    echo "Compiling $file..."
    g++ -std=c++17 -Isrc -c "$file" -o "build/$(basename ${file%.cpp}.o)" 2>&1 | head -5
done

echo "Build complete!"
ls -la build/*.o | wc -l
