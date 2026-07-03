#ifndef CONTEXT_FILTER_H
#define CONTEXT_FILTER_H

#include "reasoning.h"

// Cấu trúc từ có nghĩa
typedef struct {
    char word[64];
    char type[32];      // "variable", "function", "concept", "keyword"
    float weight;       // Độ quan trọng
} MeaningfulWord;

// Cấu trúc ngữ cảnh đã lọc
typedef struct {
    MeaningfulWord words[50];
    int count;
    char raw_input[512];
    char context_type[32];  // "math", "physics", "chemistry", "general"
} FilteredContext;

// Hàm chính
FilteredContext* filter_context(const char *input);
void free_filtered_context(FilteredContext *fc);

// Hàm kiểm tra từ có nghĩa
int is_meaningful_word(const char *word);
int is_variable_word(const char *word);
int is_keyword(const char *word);

// Hàm lấy context type
char* detect_context_type(FilteredContext *fc);

// Hàm tích hợp với reasoning
char* reasoning_with_context(const char *input, FilteredContext *fc);

#endif
