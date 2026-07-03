#include "context_tracker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// ============================================
//  KHỞI TẠO
// ============================================

void context_tracker_init(ContextTracker *ct) {
    ct->count = 0;
    ct->entity_count = 0;
    strcpy(ct->current_topic, "general");
    strcpy(ct->last_condition, "");
    strcpy(ct->last_requirement, "");
    for (int i = 0; i < MAX_CONTEXT_ITEMS; i++) {
        ct->items[i].word[0] = '\0';
        ct->items[i].type[0] = '\0';
        ct->items[i].context[0] = '\0';
        ct->items[i].importance = 0;
        ct->items[i].timestamp = time(NULL);
    }
    for (int i = 0; i < 10; i++) {
        ct->detected_entities[i][0] = '\0';
    }
}

void add_context_item(ContextTracker *ct, const char *word, const char *type, const char *context, int importance) {
    if (ct->count >= MAX_CONTEXT_ITEMS) return;
    
    // Kiểm tra trùng lặp
    for (int i = 0; i < ct->count; i++) {
        if (strcmp(ct->items[i].word, word) == 0) {
            // Cập nhật nếu đã tồn tại
            strcpy(ct->items[i].context, context);
            ct->items[i].importance = importance;
            ct->items[i].timestamp = time(NULL);
            return;
        }
    }
    
    strcpy(ct->items[ct->count].word, word);
    strcpy(ct->items[ct->count].type, type);
    strcpy(ct->items[ct->count].context, context);
    ct->items[ct->count].importance = importance;
    ct->items[ct->count].timestamp = time(NULL);
    ct->count++;
    
    // Nếu là topic, cập nhật current_topic
    if (strcmp(type, "topic") == 0 && importance > 7) {
        strcpy(ct->current_topic, word);
    }
    
    // Nếu là condition hoặc requirement
    if (strcmp(type, "condition") == 0) {
        strcpy(ct->last_condition, context);
    }
    if (strcmp(type, "requirement") == 0) {
        strcpy(ct->last_requirement, context);
    }
}

// ============================================
//  PHÂN TÍCH NGỮ CẢNH
// ============================================

void analyze_context(ContextTracker *ct, const char *input) {
    if (!input) return;
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    // ========================================
    //  PHÁT HIỆN TỪ KHÓA ĐÁNG CHÚ Ý
    // ========================================
    
    // 1. Điều kiện (nếu ... thì, với điều kiện)
    if (strstr(lower, "nếu") != NULL || strstr(lower, "điều kiện") != NULL || 
        strstr(lower, "khi") != NULL || strstr(lower, "với điều kiện") != NULL) {
        char context[256];
        snprintf(context, sizeof(context), "Điều kiện: %s", input);
        add_context_item(ct, "dieu_kien", "condition", context, 8);
    }
    
    // 2. Yêu cầu (cần, muốn, yêu cầu, mong muốn)
    if (strstr(lower, "cần") != NULL || strstr(lower, "muốn") != NULL || 
        strstr(lower, "yêu cầu") != NULL || strstr(lower, "mong muốn") != NULL) {
        char context[256];
        snprintf(context, sizeof(context), "Yêu cầu: %s", input);
        add_context_item(ct, "yeu_cau", "requirement", context, 9);
    }
    
    // 3. Từ khóa quan trọng (dùng regex đơn giản)
    const char *important_words[] = {
        "quan trọng", "cần thiết", "bắt buộc", "ưu tiên",
        "không được", "cấm", "được phép", "nên"
    };
    for (int i = 0; i < 8; i++) {
        if (strstr(lower, important_words[i]) != NULL) {
            char context[256];
            snprintf(context, sizeof(context), "Từ khóa quan trọng: %s trong '%s'", important_words[i], input);
            add_context_item(ct, important_words[i], "keyword", context, 6);
        }
    }
    
    // 4. Chủ đề (về, chủ đề, nói về)
    if (strstr(lower, "về") != NULL || strstr(lower, "chủ đề") != NULL || 
        strstr(lower, "nói về") != NULL) {
        char topic[64] = "";
        char *ve = strstr(lower, "về");
        if (ve) {
            ve += 2;
            while (*ve == ' ') ve++;
            int j = 0;
            while (ve[j] != ' ' && ve[j] != '\0' && ve[j] != '.' && ve[j] != '?' && j < 63) {
                topic[j] = ve[j];
                j++;
            }
            topic[j] = '\0';
            if (strlen(topic) > 0 && strlen(topic) < 50) {
                char context[256];
                snprintf(context, sizeof(context), "Chủ đề: %s", topic);
                add_context_item(ct, topic, "topic", context, 7);
            }
        }
    }
    
    // 5. Thực thể (tên riêng, tên gọi)
    // Phát hiện từ viết hoa trong input (có thể là tên riêng)
    char temp[512];
    strcpy(temp, input);
    char *token = strtok(temp, " .,!?;:\n\t");
    while (token != NULL) {
        if (strlen(token) > 1 && isupper(token[0])) {
            // Kiểm tra không phải đầu câu
            int add = 0;
            // Kiểm tra xem đã có chưa
            for (int i = 0; i < ct->entity_count; i++) {
                if (strcmp(ct->detected_entities[i], token) == 0) {
                    add = 1;
                    break;
                }
            }
            if (!add && ct->entity_count < 10) {
                strcpy(ct->detected_entities[ct->entity_count], token);
                ct->entity_count++;
                char context[256];
                snprintf(context, sizeof(context), "Thực thể: %s", token);
                add_context_item(ct, token, "entity", context, 5);
            }
        }
        token = strtok(NULL, " .,!?;:\n\t");
    }
}

