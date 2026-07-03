#ifndef CONTEXT_TRACKER_H
#define CONTEXT_TRACKER_H

#include <time.h>  // THÊM DÒNG NÀY

#define MAX_KEYWORDS 100
#define MAX_CONDITIONS 50
#define MAX_CONTEXT_ITEMS 50

// Cấu trúc 1 từ khóa đáng chú ý
typedef struct {
    char word[64];
    char type[32];      // "keyword", "condition", "requirement", "topic"
    char context[256];
    int importance;     // 1-10
    time_t timestamp;
} ContextItem;

// Cấu trúc bộ nhớ ngữ cảnh
typedef struct {
    ContextItem items[MAX_CONTEXT_ITEMS];
    int count;
    char current_topic[128];
    char last_condition[256];
    char last_requirement[256];
    char detected_entities[10][64];
    int entity_count;
} ContextTracker;

// Hàm khởi tạo
void context_tracker_init(ContextTracker *ct);

// Hàm phân tích và lưu
void analyze_context(ContextTracker *ct, const char *input);
void add_context_item(ContextTracker *ct, const char *word, const char *type, const char *context, int importance);

// Hàm truy vấn
char* get_current_topic(ContextTracker *ct);
char* get_context_summary(ContextTracker *ct);
int has_keyword(ContextTracker *ct, const char *keyword);
char* get_last_condition(ContextTracker *ct);
char* get_last_requirement(ContextTracker *ct);

// Hàm nhận diện
int is_context_request(const char *input);
char* answer_context_request(const char *input, ContextTracker *ct);

#endif
