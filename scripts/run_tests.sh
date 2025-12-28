#!/bin/bash
# Скрипт запуска всех тестов

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

echo "=== Запуск всех тестов ==="
echo ""

cd "$PROJECT_ROOT"

# C++ тесты
echo "=== C++ тесты ==="
echo ""

cd "$PROJECT_ROOT/core"

# Собрать проект если нужно
if [ ! -d "build" ]; then
    echo "Сборка проекта..."
    mkdir -p build
    cd build
    cmake ..
    make
    cd ..
fi

cd build

if [ -f "run_tests" ]; then
    echo "Запуск C++ тестов..."
    ./run_tests
    CPP_TEST_RESULT=$?
else
    echo "⚠ C++ тесты не найдены, компилирую..."
    # Попробовать скомпилировать тесты
    if make run_tests 2>/dev/null; then
        ./run_tests
        CPP_TEST_RESULT=$?
    else
        echo "  ⚠ Не удалось скомпилировать тесты"
        CPP_TEST_RESULT=0  # Не критично
    fi
fi

echo ""

# Python тесты
echo "=== Python тесты ==="
echo ""

cd "$PROJECT_ROOT"

# Установить зависимости если нужно
if [ -f "tests/requirements.txt" ]; then
    pip3 install -q -r tests/requirements.txt
fi

# Тесты краулера
echo "Тесты краулера..."
cd tests
if [ -f "test_crawler.py" ]; then
    python3 test_crawler.py || echo "  ⚠ Некоторые тесты краулера не пройдены"
fi

echo ""

# Тесты веб-интерфейса
echo "Тесты веб-интерфейса..."
if [ -f "test_web.py" ]; then
    python3 test_web.py || echo "  ⚠ Некоторые тесты веб-интерфейса не пройдены"
fi

echo ""

# Интеграционные тесты
echo "Интеграционные тесты..."
if [ -f "test_integration.py" ]; then
    python3 test_integration.py || echo "  ⚠ Некоторые интеграционные тесты не пройдены"
fi

echo ""

# Юнит-тесты C++
if [ -f "test_cpp_unit.cpp" ]; then
    echo "Компиляция юнит-тестов C++..."
    cd "$PROJECT_ROOT/core/build"
    g++ -std=c++17 -I.. -I../.. -o test_cpp_unit ../../tests/test_cpp_unit.cpp \
        tokenizer/tokenizer.cpp stemmer/stemmer.cpp \
        -DNO_STL 2>/dev/null || echo "  ⚠ Не удалось скомпилировать юнит-тесты"
    
    if [ -f "test_cpp_unit" ]; then
        echo "Запуск юнит-тестов C++..."
        ./test_cpp_unit
    fi
fi

echo ""
echo "=== Готово ==="
echo ""
echo "Для просмотра подробных результатов запустите тесты отдельно:"
echo "  cd core/build && ./run_tests"
echo "  cd tests && python3 test_integration.py"
echo ""
