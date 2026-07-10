#include "rule_learner.h"
#include "reasoning_nl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEARNED_RULES 100

static CauseEffect learned_rules[MAX_LEARNED_RULES];
static int learned_count = 0;

// ============================================
//  KIỂM TRA CÓ CẤU TRÚC HỌC KHÔNG
// ============================================

int has_learn_structure(const char *input) {
    if (!input) return 0;
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) lower[i] = tolower(lower[i]);
    
    // KIỂM TRA TRỰC TIẾP CÁC MẪU
    if (strstr(lower, " cần ") != NULL) return 1;
    if (strstr(lower, "cần ") != NULL && strlen(strstr(lower, "cần ")) > 3) return 1;
    if (strstr(lower, " dẫn đến ") != NULL) return 1;
    if (strstr(lower, "dẫn đến") != NULL) return 1;
    if (strstr(lower, " gây ra ") != NULL) return 1;
    if (strstr(lower, "gây ra") != NULL) return 1;
    if (strstr(lower, "nếu") != NULL && strstr(lower, "thì") != NULL) return 1;
    
    return 0;
}

// ============================================
//  HỌC RULE TỪ CÂU
// ============================================

int learn_rule_from_sentence(const char *sentence) {
    if (!sentence || strlen(sentence) < 5) return 0;
    
    // In ra để debug
    printf("🐛 Đang phân tích: %s\n", sentence);
    
    char lower[512];
    strcpy(lower, sentence);
    for (int i = 0; lower[i]; i++) lower[i] = tolower(lower[i]);
    
    char cause[128] = "", effect[128] = "", relation[64] = "";
    
    // ============================================
    //  PATTERN: "A cần B"
    // ============================================
    if (strstr(lower, "cần") != NULL) {
        char *pos = strstr(lower, "cần");
        if (pos) {
            // Lấy cause (phần trước "cần")
            int len = pos - lower;
            if (len > 0) {
                char temp[128];
                strncpy(temp, lower, len);
                temp[len] = '\0';
                // Xóa khoảng trắng thừa
                char *p = temp;
                while (*p == ' ') p++;
                char *q = temp + strlen(temp) - 1;
                while (q > p && *q == ' ') q--;
                *(q + 1) = '\0';
                if (p != temp) memmove(temp, p, strlen(p) + 1);
                // Xóa từ "học" nếu là từ đầu tiên
                if (strncmp(temp, "học ", 4) == 0) {
                    char *after_hoc = temp + 4;
                    while (*after_hoc == ' ') after_hoc++;
                    if (strlen(after_hoc) > 0) {
                        strcpy(temp, after_hoc);
                    }
                }
                if (strlen(temp) > 0) strcpy(cause, temp);
            }
            
            // Lấy effect (phần sau "cần")
            pos += strlen("cần");
            while (*pos == ' ') pos++;
            char *end = pos + strlen(pos) - 1;
            while (end > pos && (*end == ' ' || *end == '.' || *end == '?' || *end == '!' || *end == ',')) end--;
            *(end + 1) = '\0';
            if (strlen(pos) > 0) strcpy(effect, pos);
            
            strcpy(relation, "cần");
        }
    }
    // ============================================
    //  PATTERN: "A dẫn đến B"
    // ============================================
    else if (strstr(lower, "dẫn đến") != NULL) {
        char *pos = strstr(lower, "dẫn đến");
        if (pos) {
            int len = pos - lower;
            if (len > 0) {
                char temp[128];
                strncpy(temp, lower, len);
                temp[len] = '\0';
                char *p = temp;
                while (*p == ' ') p++;
                char *q = temp + strlen(temp) - 1;
                while (q > p && *q == ' ') q--;
                *(q + 1) = '\0';
                if (p != temp) memmove(temp, p, strlen(p) + 1);
                if (strlen(temp) > 0) strcpy(cause, temp);
            }
            
            pos += strlen("dẫn đến");
            while (*pos == ' ') pos++;
            char *end = pos + strlen(pos) - 1;
            while (end > pos && (*end == ' ' || *end == '.' || *end == '?' || *end == '!')) end--;
            *(end + 1) = '\0';
            if (strlen(pos) > 0) strcpy(effect, pos);
            
            strcpy(relation, "dẫn đến");
        }
    }
    // ============================================
    //  PATTERN: "A gây ra B"
    // ============================================
    else if (strstr(lower, "gây ra") != NULL) {
        char *pos = strstr(lower, "gây ra");
        if (pos) {
            int len = pos - lower;
            if (len > 0) {
                char temp[128];
                strncpy(temp, lower, len);
                temp[len] = '\0';
                char *p = temp;
                while (*p == ' ') p++;
                char *q = temp + strlen(temp) - 1;
                while (q > p && *q == ' ') q--;
                *(q + 1) = '\0';
                if (p != temp) memmove(temp, p, strlen(p) + 1);
                if (strlen(temp) > 0) strcpy(cause, temp);
            }
            
            pos += strlen("gây ra");
            while (*pos == ' ') pos++;
            char *end = pos + strlen(pos) - 1;
            while (end > pos && (*end == ' ' || *end == '.' || *end == '?' || *end == '!')) end--;
            *(end + 1) = '\0';
            if (strlen(pos) > 0) strcpy(effect, pos);
            
            strcpy(relation, "gây ra");
        }
    }
    // ============================================
    //  PATTERN: "Nếu A thì B"
    // ============================================
    else if (strstr(lower, "nếu") != NULL && strstr(lower, "thì") != NULL) {
        char *if_pos = strstr(lower, "nếu");
        char *then_pos = strstr(lower, "thì");
        if (if_pos && then_pos && then_pos > if_pos) {
            if_pos += strlen("nếu");
            while (*if_pos == ' ') if_pos++;
            char *end = then_pos - 1;
            while (end > if_pos && *end == ' ') end--;
            int len = end - if_pos + 1;
            if (len > 0) {
                char temp[128];
                strncpy(temp, if_pos, len);
                temp[len] = '\0';
                char *p = temp;
                while (*p == ' ') p++;
                if (p != temp) memmove(temp, p, strlen(p) + 1);
                if (strlen(temp) > 0) strcpy(cause, temp);
            }
            
            then_pos += strlen("thì");
            while (*then_pos == ' ') then_pos++;
            char *end2 = then_pos + strlen(then_pos) - 1;
            while (end2 > then_pos && (*end2 == ' ' || *end2 == '.' || *end2 == '?' || *end2 == '!')) end2--;
            *(end2 + 1) = '\0';
            if (strlen(then_pos) > 0) strcpy(effect, then_pos);
            
            strcpy(relation, "dẫn đến");
        }
    }
    
    // ============================================
    //  LƯU RULE
    // ============================================
    if (strlen(cause) > 0 && strlen(effect) > 0 && learned_count < MAX_LEARNED_RULES) {
        // Kiểm tra trùng
        for (int i = 0; i < learned_count; i++) {
            if (strcmp(learned_rules[i].cause, cause) == 0 && 
                strcmp(learned_rules[i].effect, effect) == 0) {
                printf("🐛 Rule đã tồn tại: %s %s %s\n", cause, relation, effect);
                return 0;
            }
        }
        
        printf("🐛 Học rule: %s %s %s\n", cause, relation, effect);
        
        strcpy(learned_rules[learned_count].cause, cause);
        strcpy(learned_rules[learned_count].effect, effect);
        strcpy(learned_rules[learned_count].relation, relation);
        learned_rules[learned_count].confidence = 0.85;
        learned_count++;
        
        add_rule(cause, effect, relation);
        return 1;
    }
    
    printf("🐛 Không trích xuất được rule từ: %s\n", sentence);
    return 0;
}

