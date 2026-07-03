#include "brain.h"
#include "definition.h"
#include "physics.h"
#include "coder.h"
#include "knowledge.h"
#include "chemistry.h"
#include "vietnamese.h"
#include "bridge.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

extern int def_count;

void brain_init(Brain *b, const char *name) {
    markov_init(&b->dict);
    strcpy(b->name, name);
    b->conversation_count = 0;
    context_init(&b->context);
    b->enable_intent = 1;
    b->enable_attribute = 1;
    b->enable_physics = 1;
    b->enable_coder = 1;
    b->enable_knowledge = 1;
    b->enable_chemistry = 1;
    b->enable_vietnamese = 1;
    b->enable_bridge = 1;
    load_definitions();
    load_physics_formulas();
    load_c_knowledge();
    load_chemistry_knowledge();
    // KHỞI TẠO BRIDGE SAU KHI BRAIN ĐÃ SẴN SÀNG
    bridge_init(b);
    srand(time(NULL));
}

void brain_free(Brain *b) {
    bridge_free();
    markov_free(&b->dict);
    free_definitions();
}

void brain_train(Brain *b, const char *data_file) {
    markov_load_file(&b->dict, data_file);
    if (b->dict.count == 0) {
        printf("📚 Sử dụng kiến thức mặc định...\n");
        const char *default_data[] = {
            "xin chào tôi là cai một trí tuệ nhân tạo",
            "tôi tên là cai và tôi được viết bằng c",
            "học máy rất thú vị và mạnh mẽ",
            "lập trình c rất nhanh và hiệu quả",
            "tôi thích học những điều mới mẻ",
            "lập trình là một nghệ thuật",
            "tôi là một ai ngốc nhưng tôi chạy rất nhanh"
        };
        for (int i = 0; i < 7; i++) {
            markov_learn(&b->dict, default_data[i]);
        }
        printf("✅ Đã tải %d kiến thức mặc định\n", 7);
    }
}

void brain_learn(Brain *b, const char *sentence) {
    if (strlen(sentence) > 5) {
        markov_learn(&b->dict, sentence);
    }
}

void brain_set_intent(Brain *b, int enable) {
    b->enable_intent = enable;
}

void brain_set_attribute(Brain *b, int enable) {
    b->enable_attribute = enable;
}

void brain_set_physics(Brain *b, int enable) {
    b->enable_physics = enable;
}

void brain_set_coder(Brain *b, int enable) {
    b->enable_coder = enable;
}

void brain_set_knowledge(Brain *b, int enable) {
    b->enable_knowledge = enable;
}

void brain_set_chemistry(Brain *b, int enable) {
    b->enable_chemistry = enable;
}

void brain_set_vietnamese(Brain *b, int enable) {
    b->enable_vietnamese = enable;
}

