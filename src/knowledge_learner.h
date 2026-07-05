#ifndef KNOWLEDGE_LEARNER_H
#define KNOWLEDGE_LEARNER_H

#include "markov.h"

// Cấu trúc 1 tri thức học được
typedef struct {
    char sentence[512];
    char keywords[10][64];
    int keyword_count;
    char context[128];
    int importance;  // 1-10
    char source[128];
} LearnedKnowledge;

// Cấu trúc kho tri thức
typedef struct {
    LearnedKnowledge items[500];
    int count;
    char learned_from[10][128];
    int file_count;
} KnowledgeBase;

// Hàm chính
KnowledgeBase* init_knowledge_base();
void free_knowledge_base(KnowledgeBase *kb);

// Hàm học từ file
int learn_from_file(KnowledgeBase *kb, const char *filename, Dictionary *dict);
int learn_from_text(KnowledgeBase *kb, const char *text, const char *source, Dictionary *dict);

// Hàm trích xuất
void extract_keywords(const char *sentence, char keywords[][64], int *count);
void extract_context(const char *sentence, char *context);

// Hàm truy vấn
char* search_knowledge(KnowledgeBase *kb, const char *query);
char* get_knowledge_summary(KnowledgeBase *kb);

// Hàm tích hợp
int is_learn_request(const char *input);
char* answer_learn_request(const char *input, KnowledgeBase *kb, Dictionary *dict);

#endif
