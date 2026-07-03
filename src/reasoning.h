#ifndef REASONING_H
#define REASONING_H

// Cấu trúc công thức
typedef struct {
    char name[64];
    char formula[128];
    char variables[10][32];
    int var_count;
    char description[256];
} Formula;

// Cấu trúc suy luận
typedef struct {
    char target[32];
    char known[10][32];
    int known_count;
    char steps[10][256];
    int step_count;
    char result[256];
    int can_solve;
} ReasoningResult;

// Hàm chính
void load_formulas();
char* solve_problem(const char *query);

// Hàm hỗ trợ
int is_reasoning_request(const char *input);
char* answer_reasoning_request(const char *input);

// Hàm tìm công thức (xuất ra ngoài để test)
Formula* find_formula_with_vars(const char *target, const char **known, int known_count);

#endif
