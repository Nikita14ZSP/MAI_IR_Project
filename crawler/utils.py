#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Вспомогательные функции для краулера
"""

import re
from pathlib import Path
from typing import List


def count_words(text: str) -> int:
    """
    Подсчет количества слов в тексте
    
    Args:
        text: текст для подсчета
        
    Returns:
        количество слов
    """
    if not text:
        return 0
    # Разбиение по пробелам и знакам препинания
    words = re.findall(r'\b\w+\b', text)
    return len(words)


def normalize_filename(title: str, doc_id: int, max_length: int = 100) -> str:
    """
    Нормализация имени файла
    
    Args:
        title: заголовок статьи
        doc_id: идентификатор документа
        max_length: максимальная длина имени файла
        
    Returns:
        нормализованное имя файла в формате doc_XXXXX.txt
    """
    # Формат: doc_00001.txt, doc_00002.txt, ...
    return f"doc_{doc_id:05d}.txt"


def clean_text(text: str) -> str:
    """
    Очистка текста от лишних символов
    
    Args:
        text: исходный текст
        
    Returns:
        очищенный текст
    """
    if not text:
        return ""
    # Удаление множественных пробелов
    text = re.sub(r'\s+', ' ', text)
    # Нормализация переносов строк
    text = re.sub(r'\n\s*\n+', '\n\n', text)
    return text.strip()


def extract_text_from_html(html: str) -> str:
    """
    Извлечение чистого текста из HTML
    
    Args:
        html: HTML содержимое
        
    Returns:
        чистый текст
    """
    try:
        from bs4 import BeautifulSoup
        soup = BeautifulSoup(html, 'lxml')
        
        # Удаление script и style элементов
        for script in soup(["script", "style", "meta", "link"]):
            script.decompose()
        
        text = soup.get_text(separator='\n', strip=True)
        return clean_text(text)
    except Exception:
        return clean_text(html)


def sanitize_title(title: str) -> str:
    """
    Очистка заголовка от специальных символов
    
    Args:
        title: исходный заголовок
        
    Returns:
        очищенный заголовок
    """
    if not title:
        return "Untitled"
    # Удаление лишних пробелов и символов
    title = re.sub(r'\s+', ' ', title)
    # Удаление управляющих символов
    title = ''.join(char for char in title if ord(char) >= 32 or char in '\n\r\t')
    return title.strip()
