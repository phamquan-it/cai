#include "reasoning.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_FORMULAS 50
#define MAX_VARS 10
#define MAX_DEPTH 5

static Formula formulas[MAX_FORMULAS];
static int formula_count = 0;

// ============================================
//  KHO CÔNG THỨC (giữ nguyên)
// ============================================

void add_formula(const char *name, const char *formula, 
                 const char *vars, const char *desc) {
    if (formula_count >= MAX_FORMULAS) return;
    strcpy(formulas[formula_count].name, name);
    strcpy(formulas[formula_count].formula, formula);
    strcpy(formulas[formula_count].description, desc);
    formulas[formula_count].var_count = 0;
    char temp[128];
    strcpy(temp, vars);
    char *token = strtok(temp, ",");
    while (token != NULL && formulas[formula_count].var_count < MAX_VARS) {
        while (*token == ' ') token++;
        strcpy(formulas[formula_count].variables[formulas[formula_count].var_count], token);
        formulas[formula_count].var_count++;
        token = strtok(NULL, ",");
    }
    formula_count++;
}

static void init_formulas() {
    if (formula_count > 0) return;
    
    add_formula("Tổng 2 số", "c = a + b", "a,b,c", "Tính tổng của a và b");
    add_formula("Hiệu 2 số", "a = d - t", "d,t,a", "Tính hiệu của d và t");
    add_formula("Tích 2 số", "c = a * b", "a,b,c", "Tính tích của a và b");
    add_formula("Thương 2 số", "c = a / b", "a,b,c", "Tính thương của a và b");
    add_formula("Chu vi hình chữ nhật", "p = 2 * (a + b)", "a,b,p", "Chu vi = 2*(dài+rộng)");
    add_formula("Diện tích hình chữ nhật", "s = a * b", "a,b,s", "Diện tích = dài*rộng");
    add_formula("Vận tốc", "v = s / t", "s,t,v", "Vận tốc = quãng đường / thời gian");
    add_formula("Quãng đường", "s = v * t", "v,t,s", "Quãng đường = vận tốc * thời gian");
    add_formula("Thời gian", "t = s / v", "s,v,t", "Thời gian = quãng đường / vận tốc");
    add_formula("Lực", "f = m * g", "m,g,f", "Lực = khối lượng * gia tốc");
    add_formula("Công", "a = f * s", "f,s,a", "Công = lực * quãng đường");
    add_formula("Nhiệt lượng", "q = m * c * dt", "m,c,dt,q", "Nhiệt lượng = m*c*ΔT");
    add_formula("Áp suất", "p = f / s", "f,s,p", "Áp suất = lực / diện tích");
    add_formula("Lợi nhuận", "l = r - c", "r,c,l", "Lợi nhuận = doanh thu - chi phí");
    add_formula("Doanh thu", "r = p * q", "p,q,r", "Doanh thu = giá bán * số lượng");
    add_formula("Diện tích hình tam giác", "s = (a * h) / 2", "a,h,s", "Diện tích = (đáy*cao)/2");
}

void load_formulas() {
    init_formulas();
}

// ============================================
//  HÀM TÌM CÔNG THỨC
// ============================================

Formula* find_formula_with_vars(const char *target, const char **known, int known_count) {
    for (int i = 0; i < formula_count; i++) {
        int has_target = 0;
        for (int j = 0; j < formulas[i].var_count; j++) {
            if (strcmp(formulas[i].variables[j], target) == 0) {
                has_target = 1;
                break;
            }
        }
        if (!has_target) continue;
        
        int all_known = 1;
        for (int j = 0; j < formulas[i].var_count; j++) {
            if (strcmp(formulas[i].variables[j], target) == 0) continue;
            int found = 0;
            for (int k = 0; k < known_count; k++) {
                if (strcmp(formulas[i].variables[j], known[k]) == 0) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                all_known = 0;
                break;
            }
        }
        if (all_known) {
            return &formulas[i];
        }
    }
    return NULL;
}

// ============================================
//  PARSE CÂU HỎI - QUAN TRỌNG
// ============================================

