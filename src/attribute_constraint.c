#include "attribute_constraint.h"
#include "reasoning.h"  // THÊM DÒNG NÀY
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ============================================
//  KHO THỰC THỂ + THUỘC TÍNH
// ============================================

// Bò
static const char *cow_attrs[] = {"sữa", "thịt", "đen", "trắng", "vàng", "nâu"};
// Xe
static const char *car_attrs[] = {"đỏ", "xanh", "trắng", "đen", "mới", "cũ", "toyota", "honda"};
// Nhà
static const char *house_attrs[] = {"đỏ", "vàng", "cao", "thấp", "mới", "cũ", "lớn", "nhỏ"};
// Người
static const char *person_attrs[] = {"cao", "thấp", "béo", "ốm", "trẻ", "già", "đẹp", "xấu"};
// Chó
static const char *dog_attrs[] = {"đen", "vàng", "trắng", "nhỏ", "to", "dữ", "hiền", "nuôi"};

// ============================================
//  HÀM KIỂM TRA THUỘC TÍNH
// ============================================

int is_attribute(const char *word) {
    if (!word) return 0;
    
    const char **attr_lists[] = {cow_attrs, car_attrs, house_attrs, person_attrs, dog_attrs};
    int list_sizes[] = {
        sizeof(cow_attrs)/sizeof(cow_attrs[0]),
        sizeof(car_attrs)/sizeof(car_attrs[0]),
        sizeof(house_attrs)/sizeof(house_attrs[0]),
        sizeof(person_attrs)/sizeof(person_attrs[0]),
        sizeof(dog_attrs)/sizeof(dog_attrs[0])
    };
    
    for (int list = 0; list < 5; list++) {
        for (int i = 0; i < list_sizes[list]; i++) {
            if (strcmp(word, attr_lists[list][i]) == 0) {
                return 1;
            }
        }
    }
    return 0;
}

