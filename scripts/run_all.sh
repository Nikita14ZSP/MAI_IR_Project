#!/bin/bash
# Скрипт полного запуска проекта

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

echo "=== Запуск проекта MAI-IR ==="
echo ""

cd "$PROJECT_ROOT"

# Проверить наличие корпуса
CORPUS_DIR="$PROJECT_ROOT/corpus"
CORPUS_COUNT=$(find "$CORPUS_DIR" -name "doc_*.txt" 2>/dev/null | wc -l | tr -d ' ')

if [ "$CORPUS_COUNT" -eq 0 ]; then
    echo "⚠ ВНИМАНИЕ: Корпус документов не найден"
    echo "Запустить скачивание корпуса? (y/n)"
    read -r response
    if [[ "$response" =~ ^[Yy]$ ]]; then
        echo ""
        echo "Запуск скачивания корпуса..."
        ./scripts/download_corpus.sh
    else
        echo "Скачивание пропущено. Проект может не работать без корпуса."
        exit 1
    fi
else
    echo "✓ Найдено файлов корпуса: $CORPUS_COUNT"
fi

echo ""

# Проверить наличие индекса
INDEX_PATH="$PROJECT_ROOT/core/index/boolean_index.bin"
BUILD_INDEX="$PROJECT_ROOT/core/build/build_index"

if [ ! -f "$INDEX_PATH" ]; then
    echo "⚠ Индекс не найден"
    
    if [ ! -f "$BUILD_INDEX" ]; then
        echo "Сборка проекта..."
        ./scripts/build_all.sh --no-tests
    fi
    
    echo "Построение индекса..."
    echo ""
    
    if [ -f "$BUILD_INDEX" ]; then
        "$BUILD_INDEX" "$CORPUS_DIR" "$INDEX_PATH"
    else
        echo "✗ Ошибка: бинарник build_index не найден"
        echo "Запустите: ./scripts/build_all.sh"
        exit 1
    fi
else
    echo "✓ Индекс найден: $INDEX_PATH"
fi

echo ""

# Проверить наличие CLI бинарника
CLI_BINARY="$PROJECT_ROOT/core/build/search_cli"
if [ ! -f "$CLI_BINARY" ]; then
    echo "Сборка проекта..."
    ./scripts/build_all.sh --no-tests
fi

if [ ! -f "$CLI_BINARY" ]; then
    echo "✗ Ошибка: CLI binary не найден"
    exit 1
fi

echo "✓ CLI binary найден"
echo ""

# Запуск веб-сервера
echo "=== Запуск веб-сервера ==="
echo ""

cd "$PROJECT_ROOT/web"

# Установить переменные окружения
export INDEX_PATH="$INDEX_PATH"
export CORPUS_DIR="$CORPUS_DIR"
export CLI_BINARY="$CLI_BINARY"
export PORT="${PORT:-5000}"
export DEBUG="${DEBUG:-False}"

echo "Конфигурация:"
echo "  INDEX_PATH: $INDEX_PATH"
echo "  CORPUS_DIR: $CORPUS_DIR"
echo "  CLI_BINARY: $CLI_BINARY"
echo "  PORT: $PORT"
echo ""

echo "Веб-сервер запущен на http://localhost:$PORT"
echo "Для остановки нажмите Ctrl+C"
echo ""

python3 app.py