// ============================================
//  HÀM TRUY VẤN
// ============================================

char* get_current_topic(ContextTracker *ct) {
    static char result[128];
    if (strlen(ct->current_topic) > 0 && strcmp(ct->current_topic, "general") != 0) {
        snprintf(result, sizeof(result), "📌 Chủ đề hiện tại: %s", ct->current_topic);
    } else {
        // Tìm topic mới nhất
        for (int i = ct->count - 1; i >= 0; i--) {
            if (strcmp(ct->items[i].type, "topic") == 0) {
                snprintf(result, sizeof(result), "📌 Chủ đề hiện tại: %s", ct->items[i].word);
                return result;
            }
        }
        strcpy(result, "📌 Chưa có chủ đề cụ thể");
    }
    return result;
}

char* get_context_summary(ContextTracker *ct) {
    static char summary[2048];
    summary[0] = '\0';
    
    if (ct->count == 0) {
        strcpy(summary, "📭 Chưa có thông tin ngữ cảnh nào!");
        return summary;
    }
    
    strcat(summary, "🧠 TỔNG KẾT NGỮ CẢNH:\n\n");
    
    // Topic hiện tại
    char *topic = get_current_topic(ct);
    strcat(summary, topic);
    strcat(summary, "\n\n");
    
    // Điều kiện gần nhất
    if (strlen(ct->last_condition) > 0) {
        strcat(summary, "⚡ Điều kiện: ");
        strcat(summary, ct->last_condition);
        strcat(summary, "\n");
    }
    
    // Yêu cầu gần nhất
    if (strlen(ct->last_requirement) > 0) {
        strcat(summary, "🎯 Yêu cầu: ");
        strcat(summary, ct->last_requirement);
        strcat(summary, "\n");
    }
    
    // Các thực thể
    if (ct->entity_count > 0) {
        strcat(summary, "\n👤 Thực thể:\n");
        for (int i = 0; i < ct->entity_count; i++) {
            if (strlen(ct->detected_entities[i]) > 0) {
                char line[128];
                snprintf(line, sizeof(line), "   - %s\n", ct->detected_entities[i]);
                strcat(summary, line);
            }
        }
    }
    
    // Các từ khóa quan trọng
    int keyword_count = 0;
    strcat(summary, "\n🔑 Từ khóa quan trọng:\n");
    for (int i = 0; i < ct->count; i++) {
        if (strcmp(ct->items[i].type, "keyword") == 0 && ct->items[i].importance >= 6) {
            char line[128];
            snprintf(line, sizeof(line), "   - %s: %s\n", ct->items[i].word, ct->items[i].context);
            strcat(summary, line);
            keyword_count++;
        }
    }
    if (keyword_count == 0) {
        strcat(summary, "   (Chưa có từ khóa đáng chú ý)\n");
    }
    
    return summary;
}

int has_keyword(ContextTracker *ct, const char *keyword) {
    for (int i = 0; i < ct->count; i++) {
        if (strcasestr(ct->items[i].word, keyword) != NULL) {
            return 1;
        }
    }
    return 0;
}

char* get_last_condition(ContextTracker *ct) {
    static char result[256];
    if (strlen(ct->last_condition) > 0) {
        snprintf(result, sizeof(result), "⚡ Điều kiện cuối: %s", ct->last_condition);
    } else {
        strcpy(result, "⚡ Chưa có điều kiện nào");
    }
    return result;
}

char* get_last_requirement(ContextTracker *ct) {
    static char result[256];
    if (strlen(ct->last_requirement) > 0) {
        snprintf(result, sizeof(result), "🎯 Yêu cầu cuối: %s", ct->last_requirement);
    } else {
        strcpy(result, "🎯 Chưa có yêu cầu nào");
    }
    return result;
}

// ============================================
//  PHÁT HIỆN YÊU CẦU VỀ NGỮ CẢNH
// ============================================

int is_context_request(const char *input) {
    if (!input) return 0;
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    const char *keywords[] = {
        "ngữ cảnh", "context", "tóm tắt", "tổng kết",
        "chủ đề", "điều kiện", "yêu cầu"
    };
    
    for (int i = 0; i < 7; i++) {
        if (strstr(lower, keywords[i]) != NULL) {
            return 1;
        }
    }
    return 0;
}

char* answer_context_request(const char *input, ContextTracker *ct) {
    static char response[2048];
    response[0] = '\0';
    
    if (!is_context_request(input)) {
        return NULL;
    }
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    if (strstr(lower, "tóm tắt") != NULL || strstr(lower, "tổng kết") != NULL) {
        strcpy(response, get_context_summary(ct));
    } else if (strstr(lower, "chủ đề") != NULL) {
        char *topic = get_current_topic(ct);
        strcpy(response, topic);
    } else if (strstr(lower, "điều kiện") != NULL) {
        char *cond = get_last_condition(ct);
        strcpy(response, cond);
    } else if (strstr(lower, "yêu cầu") != NULL) {
        char *req = get_last_requirement(ct);
        strcpy(response, req);
    } else {
        strcpy(response, get_context_summary(ct));
    }
    
    return response;
}
