#ifndef DEFINITION_H
#define DEFINITION_H

#define MAX_DEF_LEN 512
#define MAX_KEYWORD_LEN 64

// Cấu trúc định nghĩa
typedef struct {
    char keyword[MAX_KEYWORD_LEN];
    char definition[MAX_DEF_LEN];
    char example[MAX_DEF_LEN];
} Definition;

// Hàm phân tích câu hỏi "là gì", "là thế nào", "là ai"
int is_definition_question(const char *input);
char* extract_keyword(const char *input);
char* get_definition(const char *keyword);
char* answer_definition_question(const char *input);

// Hàm quản lý từ điển định nghĩa
void load_definitions();
void add_definition(const char *keyword, const char *def, const char *example);
void free_definitions();

#endif
