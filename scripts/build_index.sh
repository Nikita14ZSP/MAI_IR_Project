#!/bin/bash
# Скрипт построения индекса

set -e

echo "=== Построение индекса ==="
echo ""

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

CORPUS_DIR="${1:-$PROJECT_ROOT/corpus}"
INDEX_PATH="${2:-$PROJECT_ROOT/core/index/boolean_index.bin}"

if [ ! -d "$CORPUS_DIR" ]; then
    echo "Ошибка: директория корпуса не найдена: $CORPUS_DIR"
    exit 1
fi

# Перейти в директорию core
cd "$(dirname "$0")/../core" || exit 1

# Проверить наличие скомпилированного проекта
if [ ! -f "build/libmai_ir_core.a" ]; then
    echo "Ошибка: проект не скомпилирован. Запустите build_all.sh"
    exit 1
fi

echo "Корпус: $CORPUS_DIR"
echo "Индекс: $INDEX_PATH"
echo ""

# TODO: Создать утилиту для построения индекса
# Сейчас нужно создать отдельную программу для построения индекса
# или использовать существующую CLI

echo "Построение индекса..."
"$PROJECT_ROOT/core/build/build_index" "$CORPUS_DIR" "$INDEX_PATH"

echo ""
echo "✓ Индекс построен: $INDEX_PATH"