void brain_set_bridge(Brain *b, int enable) {
    b->enable_bridge = enable;
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
    //  BRIDGE - SUY NGHĨ TỔNG HỢP
    // ============================================
    if (b->enable_bridge && b) {
        Thought *thought = think(input, b);
        if (thought) {
            response = select_best_response(thought);
            free_thought(thought);
        }
    }
    
    // ============================================
    //  VIETNAMESE - XỬ LÝ CÂU TIẾNG VIỆT
    // ============================================
    if (response && b->enable_vietnamese && is_vietnamese_text(response)) {
        char *formatted = format_vietnamese_sentence(response);
        if (formatted) {
            context_add(&b->context, input, formatted);
            return formatted;
        }
    }
    
    // ============================================
    //  CHEMISTRY - HÓA HỌC
    // ============================================
    if (response == NULL && b->enable_chemistry && is_chemistry_question(input)) {
        response = answer_chemistry_question(input);
        if (response) {
            context_add(&b->context, input, response);
            return response;
        }
    }
    
    // ============================================
    //  KNOWLEDGE - KIẾN THỨC VỀ C
    // ============================================
    if (response == NULL && b->enable_knowledge && is_c_knowledge_question(input)) {
        response = answer_c_knowledge_question(input);
        if (response) {
            context_add(&b->context, input, response);
            return response;
        }
    }
    
    // ============================================
    //  CODER - VIẾT CODE
    // ============================================
    if (response == NULL && b->enable_coder && is_coding_request(input)) {
        response = answer_coding_request(input);
        if (response) {
            context_add(&b->context, input, response);
            return response;
        }
    }
    
    // ============================================
    //  PHYSICS - NHÀ VẬT LÝ LỖI LẠC
    // ============================================
    if (response == NULL && b->enable_physics && is_physics_question(input)) {
        response = answer_physics_question(input);
        if (response) {
            context_add(&b->context, input, response);
            return response;
        }
    }
    
    // ============================================
    //  DEFINITION - HỎI "LÀ GÌ"
    // ============================================
    if (response == NULL && is_definition_question(input)) {
        response = answer_definition_question(input);
        if (response) {
            context_add(&b->context, input, response);
            return response;
        }
    }
    
    // ============================================
    //  ATTRIBUTE EXTRACTION
    // ============================================
    if (response == NULL && b->enable_attribute) {
        ObjectAttributes *obj = extract_attributes(input);
        if (obj && obj->count > 0) {
            response = generate_attribute_response(obj);
        }
        if (obj) free_attributes(obj);
    }
    
    // ============================================
    //  INTENT ANALYSIS
    // ============================================
    if (response == NULL && b->enable_intent) {
        Intent *intent = analyze_intent(input);
        if (intent && intent->confidence > 0.5) {
            response = generate_response_by_intent(intent);
        }
        if (intent) free_intent(intent);
    }
    
    // ============================================
    //  FALLBACK
    // ============================================
    if (response == NULL) {
        response = markov_generate(&b->dict, first_word);
    }
    
    if (b->enable_vietnamese && is_vietnamese_text(response)) {
        char *formatted = format_vietnamese_sentence(response);
        if (formatted) {
            context_add(&b->context, input, formatted);
            return formatted;
        }
    }
    
    context_add(&b->context, input, response);
    return response;
}

void brain_stats(Brain *b) {
    printf("\n╔═══════════════════════════════════════════╗\n");
    printf("║        🧠  THỐNG KÊ CAI                   ║\n");
    printf("╠═══════════════════════════════════════════╣\n");
    printf("║ Tên:      %-25s ║\n", b->name);
    printf("║ Từ vựng:  %-25d ║\n", b->dict.count);
    printf("║ Số chat:  %-25d ║\n", b->conversation_count);
    printf("║ Ngữ cảnh: %-25d ║\n", b->context.count);
    printf("║ Intent:   %-25s ║\n", b->enable_intent ? "BẬT" : "TẮT");
    printf("║ Attribute:%-25s ║\n", b->enable_attribute ? "BẬT" : "TẮT");
    printf("║ Physics:  %-25s ║\n", b->enable_physics ? "BẬT" : "TẮT");
    printf("║ Chemistry:%-24s ║\n", b->enable_chemistry ? "BẬT" : "TẮT");
    printf("║ Coder:    %-25s ║\n", b->enable_coder ? "BẬT" : "TẮT");
    printf("║ Knowledge:%-24s ║\n", b->enable_knowledge ? "BẬT" : "TẮT");
    printf("║ Vietnamese:%-23s ║\n", b->enable_vietnamese ? "BẬT 🇻🇳" : "TẮT");
    printf("║ Bridge:   %-25s ║\n", b->enable_bridge ? "BẬT 🧠" : "TẮT");
    printf("║ Định nghĩa:%-24d ║\n", def_count);
    printf("╚═══════════════════════════════════════════╝\n");
}

void brain_get_context(Brain *b, char *buffer, int size) {
    context_get_summary(&b->context, buffer, size);
}
