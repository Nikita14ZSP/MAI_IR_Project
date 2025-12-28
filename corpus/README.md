# Corpus Directory

This directory is intended for storing the document corpus.

## Structure

```
corpus/
├── doc_00001.txt
├── doc_00002.txt
├── ...
├── doc_30000.txt
└── metadata.json
```

## How to Download Corpus

To download the corpus, run:

```bash
cd MAI_IR_Project
./scripts/download_corpus.sh
```

This will download 30,000+ scientific articles from arXiv.org in the Computer Science (AI) category.

## Configuration

Edit `crawler/config.json` to customize:
- Number of documents
- Word count per document
- Category
- Date range

## Note

The corpus files are not included in the repository due to their large size.
Run the download script to generate them locally.

