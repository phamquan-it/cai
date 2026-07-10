#ifndef REASONING_NL_H
#define REASONING_NL_H

#define MAX_CAUSES 50
#define MAX_CHAIN 20

// Cấu trúc 1 mối quan hệ nguyên nhân - kết quả
typedef struct {
    char cause[128];
    char effect[128];
    char relation[64];      // "cần", "là", "có", "gây ra", "dẫn đến"
    float confidence;
} CauseEffect;

// Cấu trúc chuỗi suy luận
typedef struct {
    char steps[MAX_CHAIN][256];
    int step_count;
    char conclusion[512];
    char evidence[512];
} ReasoningChain;

// Hàm chính
void load_reasoning_rules();
void add_rule(const char *cause, const char *effect, const char *relation);
ReasoningChain* reason_from_cause(const char *input);
char* reason_natural_language(const char *input);

// Hàm trích xuất
void extract_cause_effect(const char *input, CauseEffect *ce);
void extract_entities(const char *input, char *subject, char *predicate, char *object);

// Hàm kiểm tra
int is_reasoning_nl_request(const char *input);
char* answer_reasoning_nl_request(const char *input);

#endif