int is_entity(const char *word) {
    if (!word) return 0;
    const char *entities[] = {"bò", "xe", "nhà", "người", "chó", "mèo", "gà", "vịt"};
    for (int i = 0; i < 8; i++) {
        if (strcmp(word, entities[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// ============================================
//  HÀM TRÍCH XUẤT RÀNG BUỘC
// ============================================

ConstraintContext* extract_constraints(const char *input) {
    if (!input) return NULL;
    
    ConstraintContext *cc = (ConstraintContext*)malloc(sizeof(ConstraintContext));
    if (!cc) return NULL;
    
    memset(cc, 0, sizeof(ConstraintContext));
    strcpy(cc->action, "unknown");
    strcpy(cc->object, "unknown");
    strcpy(cc->condition, "");
    
    char temp[512];
    strcpy(temp, input);
    
    // Tách từ
    char *token = strtok(temp, " .,!?;:\n\t\"'()[]{}");
    EntityWithAttributes current_entity;
    memset(&current_entity, 0, sizeof(EntityWithAttributes));
    int have_entity = 0;
    
    while (token != NULL) {
        char word[64];
        strcpy(word, token);
        for (int i = 0; word[i]; i++) {
            word[i] = tolower(word[i]);
        }
        
        // Kiểm tra từ là "con", "chiếc", "cái" - có thể là bắt đầu của entity
        if (strcmp(word, "con") == 0 || strcmp(word, "chiếc") == 0 || strcmp(word, "cái") == 0) {
            token = strtok(NULL, " .,!?;:\n\t\"'()[]{}");
            if (token) {
                strcpy(word, token);
                for (int i = 0; word[i]; i++) word[i] = tolower(word[i]);
                if (is_entity(word)) {
                    if (have_entity) {
                        // Lưu entity cũ
                        if (cc->subject_count < 10) {
                            cc->subjects[cc->subject_count] = current_entity;
                            cc->subject_count++;
                        }
                    }
                    memset(&current_entity, 0, sizeof(EntityWithAttributes));
                    strcpy(current_entity.entity, word);
                    have_entity = 1;
                    token = strtok(NULL, " .,!?;:\n\t\"'()[]{}");
                    continue;
                }
            }
        }
        
        // Nếu là entity (bò, xe, nhà...)
        if (is_entity(word)) {
            if (have_entity) {
                if (cc->subject_count < 10) {
                    cc->subjects[cc->subject_count] = current_entity;
                    cc->subject_count++;
                }
            }
            memset(&current_entity, 0, sizeof(EntityWithAttributes));
            strcpy(current_entity.entity, word);
            have_entity = 1;
        }
        // Nếu là thuộc tính và đang có entity
        else if (have_entity && is_attribute(word)) {
            if (current_entity.attr_count < 5) {
                strcpy(current_entity.attributes[current_entity.attr_count], word);
                current_entity.attr_count++;
            }
        }
        // Lưu action (động từ)
        else if (strstr(word, "ăn") != NULL || strstr(word, "uống") != NULL ||
                 strstr(word, "chạy") != NULL || strstr(word, "đi") != NULL ||
                 strstr(word, "làm") != NULL || strstr(word, "tạo") != NULL) {
            strcpy(cc->action, word);
        }
        
        token = strtok(NULL, " .,!?;:\n\t\"'()[]{}");
    }
    
    // Lưu entity cuối cùng
    if (have_entity && cc->subject_count < 10) {
        cc->subjects[cc->subject_count] = current_entity;
        cc->subject_count++;
    }
    
    // Trích xuất điều kiện
    char *if_pos = strstr(input, "nếu");
    if (if_pos) {
        strcpy(cc->condition, if_pos);
    }
    
    return cc;
}

void free_constraint_context(ConstraintContext *cc) {
    if (cc) free(cc);
}

// ============================================
//  HÀM KIỂM TRA RÀNG BUỘC
// ============================================

int check_constraints(ConstraintContext *cc, const char *entity) {
    if (!cc || !entity) return 0;
    
    for (int i = 0; i < cc->subject_count; i++) {
        if (strcmp(cc->subjects[i].entity, entity) == 0) {
            return 1;
        }
    }
    return 0;
}

char* get_entity_with_attributes(ConstraintContext *cc, const char *entity) {
    static char result[256];
    result[0] = '\0';
    
    if (!cc || !entity) return NULL;
    
    for (int i = 0; i < cc->subject_count; i++) {
        if (strcmp(cc->subjects[i].entity, entity) == 0) {
            snprintf(result, sizeof(result), "%s", entity);
            if (cc->subjects[i].attr_count > 0) {
                strcat(result, " (");
                for (int j = 0; j < cc->subjects[i].attr_count; j++) {
                    strcat(result, cc->subjects[i].attributes[j]);
                    if (j < cc->subjects[i].attr_count - 1) strcat(result, ", ");
                }
                strcat(result, ")");
            }
            return result;
        }
    }
    return NULL;
}

// ============================================
//  TÍCH HỢP REASONING VỚI RÀNG BUỘC
// ============================================

char* reasoning_with_constraints(const char *input) {
    static char response[4096];
    response[0] = '\0';
    
    // Trích xuất ràng buộc
    ConstraintContext *cc = extract_constraints(input);
    if (!cc) {
        // Fallback: gọi reasoning thường
        char *fallback = answer_reasoning_request(input);
        if (fallback) return fallback;
        return "❌ Không thể xử lý yêu cầu!";
    }
    
    strcat(response, "🔍 RÀNG BUỘC THUỘC TÍNH:\n\n");
    
    if (cc->subject_count > 0) {
        strcat(response, "📌 Thực thể tìm thấy:\n");
        for (int i = 0; i < cc->subject_count; i++) {
            char line[128];
            char entity_desc[256];
            strcpy(entity_desc, cc->subjects[i].entity);
            if (cc->subjects[i].attr_count > 0) {
                strcat(entity_desc, " [");
                for (int j = 0; j < cc->subjects[i].attr_count; j++) {
                    strcat(entity_desc, cc->subjects[i].attributes[j]);
                    if (j < cc->subjects[i].attr_count - 1) strcat(entity_desc, ", ");
                }
                strcat(entity_desc, "]");
            }
            snprintf(line, sizeof(line), "   - %s\n", entity_desc);
            strcat(response, line);
        }
    }
    
    if (strlen(cc->condition) > 0) {
        strcat(response, "\n⚡ Điều kiện: ");
        strcat(response, cc->condition);
        strcat(response, "\n");
    }
    
    if (strlen(cc->action) > 0 && strcmp(cc->action, "unknown") != 0) {
        strcat(response, "\n🎯 Hành động: ");
        strcat(response, cc->action);
        strcat(response, "\n");
    }
    
    free_constraint_context(cc);
    
    // Gọi reasoning thường
    char *reason_result = answer_reasoning_request(input);
    if (reason_result) {
        strcat(response, "\n🧠 SUY LUẬN:\n");
        strcat(response, reason_result);
    }
    
    return response;
}
