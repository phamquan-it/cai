#ifndef KNOWLEDGE_H
#define KNOWLEDGE_H

// Cấu trúc kiến thức
typedef struct {
    char keyword[64];
    char category[32];      // "keyword", "syntax", "function", "concept"
    char description[512];
    char example[512];
    char note[256];
} KnowledgeItem;

// Hàm phát hiện câu hỏi về C
int is_c_knowledge_question(const char *input);
char* answer_c_knowledge_question(const char *input);

// Hàm tra cứu kiến thức
char* get_c_knowledge(const char *keyword);
void load_c_knowledge();

// THÊM: Hàm thêm kiến thức
void add_knowledge(const char *keyword, const char *category, 
                   const char *desc, const char *example, const char *note);

#endif
