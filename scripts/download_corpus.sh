#!/bin/bash
# Скрипт скачивания корпуса документов

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
CORPUS_DIR="$PROJECT_ROOT/corpus"
CRAWLER_DIR="$PROJECT_ROOT/crawler"

echo "=== Скачивание корпуса документов ==="
echo ""

# Перейти в директорию crawler
cd "$CRAWLER_DIR" || exit 1

# Проверить наличие Python
if ! command -v python3 &> /dev/null; then
    echo "Ошибка: python3 не найден"
    exit 1
fi

# Создать виртуальное окружение если нужно
if [ ! -d "venv" ]; then
    echo "Создание виртуального окружения..."
    python3 -m venv venv
fi

# Активировать виртуальное окружение
source venv/bin/activate

# Установить зависимости
echo "Установка зависимостей..."
pip install -q --upgrade pip
pip install -q -r requirements.txt

# Проверить конфигурацию
if [ ! -f "config.json" ]; then
    echo "Ошибка: config.json не найден"
    exit 1
fi

echo ""
echo "Конфигурация:"
python3 -c "import json; c=json.load(open('config.json')); print(f\"  Категория: {c.get('category', 'N/A')}\"); print(f\"  Целевой размер: {c.get('corpus_size', 'N/A')} статей\"); print(f\"  Минимум слов: {c.get('min_words_per_article', 'N/A')}\")"

echo ""

# Проверить существующие файлы
if [ -d "$CORPUS_DIR" ]; then
    existing_count=$(find "$CORPUS_DIR" -name "doc_*.txt" -type f 2>/dev/null | wc -l | tr -d ' ')
    echo "Найдено существующих документов: $existing_count"
    
    # Проверить метаданные
    if [ -f "$CORPUS_DIR/metadata.json" ]; then
        metadata_count=$(python3 -c "import json; d=json.load(open('$CORPUS_DIR/metadata.json')); print(d.get('total_documents', 0))" 2>/dev/null || echo "0")
        echo "Согласно metadata.json: $metadata_count документов"
    fi
else
    echo "Директория корпуса будет создана: $CORPUS_DIR"
fi

echo ""

# Запросить подтверждение
read -p "Продолжить скачивание? (y/n): " -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo "Отменено"
    exit 0
fi

echo ""
echo "Запуск скачивания корпуса..."
echo "Это может занять продолжительное время..."
echo ""

# Запустить скачивание
python3 download_corpus.py config.json

# Проверить результат
echo ""
echo "=== Проверка результатов ==="

if [ -d "$CORPUS_DIR" ]; then
    final_count=$(find "$CORPUS_DIR" -name "doc_*.txt" -type f 2>/dev/null | wc -l | tr -d ' ')
    echo "Всего файлов в корпусе: $final_count"
    
    # Проверить метаданные
    if [ -f "$CORPUS_DIR/metadata.json" ]; then
        echo ""
        echo "Метаданные корпуса:"
        python3 <<EOF
import json
import sys
try:
    with open('$CORPUS_DIR/metadata.json', 'r', encoding='utf-8') as f:
        data = json.load(f)
    print(f"  Всего документов: {data.get('total_documents', 0)}")
    print(f"  Категория: {data.get('category', 'N/A')}")
    print(f"  Минимум слов: {data.get('min_words', 'N/A')}")
    print(f"  Последнее обновление: {data.get('last_updated', 'N/A')}")
    
    # Статистика по словам
    if 'articles' in data and data['articles']:
        word_counts = [a.get('word_count', 0) for a in data['articles']]
        if word_counts:
            print(f"  Среднее количество слов: {sum(word_counts) // len(word_counts)}")
            print(f"  Минимум слов: {min(word_counts)}")
            print(f"  Максимум слов: {max(word_counts)}")
except Exception as e:
    print(f"Ошибка при чтении метаданных: {e}", file=sys.stderr)
    sys.exit(1)
EOF
    else
        echo "Предупреждение: metadata.json не найден"
    fi
    
    # Проверить размер корпуса
    corpus_size=$(du -sh "$CORPUS_DIR" 2>/dev/null | cut -f1)
    echo ""
    echo "Размер корпуса на диске: $corpus_size"
    
    # Проверить, достигнут ли целевой размер
    target_size=$(python3 -c "import json; c=json.load(open('config.json')); print(c.get('corpus_size', 30000))")
    if [ "$final_count" -ge "$target_size" ]; then
        echo ""
        echo "✓ Корпус готов! Достигнут целевой размер: $final_count >= $target_size"
    else
        echo ""
        echo "⚠ Целевой размер еще не достигнут: $final_count < $target_size"
        echo "Запустите скрипт снова для докачивания оставшихся статей"
    fi
else
    echo "Ошибка: директория корпуса не создана"
    exit 1
fi

echo ""
echo "✓ Готово!"
