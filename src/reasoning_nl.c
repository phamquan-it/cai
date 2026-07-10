#include "reasoning_nl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_RULES 300

static CauseEffect rules[MAX_RULES];
static int rule_count = 0;

// ============================================
//  HÀM THÊM RULE
// ============================================

void add_rule(const char *cause, const char *effect, const char *relation) {
    if (rule_count >= MAX_RULES) return;
    strcpy(rules[rule_count].cause, cause);
    strcpy(rules[rule_count].effect, effect);
    strcpy(rules[rule_count].relation, relation);
    rules[rule_count].confidence = 0.9;
    rule_count++;
}

// ============================================
//  ĐỌC RULE TỪ FILE
// ============================================

void load_rules_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("⚠️  Không tìm thấy file rules: %s\n", filename);
        return;
    }
    
    char line[512];
    int loaded = 0;
    
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;
        
        char cause[128], effect[128], relation[64];
        if (sscanf(line, "%[^|]|%[^|]|%[^\n]", cause, effect, relation) == 3) {
            char *p = cause;
            while (*p == ' ') p++;
            char *q = cause + strlen(cause) - 1;
            while (q > p && *q == ' ') q--;
            *(q + 1) = '\0';
            if (p != cause) memmove(cause, p, strlen(p) + 1);
            
            p = effect;
            while (*p == ' ') p++;
            q = effect + strlen(effect) - 1;
            while (q > p && *q == ' ') q--;
            *(q + 1) = '\0';
            if (p != effect) memmove(effect, p, strlen(p) + 1);
            
            add_rule(cause, effect, relation);
            loaded++;
        }
    }
    fclose(file);
    printf("📚 Đã tải %d rule từ %s\n", loaded, filename);
}

// ============================================
//  KHỞI TẠO RULES
// ============================================

void load_reasoning_rules() {
    if (rule_count > 0) return;
    
    load_rules_from_file("data/rules.txt");
    load_rules_from_file("data/learned_rules.txt");
    
    if (rule_count == 0) {
        printf("📚 Sử dụng rule mặc định\n");
        add_rule("xây nhà", "cần xi măng", "cần");
        add_rule("bê tông", "cần xi măng", "cần");
        add_rule("trời mưa", "đường ướt", "gây ra");
        add_rule("học tập", "có kiến thức", "dẫn đến");
    }
}

// ============================================
//  TRÍCH XUẤT THỰC THỂ
// ============================================

void extract_entities(const char *input, char *subject, char *predicate, char *object) {
    subject[0] = '\0';
    predicate[0] = '\0';
    object[0] = '\0';
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) lower[i] = tolower(lower[i]);
    
    const char *verbs[] = {"cần", "là", "có", "gây ra", "dẫn đến", "thực hiện", "tạo ra", "giúp"};
    
    for (int i = 0; i < 8; i++) {
        char *pos = strstr(lower, verbs[i]);
        if (pos) {
            int len = pos - lower;
            if (len > 0) {
                char temp[128];
                strncpy(temp, lower, len);
                temp[len] = '\0';
                char *last = strrchr(temp, ' ');
                if (last) {
                    strcpy(subject, last + 1);
                } else {
                    strcpy(subject, temp);
                }
            }
            strcpy(predicate, verbs[i]);
            pos += strlen(verbs[i]);
            while (*pos == ' ') pos++;
            int j = 0;
            while (pos[j] != ' ' && pos[j] != '\0' && pos[j] != '.' && pos[j] != '?' && j < 127) {
                object[j] = pos[j];
                j++;
            }
            object[j] = '\0';
            if (strlen(subject) > 0 && strlen(object) > 0) {
                return;
            }
        }
    }
    
    char *last = strrchr(lower, ' ');
    if (last) {
        strcpy(object, last + 1);
    } else {
        strcpy(object, lower);
    }
    strcpy(subject, "điều này");
    strcpy(predicate, "liên quan đến");
}

// ============================================
//  TÌM RULE THEO CAUSE
// ============================================

int find_rules_by_cause(const char *cause, CauseEffect *results, int max_results) {
    int found = 0;
    for (int i = 0; i < rule_count && found < max_results; i++) {
        if (strcmp(rules[i].cause, cause) == 0) {
            results[found] = rules[i];
            found++;
        }
    }
    return found;
}

// ============================================
//  SUY LUẬN CHUỖI
// ============================================

