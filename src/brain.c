#include "brain.h"
#include "debug_logger.h"
#include "config.h"
#include "reasoning_nl.h"
#include "rule_learner.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define DEBUG_ENABLED 0  // TẮT DEBUG

void brain_init(Brain *b, const char *name) {
    debug_set_enabled(DEBUG_ENABLED);
    
    markov_init(&b->dict);
    strcpy(b->name, name);
    b->conversation_count = 0;
    context_init(&b->context);
    
    b->enable_reasoning = 1;  // BẬT REASONING
    
    // Load rules
    load_reasoning_rules();
    
    // Load learned rules
    load_learned_rules("data/learned_rules.txt");
    
    srand(time(NULL));
}

void brain_free(Brain *b) {
    save_learned_rules("data/learned_rules.txt");
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
    //  LEARN - HỌC RULE MỚI
    // ============================================
    if (is_learn_request(input)) {
        response = answer_learn_request(input);
        if (response) {
            context_add(&b->context, input, response);
            return response;
        }
    }
    
    // ============================================
    //  REASONING NL - SUY LUẬN
    // ============================================
    if (b->enable_reasoning && is_reasoning_nl_request(input)) {
        response = answer_reasoning_nl_request(input);
        if (response) {
            context_add(&b->context, input, response);
            return response;
        }
    }
    
    // ============================================
    //  FALLBACK - MARKOV
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
    printf("║ Reasoning:%-20s ║\n", b->enable_reasoning ? "BẬT ✅" : "TẮT ❌");
    printf("╚══════════════════════════════════╝\n");
}

void brain_get_context(Brain *b, char *buffer, int size) {
    context_get_summary(&b->context, buffer, size);
}

void brain_set_reasoning(Brain *b, int enable) { b->enable_reasoning = enable; }