static void parse_problem(const char *input, char *target, char known[10][32], int *known_count) {
    target[0] = '\0';
    *known_count = 0;
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    // 1. Tìm biến mục tiêu (sau "tìm", "cần tìm", "tính")
    const char *target_patterns[] = {"tìm", "cần tìm", "tính"};
    for (int i = 0; i < 3; i++) {
        char *pos = strstr(lower, target_patterns[i]);
        if (pos) {
            pos += strlen(target_patterns[i]);
            while (*pos == ' ') pos++;
            int j = 0;
            while (pos[j] != ' ' && pos[j] != '\0' && pos[j] != ',' && pos[j] != '.' && pos[j] != '?' && j < 31) {
                target[j] = pos[j];
                j++;
            }
            target[j] = '\0';
            if (strlen(target) > 0) break;
        }
    }
    
    // 2. Tìm các biến đã biết (sau "biết", "có", "với")
    const char *known_patterns[] = {"biết", "có", "với"};
    for (int i = 0; i < 3; i++) {
        char *pos = strstr(lower, known_patterns[i]);
        if (pos) {
            pos += strlen(known_patterns[i]);
            char token[512];
            strcpy(token, pos);
            
            char *tok = strtok(token, " ,.?!");
            while (tok != NULL && *known_count < 10) {
                // Loại bỏ dấu câu và khoảng trắng
                char clean[32];
                int idx = 0;
                for (int j = 0; tok[j]; j++) {
                    if (isalpha(tok[j])) {
                        clean[idx++] = tok[j];
                    }
                }
                clean[idx] = '\0';
                if (strlen(clean) > 0 && strcmp(clean, "khi") != 0 && strcmp(clean, "và") != 0) {
                    strcpy(known[*known_count], clean);
                    (*known_count)++;
                }
                tok = strtok(NULL, " ,.?!");
            }
        }
    }
    
    // 3. Nếu không tìm thấy "biết", lấy các biến trong câu sau "khi"
    if (*known_count == 0) {
        char *khi = strstr(lower, "khi");
        if (khi) {
            khi += 3;
            char token[512];
            strcpy(token, khi);
            char *tok = strtok(token, " ,.?!");
            while (tok != NULL && *known_count < 10) {
                char clean[32];
                int idx = 0;
                for (int j = 0; tok[j]; j++) {
                    if (isalpha(tok[j])) {
                        clean[idx++] = tok[j];
                    }
                }
                clean[idx] = '\0';
                if (strlen(clean) > 0 && strcmp(clean, "và") != 0) {
                    strcpy(known[*known_count], clean);
                    (*known_count)++;
                }
                tok = strtok(NULL, " ,.?!");
            }
        }
    }
    
    // 4. Nếu vẫn không tìm thấy, dùng mặc định
    if (*known_count == 0) {
        strcpy(known[0], "a");
        strcpy(known[1], "b");
        *known_count = 2;
    }
    
    if (strlen(target) == 0) {
        strcpy(target, "c");
    }
}

// ============================================
//  SUY LUẬN ĐỆ QUY (giữ nguyên)
// ============================================

