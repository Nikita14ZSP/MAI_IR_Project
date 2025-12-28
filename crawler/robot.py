#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Лабораторная работа 2: Поисковый робот (краулер)
Робот, который обходит сайты и собирает документы
"""

import json
import os
import sys
import time
import requests
from pathlib import Path
from typing import Set, List, Dict, Optional
from urllib.parse import urljoin, urlparse
from collections import deque
from tqdm import tqdm
from bs4 import BeautifulSoup

# TODO: Импортировать функции из download_corpus если нужно


class SearchRobot:
    """Поисковый робот (краулер) для обхода сайта"""
    
    def __init__(self, config_path: str = "config.json"):
        """
        Инициализация поискового робота
        
        Args:
            config_path: путь к файлу конфигурации
        """
        # TODO: Загрузить конфигурацию
        with open(config_path, 'r', encoding='utf-8') as f:
            self.config = json.load(f)
        
        self.output_dir = Path(self.config['output_dir'])
        self.output_dir.mkdir(parents=True, exist_ok=True)
        
        self.session = requests.Session()
        self.session.headers.update({
            'User-Agent': self.config['user_agent']
        })
        
        # Очередь URL для обхода
        self.url_queue = deque()
        
        # Множество уже посещенных URL
        self.visited_urls: Set[str] = set()
        
        # Множество URL для скачивания (статьи)
        self.article_urls: Set[str] = set()
        
        # Статистика
        self.downloaded_count = 0
        self.min_words = self.config['min_words_per_article']
        self.target_size = self.config['corpus_size']
        
        # Домены, которые можно обходить
        self.allowed_domains: Set[str] = set()
    
    def is_allowed_url(self, url: str) -> bool:
        """
        Проверка, разрешен ли URL для обхода
        
        Args:
            url: URL для проверки
            
        Returns:
            True если разрешен, False иначе
        """
        # TODO: Реализовать проверку домена, robots.txt и т.д.
        parsed = urlparse(url)
        
        # Проверка домена
        if self.allowed_domains and parsed.netloc not in self.allowed_domains:
            return False
        
        # TODO: Проверка robots.txt
        # TODO: Проверка типа файла (только HTML)
        
        return True
    
    def is_article_url(self, url: str) -> bool:
        """
        Проверка, является ли URL статьей
        
        Args:
            url: URL для проверки
            
        Returns:
            True если это статья, False иначе
        """
        # TODO: Реализовать эвристики определения статьи
        # Например: проверка пути, наличие определенных параметров и т.д.
        parsed = urlparse(url)
        path = parsed.path.lower()
        
        # Примеры эвристик:
        # - путь содержит /article/, /post/, /news/
        # - путь заканчивается на .html
        article_keywords = ['/article/', '/post/', '/news/', '/blog/']
        return any(keyword in path for keyword in article_keywords)
    
    def extract_links(self, html: str, base_url: str) -> List[str]:
        """
        Извлечение ссылок из HTML
        
        Args:
            html: HTML содержимое страницы
            base_url: базовый URL для относительных ссылок
            
        Returns:
            список абсолютных URL
        """
        # TODO: Реализовать извлечение ссылок из HTML
        soup = BeautifulSoup(html, 'lxml')
        links = []
        
        for tag in soup.find_all('a', href=True):
            href = tag['href']
            absolute_url = urljoin(base_url, href)
            links.append(absolute_url)
        
        return links
    
    def fetch_page(self, url: str) -> Optional[Dict[str, str]]:
        """
        Получение страницы по URL
        
        Args:
            url: URL страницы
            
        Returns:
            словарь с данными страницы или None при ошибке
        """
        # TODO: Реализовать получение страницы с обработкой ошибок
        try:
            response = self.session.get(
                url,
                timeout=self.config['timeout']
            )
            response.raise_for_status()
            response.encoding = 'utf-8'
            
            soup = BeautifulSoup(response.text, 'lxml')
            
            title = soup.find('title')
            title_text = title.get_text(strip=True) if title else "Untitled"
            
            # Извлечение текста
            content = soup.find('article') or soup.find('main') or soup.find('body')
            text = content.get_text(separator='\n', strip=True) if content else ""
            
            return {
                'url': url,
                'title': title_text,
                'text': text,
                'html': response.text
            }
        except Exception as e:
            print(f"Ошибка при загрузке {url}: {e}", file=sys.stderr)
            return None
    
    def count_words(self, text: str) -> int:
        """Подсчет количества слов"""
        words = text.split()
        return len(words)
    
    def normalize_filename(self, title: str, doc_id: int) -> str:
        """Нормализация имени файла"""
        safe_title = "".join(c if c.isalnum() or c in (' ', '-', '_') else '_' for c in title)
        safe_title = safe_title[:100]
        return f"{doc_id:06d}_{safe_title}.txt"
    
    def save_article(self, article: Dict[str, str], doc_id: int) -> bool:
        """Сохранение статьи"""
        word_count = self.count_words(article['text'])
        if word_count < self.min_words:
            return False
        
        filename = self.normalize_filename(article['title'], doc_id)
        filepath = self.output_dir / filename
        
        try:
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(f"TITLE: {article['title']}\n")
                f.write(f"URL: {article['url']}\n")
                f.write(f"WORDS: {word_count}\n")
                f.write("\n" + "="*80 + "\n\n")
                f.write(article['text'])
            return True
        except Exception as e:
            print(f"Ошибка при сохранении {filepath}: {e}", file=sys.stderr)
            return False
    
    def crawl(self, start_url: str, max_depth: int = 3):
        """
        Основной метод обхода сайта
        
        Args:
            start_url: начальный URL для обхода
            max_depth: максимальная глубина обхода
        """
        print(f"Начало обхода с URL: {start_url}")
        print(f"Максимальная глубина: {max_depth}")
        print(f"Целевой размер корпуса: {self.target_size} статей")
        
        # Инициализация очереди
        self.url_queue.append((start_url, 0))  # (url, depth)
        
        # Извлечение домена для фильтрации
        parsed_start = urlparse(start_url)
        self.allowed_domains.add(parsed_start.netloc)
        
        doc_id = 1
        
        # TODO: Использовать tqdm для прогресс-бара
        with tqdm(total=self.target_size, desc="Собрано статей") as pbar:
            while self.url_queue and self.downloaded_count < self.target_size:
                url, depth = self.url_queue.popleft()
                
                # Пропуск уже посещенных
                if url in self.visited_urls:
                    continue
                
                # Проверка глубины
                if depth > max_depth:
                    continue
                
                # Проверка разрешенности URL
                if not self.is_allowed_url(url):
                    continue
                
                self.visited_urls.add(url)
                
                # Получение страницы
                page = self.fetch_page(url)
                if not page:
                    continue
                
                # Если это статья - сохранить
                if self.is_article_url(url):
                    if self.save_article(page, doc_id):
                        self.downloaded_count += 1
                        doc_id += 1
                        pbar.update(1)
                
                # Если не достигли максимальной глубины - извлечь ссылки
                if depth < max_depth:
                    links = self.extract_links(page['html'], url)
                    for link in links:
                        if link not in self.visited_urls:
                            self.url_queue.append((link, depth + 1))
                
                # Задержка между запросами
                time.sleep(self.config['delay_between_requests'])
        
        print(f"\nОбход завершен. Собрано статей: {self.downloaded_count}")
        print(f"Посещено URL: {len(self.visited_urls)}")
        print(f"Корпус сохранен в: {self.output_dir}")


def main():
    """Точка входа"""
    # TODO: Обработка аргументов командной строки
    config_path = sys.argv[1] if len(sys.argv) > 1 else "config.json"
    start_url = sys.argv[2] if len(sys.argv) > 2 else "https://example.com"
    
    robot = SearchRobot(config_path)
    robot.crawl(start_url, max_depth=3)
    
    # TODO: Сохранить robots.txt если требуется


if __name__ == "__main__":
    main()

