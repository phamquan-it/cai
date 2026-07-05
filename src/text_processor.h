#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H

#include "markov.h"  // Lấy MAX_WORD_LEN từ markov.h

#define MAX_SENTENCES 50
#define MAX_WORDS_PER_SENTENCE 100

// Cấu trúc 1 câu
typedef struct {
    char original[512];
    char words[MAX_WORDS_PER_SENTENCE][MAX_WORD_LEN];
    int word_count;
    char punctuation[8];
    int is_question;
    int is_exclamation;
} Sentence;

// Cấu trúc đoạn văn
typedef struct {
    Sentence sentences[MAX_SENTENCES];
    int sentence_count;
    char raw_text[4096];
} Paragraph;

// Cấu trúc thông tin trích xuất
typedef struct {
    char numbers[20][32];
    int number_count;
    char names[20][64];
    int name_count;
    char dates[20][32];
    int date_count;
    char keywords[50][64];
    int keyword_count;
} ExtractedInfo;

// Hàm xử lý văn bản
Paragraph* parse_paragraph(const char *text);
void free_paragraph(Paragraph *p);

// Hàm trích xuất thông tin
ExtractedInfo* extract_info(const char *text);
void free_extracted_info(ExtractedInfo *info);

// Hàm hỗ trợ
int is_text_processing_request(const char *input);
char* answer_text_processing_request(const char *input);

#endif