int recursive_reasoning(const char *target, const char **known, int known_count, 
                        int depth, ReasoningResult *result, int visited[]) {
    if (depth > MAX_DEPTH) return 0;
    
    for (int i = 0; i < known_count; i++) {
        if (strcmp(target, known[i]) == 0) {
            snprintf(result->result, sizeof(result->result), 
                     "✅ '%s' đã có trong dữ liệu!", target);
            return 1;
        }
    }
    
    Formula *f = find_formula_with_vars(target, known, known_count);
    if (f) {
        char step[256];
        snprintf(step, sizeof(step), "Cấp %d: %s → %s", depth, f->name, f->formula);
        strcpy(result->steps[result->step_count], step);
        result->step_count++;
        strcpy(result->result, f->formula);
        return 1;
    }
    
    for (int i = 0; i < formula_count; i++) {
        int can_calc = 1;
        char new_var[32] = "";
        int new_var_found = 0;
        
        for (int j = 0; j < formulas[i].var_count; j++) {
            int found = 0;
            for (int k = 0; k < known_count; k++) {
                if (strcmp(formulas[i].variables[j], known[k]) == 0) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                if (new_var_found) {
                    can_calc = 0;
                    break;
                }
                strcpy(new_var, formulas[i].variables[j]);
                new_var_found = 1;
            }
        }
        
        if (can_calc && new_var_found) {
            int already_visited = 0;
            for (int v = 0; v < depth; v++) {
                if (visited[v] == i) {
                    already_visited = 1;
                    break;
                }
            }
            if (already_visited) continue;
            
            char step[256];
            snprintf(step, sizeof(step), "Cấp %d: %s → Tính %s = %s", 
                     depth, formulas[i].name, new_var, formulas[i].formula);
            
            const char *new_known[20];
            for (int k = 0; k < known_count; k++) {
                new_known[k] = known[k];
            }
            new_known[known_count] = new_var;
            visited[depth] = i;
            
            ReasoningResult sub_result;
            memset(&sub_result, 0, sizeof(ReasoningResult));
            
            if (recursive_reasoning(target, new_known, known_count + 1, 
                                    depth + 1, &sub_result, visited)) {
                strcpy(result->steps[result->step_count], step);
                result->step_count++;
                for (int s = 0; s < sub_result.step_count; s++) {
                    strcpy(result->steps[result->step_count], sub_result.steps[s]);
                    result->step_count++;
                }
                strcpy(result->result, sub_result.result);
                return 1;
            }
        }
    }
    return 0;
}

// ============================================
//  HÀM CHÍNH
// ============================================

char* solve_problem(const char *query) {
    static char result[4096];
    result[0] = '\0';
    
    char target[32];
    char known[10][32];
    int known_count = 0;
    
    parse_problem(query, target, known, &known_count);
    
    ReasoningResult reasoning_result;
    memset(&reasoning_result, 0, sizeof(ReasoningResult));
    
    int visited[MAX_DEPTH] = {0};
    const char *known_ptrs[10];
    for (int i = 0; i < known_count; i++) {
        known_ptrs[i] = known[i];
    }
    
    int success = recursive_reasoning(target, known_ptrs, known_count, 
                                      1, &reasoning_result, visited);
    
    snprintf(result, sizeof(result),
             "🧠 SUY LUẬN ĐỆ QUY (tối đa %d cấp):\n\n"
             "🎯 Mục tiêu: Tìm '%s'\n"
             "📊 Dữ liệu đã có: ",
             MAX_DEPTH, target);
    
    for (int i = 0; i < known_count; i++) {
        strcat(result, known[i]);
        if (i < known_count - 1) strcat(result, ", ");
    }
    strcat(result, "\n\n");
    
    if (success) {
        strcat(result, "📋 Các bước suy luận:\n");
        for (int i = 0; i < reasoning_result.step_count; i++) {
            char step[256];
            snprintf(step, sizeof(step), "   %d. %s\n", i+1, reasoning_result.steps[i]);
            strcat(result, step);
        }
        strcat(result, "\n✅ Kết luận:\n");
        strcat(result, "   ");
        strcat(result, reasoning_result.result);
        strcat(result, "\n");
    } else {
        strcat(result, "❌ Không tìm thấy cách giải trong kho công thức!\n");
        strcat(result, "💡 Cần thêm công thức hoặc dữ liệu.\n");
    }
    
    return result;
}

// ============================================
//  PHÁT HIỆN YÊU CẦU
// ============================================

int is_reasoning_request(const char *input) {
    if (!input) return 0;
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) lower[i] = tolower(lower[i]);
    
    const char *keywords[] = {"suy luận", "tìm", "biết", "tính", "giải", "công thức"};
    for (int i = 0; i < 6; i++) {
        if (strstr(lower, keywords[i]) != NULL) return 1;
    }
    return 0;
}

char* answer_reasoning_request(const char *input) {
    if (!is_reasoning_request(input)) return NULL;
    return solve_problem(input);
}