ReasoningChain* reason_from_cause(const char *input) {
    static ReasoningChain chain;
    memset(&chain, 0, sizeof(ReasoningChain));
    
    // Trích xuất cause từ input
    char cause[128];
    char subject[128], predicate[64], object[128];
    extract_entities(input, subject, predicate, object);
    
    // Ưu tiên subject làm cause
    if (strlen(subject) > 0) {
        strcpy(cause, subject);
    } else {
        strcpy(cause, input);
    }
    
    // Tìm tất cả rule có cause trùng
    CauseEffect matches[20];
    int match_count = find_rules_by_cause(cause, matches, 20);
    
    if (match_count == 0) {
        // Không tìm thấy rule
        snprintf(chain.conclusion, sizeof(chain.conclusion),
                 "🤔 Tôi chưa có thông tin về '%s'.\n"
                 "💡 Hãy dạy tôi: '%s cần ...' hoặc '%s dẫn đến ...'",
                 cause, cause, cause);
        return &chain;
    }
    
    // Suy luận theo từng chain
    for (int m = 0; m < match_count && chain.step_count < MAX_CHAIN; m++) {
        char step[256];
        snprintf(step, sizeof(step), "📌 %s %s %s", 
                 matches[m].cause, matches[m].relation, matches[m].effect);
        strcpy(chain.steps[chain.step_count], step);
        chain.step_count++;
        
        // Tìm tiếp theo
        char current[128];
        strcpy(current, matches[m].effect);
        int max_depth = 10;
        
        while (max_depth-- > 0 && chain.step_count < MAX_CHAIN) {
            CauseEffect next_matches[10];
            int next_count = find_rules_by_cause(current, next_matches, 10);
            if (next_count == 0) break;
            
            char next[256];
            snprintf(next, sizeof(next), "   └─ %s %s %s", 
                     next_matches[0].cause, next_matches[0].relation, next_matches[0].effect);
            strcpy(chain.steps[chain.step_count], next);
            chain.step_count++;
            strcpy(current, next_matches[0].effect);
        }
    }
    
    // Kết luận
    if (chain.step_count > 0) {
        // Lấy effect cuối cùng
        char last_effect[128];
        for (int i = chain.step_count - 1; i >= 0; i--) {
            // Tìm effect trong step cuối
            char *effect_pos = strstr(chain.steps[i], "→");
            if (!effect_pos) effect_pos = strstr(chain.steps[i], ">");
            if (effect_pos) {
                effect_pos += 1;
                while (*effect_pos == ' ') effect_pos++;
                char temp[128];
                strcpy(temp, effect_pos);
                // Tách lấy effect
                char *p = temp;
                while (*p && *p != ' ') p++;
                *p = '\0';
                strcpy(last_effect, temp);
                break;
            }
        }
        if (strlen(last_effect) == 0) {
            strcpy(last_effect, "kết quả");
        }
        snprintf(chain.conclusion, sizeof(chain.conclusion), 
                 "✅ Kết luận: %s → %s", cause, last_effect);
        snprintf(chain.evidence, sizeof(chain.evidence), 
                 "📝 Dựa trên chuỗi suy luận %d bước", chain.step_count);
    } else {
        snprintf(chain.conclusion, sizeof(chain.conclusion), 
                 "❌ Không thể suy luận từ '%s'", cause);
    }
    
    return &chain;
}

// ============================================
//  HÀM CHÍNH
// ============================================

char* reason_natural_language(const char *input) {
    static char response[4096];
    response[0] = '\0';
    
    if (!input) return NULL;
    
    ReasoningChain *chain = reason_from_cause(input);
    if (!chain) return NULL;
    
    strcat(response, "🧠 SUY LUẬN NHÂN QUẢ ĐA CẤP:\n\n");
    
    if (chain->step_count > 0) {
        for (int i = 0; i < chain->step_count && i < MAX_CHAIN; i++) {
            strcat(response, chain->steps[i]);
            strcat(response, "\n");
        }
        strcat(response, "\n");
        strcat(response, chain->conclusion);
        strcat(response, "\n");
        strcat(response, chain->evidence);
    } else {
        strcat(response, chain->conclusion);
    }
    
    return response;
}

// ============================================
//  PHÁT HIỆN YÊU CẦU
// ============================================

int is_reasoning_nl_request(const char *input) {
    if (!input) return 0;
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) lower[i] = tolower(lower[i]);
    
    // Chỉ nhận diện khi câu hỏi ngắn hoặc có từ khóa
    if (strlen(lower) < 30) {
        return 1;
    }
    
    const char *keywords[] = {
        "suy luận", "nguyên nhân", "kết quả", "vì sao", 
        "tại sao", "dẫn đến", "gây ra", "cần", "có", "là"
    };
    
    for (int i = 0; i < 10; i++) {
        if (strstr(lower, keywords[i]) != NULL) {
            return 1;
        }
    }
    return 0;
}

char* answer_reasoning_nl_request(const char *input) {
    if (!is_reasoning_nl_request(input)) {
        return NULL;
    }
    
    return reason_natural_language(input);
}
