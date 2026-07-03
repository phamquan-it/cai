#ifndef THOUGHT_PROCESSOR_H
#define THOUGHT_PROCESSOR_H

// Cấu trúc 1 bước suy nghĩ
typedef struct {
    char step[256];
    char action[64];
    char condition[128];
    char expected_result[128];
    int confidence;
} ThoughtStep;

// Cấu trúc kế hoạch suy nghĩ
typedef struct {
    ThoughtStep steps[20];
    int step_count;
    char goal[256];
    char constraints[512];
    char final_answer[512];
    int confidence_level;
    char errors[10][256];
    int error_count;
} ThoughtPlan;

// Hàm chính
ThoughtPlan* create_thought_plan(const char *input);
void free_thought_plan(ThoughtPlan *tp);

// Hàm phân tích và suy nghĩ
char* process_thought(const char *input);

// Hàm kiểm tra lỗi
int validate_thought(ThoughtPlan *tp);
char* get_thought_errors(ThoughtPlan *tp);

#endif