// ============================================
//  LƯU RULE VÀO FILE
// ============================================

int save_learned_rules(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) return 0;
    
    fprintf(file, "# Learned Rules (auto-generated)\n");
    fprintf(file, "# Format: cause|effect|relation\n\n");
    
    for (int i = 0; i < learned_count; i++) {
        fprintf(file, "%s|%s|%s\n", 
                learned_rules[i].cause, 
                learned_rules[i].effect, 
                learned_rules[i].relation);
    }
    
    fclose(file);
    return 1;
}

int load_learned_rules(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[512];
    int loaded = 0;
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        
        char cause[128], effect[128], relation[64];
        if (sscanf(line, "%[^|]|%[^|]|%[^\n]", cause, effect, relation) == 3) {
            char *p = cause;
            while (*p == ' ') p++;
            if (p != cause) memmove(cause, p, strlen(p) + 1);
            p = effect;
            while (*p == ' ') p++;
            if (p != effect) memmove(effect, p, strlen(p) + 1);
            
            if (learned_count < MAX_LEARNED_RULES) {
                strcpy(learned_rules[learned_count].cause, cause);
                strcpy(learned_rules[learned_count].effect, effect);
                strcpy(learned_rules[learned_count].relation, relation);
                learned_rules[learned_count].confidence = 0.85;
                learned_count++;
                add_rule(cause, effect, relation);
                loaded++;
            }
        }
    }
    fclose(file);
    return loaded;
}

