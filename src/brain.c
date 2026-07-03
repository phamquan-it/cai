#include "brain.h"
#include "context_tracker.h"
#include "reasoning.h"
#include "text_processor.h"
#include "context_filter.h"
#include "attribute_constraint.h"
#include "thought_processor.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

static ContextTracker global_context;

void brain_init(Brain *b, const char *name) {
    markov_init(&b->dict);
    strcpy(b->name, name);
    b->conversation_count = 0;
    context_init(&b->context);
    context_tracker_init(&global_context);
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
    markov_free(&b->dict);
}

void brain_train(Brain *b, const char *data_file) {
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
}

void brain_learn(Brain *b, const char *sentence) {
    if (strlen(sentence) > 5) {
        markov_learn(&b->dict, sentence);
    }
}

char* brain_chat(Brain *b, const char *input) {
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
    //  THOUGHT PROCESSOR - SUY NGHĨ CÓ KẾ HOẠCH
    // ============================================
    if (b->enable_reasoning && (is_reasoning_request(input) || 
        strstr(input, "công thức") != NULL || 
        strstr(input, "tìm") != NULL ||
        strstr(input, "tính") != NULL ||
        strstr(input, "suy nghĩ") != NULL ||
        strstr(input, "kế hoạch") != NULL)) {
        
        // Nếu yêu cầu suy nghĩ có kế hoạch
        if (strstr(input, "suy nghĩ") != NULL || strstr(input, "kế hoạch") != NULL) {
            response = process_thought(input);
            if (response) {
                context_add(&b->context, input, response);
                return response;
            }
        }
        
        // Thử với attribute constraints
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
