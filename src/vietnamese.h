#ifndef VIETNAMESE_H
#define VIETNAMESE_H

// Cấu trúc câu
typedef struct {
    char original[512];
    char normalized[512];      // Chuẩn hóa (không dấu)
    char words[50][64];        // Các từ
    int word_count;
    char punctuation[16];      // Dấu câu cuối câu
} VietnameseSentence;

// Hàm xử lý tiếng Việt
VietnameseSentence* parse_vietnamese(const char *input);
void free_vietnamese(VietnameseSentence *vs);

// Hàm chuẩn hóa
void remove_accents(const char *src, char *dst);
void normalize_vietnamese(char *str);

// Hàm đặt câu
char* format_vietnamese_sentence(const char *input);
char* capitalize_vietnamese(const char *text);

// Hàm kiểm tra
int is_vietnamese_text(const char *text);
int count_vietnamese_words(const char *text);

#endif
