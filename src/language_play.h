#ifndef LANGUAGE_PLAY_H
#define LANGUAGE_PLAY_H

// Cấu trúc từ vựng
typedef struct {
    char word[64];
    char type[32];      // "don", "ghep", "lay", "duong", "am", "tay", "han", "anh"
    char meaning[256];
} Vocabulary;

// Hàm chính
void load_vocabulary();
char* generate_compound_word(const char *base);
char* generate_reduplicative_word(const char *base);
char* generate_sentence_with_word(const char *word, const char *topic);
char* answer_language_request(const char *input);

// Hàm hỗ trợ
int is_language_request(const char *input);

#endif
