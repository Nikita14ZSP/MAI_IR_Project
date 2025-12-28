#!/bin/bash
# Скрипт сборки всего проекта

set -e  # Прервать при ошибке

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

echo "=== Сборка проекта MAI-IR ==="
echo ""

cd "$PROJECT_ROOT"

# 1. Установка Python зависимостей
echo "1. Установка Python зависимостей..."
echo ""

echo "  - Зависимости краулера..."
if [ -f "crawler/requirements.txt" ]; then
    pip3 install -q -r crawler/requirements.txt || echo "    ⚠ Ошибка установки зависимостей краулера"
fi

echo "  - Зависимости веб-интерфейса..."
if [ -f "web/requirements.txt" ]; then
    pip3 install -q -r web/requirements.txt || echo "    ⚠ Ошибка установки зависимостей веб-интерфейса"
fi

echo "  - Зависимости тестов..."
if [ -f "tests/requirements.txt" ]; then
    pip3 install -q -r tests/requirements.txt || echo "    ⚠ Ошибка установки зависимостей тестов"
fi

echo ""

# 2. Сборка C++ кода
echo "2. Сборка C++ кода..."
echo ""

cd "$PROJECT_ROOT/core"

# Создать директорию для сборки
mkdir -p build
cd build

# Запустить CMake
echo "  Запуск CMake..."
cmake .. || {
    echo "    ✗ Ошибка CMake"
    exit 1
}

# Сборка
echo "  Компиляция проекта..."
make -j$(sysctl -n hw.ncpu 2>/dev/null || echo 4) || {
    echo "    ✗ Ошибка компиляции"
    exit 1
}

echo ""
echo "✓ Сборка завершена успешно!"
echo ""

# 3. Запуск тестов (опционально)
if [ "${1}" != "--no-tests" ]; then
    echo "3. Запуск тестов..."
    echo ""
    
    if [ -f "run_tests" ]; then
        ./run_tests || echo "    ⚠ Некоторые тесты не пройдены"
    else
        echo "    ⚠ Тесты не найдены"
    fi
    
    echo ""
fi

echo "=== Готово ==="
echo ""
echo "Исполняемые файлы:"
echo "  - CLI поиск: core/build/search_cli"
echo "  - Построение индекса: core/build/build_index"
echo "  - Анализ Ципфа: core/build/zipf_analysis"
echo ""
echo "Следующие шаги:"
echo "  1. Построить индекс: ./scripts/build_index.sh"
echo "  2. Запустить веб-сервер: ./scripts/run_all.sh"
echo ""
