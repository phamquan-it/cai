#include "brain.h"
#include "context_tracker.h"
#include "reasoning.h"
#include "text_processor.h"
#include "context_filter.h"
#include "attribute_constraint.h"
#include "thought_processor.h"
#include "sentence_templates.h"
#include "knowledge_learner.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

extern int template_count;
static ContextTracker global_context;
static KnowledgeBase *global_kb = NULL;

void brain_init(Brain *b, const char *name) {
    markov_init(&b->dict);
    strcpy(b->name, name);
    b->conversation_count = 0;
    context_init(&b->context);
    context_tracker_init(&global_context);
    load_sentence_templates();
    
    // KHỞI TẠO KNOWLEDGE BASE
    global_kb = init_knowledge_base();
    if (!global_kb) {
        printf("⚠️  Không thể khởi tạo Knowledge Base!\n");
    }
    
    b->enable_intent = 0;
    b->enable_attribute = 0;
    b->enable_physics = 0;
    b->enable_coder = 0;
    b->enable_knowledge = 0;
    b->enable_chemistry = 0;
    b->enable_vietnamese = 0;
    b->enable_bridge = 0;
    b->enable_reasoning = 1;
    load_formulas();
    srand(time(NULL));
}

void brain_free(Brain *b) {
    if (global_kb) {
        free_knowledge_base(global_kb);
        global_kb = NULL;
    }
    markov_free(&b->dict);
}

void brain_train(Brain *b, const char *data_file) {
    // KIỂM TRA global_kb
    if (!global_kb) {
        printf("⚠️  Knowledge Base NULL, khởi tạo lại...\n");
        global_kb = init_knowledge_base();
        if (!global_kb) {
            printf("❌ Không thể khởi tạo Knowledge Base!\n");
            return;
        }
    }
    
    markov_load_file(&b->dict, data_file);
    if (b->dict.count == 0) {
        printf("📚 Sử dụng kiến thức mặc định...\n");
        const char *default_data[] = {
            "xin chào tôi là một trí tuệ nhân tạo cơ bản",
            "tôi được viết bằng ngôn ngữ c",
            "tôi chỉ biết học và lặp lại",
            "tôi là một chatbot đơn giản"
        };
        for (int i = 0; i < 4; i++) {
            markov_learn(&b->dict, default_data[i]);
        }
        printf("✅ Đã tải %d kiến thức mặc định\n", 4);
    }
    
    // Học từ file knowledge.txt
    if (global_kb && data_file) {
        int learned = learn_from_file(global_kb, data_file, &b->dict);
        printf("📚 Đã học %d câu từ %s vào Knowledge Base\n", learned, data_file);
    }
}

void brain_learn(Brain *b, const char *sentence) {
    if (!sentence || strlen(sentence) < 5) return;
    
    markov_learn(&b->dict, sentence);
    if (global_kb) {
        learn_from_text(global_kb, sentence, "user", &b->dict);
    }
}