char* get_learned_rules_summary() {
    static char summary[2048];
    summary[0] = '\0';
    
    if (learned_count == 0) {
        strcpy(summary, "📭 Chưa học rule nào!\n💡 Hãy nói: 'A cần B' hoặc 'A dẫn đến B'");
        return summary;
    }
    
    snprintf(summary, sizeof(summary), 
             "📚 Đã học %d rule:\n\n", learned_count);
    
    for (int i = 0; i < learned_count && i < 20; i++) {
        char line[128];
        snprintf(line, sizeof(line), "   %d. %s %s %s\n", 
                 i+1, 
                 learned_rules[i].cause, 
                 learned_rules[i].relation, 
                 learned_rules[i].effect);
        strcat(summary, line);
    }
    
    if (learned_count > 20) {
        strcat(summary, "   ... và nhiều hơn nữa!\n");
    }
    
    return summary;
}

// ============================================
//  PHÁT HIỆN YÊU CẦU HỌC - ĐƠN GIẢN NHẤT
// ============================================

int is_learn_request(const char *input) {
    if (!input) return 0;
    
    // LUÔN TRẢ VỀ 1 NẾU CÓ CẤU TRÚC HỌC
    return has_learn_structure(input);
}

char* answer_learn_request(const char *input) {
    static char response[1024];
    response[0] = '\0';
    
    if (!is_learn_request(input)) {
        return NULL;
    }
    
    // Xem danh sách rule đã học
    if (strstr(input, "danh sách") != NULL || 
        strstr(input, "list") != NULL ||
        strstr(input, "đã học") != NULL) {
        return get_learned_rules_summary();
    }
    
    // Thử học rule
    int learned = learn_rule_from_sentence(input);
    
    if (learned) {
        snprintf(response, sizeof(response),
                 "✅ Đã học rule mới!\n"
                 "📚 Tổng số rule đã học: %d\n"
                 "💡 Hãy thử suy luận với từ khóa vừa học!",
                 learned_count);
        save_learned_rules("data/learned_rules.txt");
    } else {
        snprintf(response, sizeof(response),
                 "🤔 Tôi chưa hiểu rule này!\n"
                 "💡 Hãy nói theo mẫu:\n"
                 "   - 'A cần B' (ví dụ: xây nhà cần xi măng)\n"
                 "   - 'A dẫn đến B' (ví dụ: học tập dẫn đến thành công)\n"
                 "   - 'A gây ra B' (ví dụ: trời mưa gây ra đường ướt)\n"
                 "   - 'Nếu A thì B' (ví dụ: nếu trời mưa thì đường ướt)");
    }
    
    return response;
}
