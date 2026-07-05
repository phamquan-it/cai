#ifndef SENTENCE_TEMPLATES_H
#define SENTENCE_TEMPLATES_H

// Cấu trúc 1 mẫu câu
typedef struct {
    char id[32];
    char pattern[256];
    char category[32];      // "greeting", "question", "response", "description"
    char slots[5][32];      // Các slot cần điền
    int slot_count;
    char example[256];
} SentenceTemplate;

// Cấu trúc câu đã điền
typedef struct {
    char template_id[32];
    char filled_sentence[512];
    char filled_slots[5][64];
    int filled_count;
} FilledSentence;

// Hàm chính
void load_sentence_templates();
SentenceTemplate* find_template(const char *id);
SentenceTemplate* find_template_by_pattern(const char *pattern);

// Hàm điền slot
FilledSentence* fill_template(const char *template_id, char *slots[], int slot_count);
char* fill_template_with_string(const char *template_id, const char *slot_values);
void free_filled_sentence(FilledSentence *fs);

// Hàm tạo câu ngẫu nhiên
char* generate_random_sentence(const char *category);
char* generate_response_with_template(const char *input);

// Hàm kiểm tra
int is_template_request(const char *input);
char* answer_template_request(const char *input);

#endif