char* brain_chat(Brain *b, const char *input) {
    if (!input) return "Xin lỗi, tôi không hiểu!";
    
    b->conversation_count++;
    analyze_context(&global_context, input);
    
    char first_word[MAX_WORD_LEN];
    char temp[1000];
    strcpy(temp, input);
    char *token = strtok(temp, " .,!?;:\n\t\"'()[]{}");
    if (!token) {
        return "Xin lỗi, tôi không hiểu!";
    }
    strcpy(first_word, token);
    
    char *response = NULL;
    
    // ============================================
    //  FILL TEMPLATE
    // ============================================
    if (strncmp(input, "fill ", 5) == 0) {
        char template_id[32], slot_values[256];
        template_id[0] = '\0'; slot_values[0] = '\0';
        char temp_input[512];
        strcpy(temp_input, input + 5);
        char *space = strchr(temp_input, ' ');
        if (space) {
            *space = '\0';
            strcpy(template_id, temp_input);
            strcpy(slot_values, space + 1);
        } else {
            strcpy(template_id, temp_input);
        }
        char *filled = fill_template_with_string(template_id, slot_values);
        if (filled && strlen(filled) > 0) {
            context_add(&b->context, input, filled);
            return filled;
        } else {
            char error[256];
            snprintf(error, sizeof(error), "❌ Không tìm thấy template '%s'!", template_id);
            context_add(&b->context, input, error);
            return error;
        }
    }
    
    // ============================================
    //  KNOWLEDGE LEARNER
    // ============================================
    if (global_kb && is_learn_request(input)) {
        response = answer_learn_request(input, global_kb, &b->dict);
        if (response) {
            context_add(&b->context, input, response);
            return response;
        }
    }
    
    // ============================================
    //  TEMPLATE
    // ============================================
    if (is_template_request(input)) {
        response = answer_template_request(input);
        if (response) {
            context_add(&b->context, input, response);
            return response;
        }
    }
    
    // ============================================
    //  TEXT PROCESSOR
    // ============================================
    if (is_text_processing_request(input)) {
        response = answer_text_processing_request(input);
        if (response) {
            context_add(&b->context, input, response);
            return response;
        }
    }
    
    // ============================================
    //  CONTEXT TRACKER
    // ============================================
    if (is_context_request(input)) {
        response = answer_context_request(input, &global_context);
        if (response) {
            context_add(&b->context, input, response);
            return response;
        }
    }
    
    // ============================================
    //  THOUGHT + REASONING
    // ============================================
    if (b->enable_reasoning && (is_reasoning_request(input) || 
        strstr(input, "công thức") != NULL || 
        strstr(input, "tìm") != NULL ||
        strstr(input, "tính") != NULL ||
        strstr(input, "suy nghĩ") != NULL ||
        strstr(input, "kế hoạch") != NULL)) {
        
        if (strstr(input, "suy nghĩ") != NULL || strstr(input, "kế hoạch") != NULL) {
            response = process_thought(input);
            if (response) {
                context_add(&b->context, input, response);
                return response;
            }
        }
        
        response = reasoning_with_constraints(input);
        if (response && strstr(response, "RÀNG BUỘC") != NULL) {
            context_add(&b->context, input, response);
            return response;
        }
    }
    
    // ============================================
    //  FALLBACK
    // ============================================
    response = markov_generate(&b->dict, first_word);
    context_add(&b->context, input, response);
    return response;
}

void brain_stats(Brain *b) {
    printf("\n╔══════════════════════════════════╗\n");
    printf("║        THỐNG KÊ CƠ BẢN           ║\n");
    printf("╠══════════════════════════════════╣\n");
    printf("║ Tên:      %-20s ║\n", b->name);
    printf("║ Từ vựng:  %-20d ║\n", b->dict.count);
    printf("║ Số chat:  %-20d ║\n", b->conversation_count);
    printf("║ Ngữ cảnh: %-20d ║\n", global_context.count);
    printf("║ Mẫu câu:  %-20d ║\n", template_count);
    printf("║ Kiến thức:%-20d ║\n", global_kb ? global_kb->count : 0);
    printf("╚══════════════════════════════════╝\n");
}

void brain_get_context(Brain *b, char *buffer, int size) {
    context_get_summary(&b->context, buffer, size);
}

void brain_set_intent(Brain *b, int enable) { b->enable_intent = enable; }
void brain_set_attribute(Brain *b, int enable) { b->enable_attribute = enable; }
void brain_set_physics(Brain *b, int enable) { b->enable_physics = enable; }
void brain_set_coder(Brain *b, int enable) { b->enable_coder = enable; }
void brain_set_knowledge(Brain *b, int enable) { b->enable_knowledge = enable; }
void brain_set_chemistry(Brain *b, int enable) { b->enable_chemistry = enable; }
void brain_set_vietnamese(Brain *b, int enable) { b->enable_vietnamese = enable; }
void brain_set_bridge(Brain *b, int enable) { b->enable_bridge = enable; }
void brain_set_reasoning(Brain *b, int enable) { b->enable_reasoning = enable; }
