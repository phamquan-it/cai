#include "reasoning.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ============================================
//  CHỈ GIỮ TƯƠNG TÁC NGÔN NGỮ
// ============================================

int is_reasoning_request(const char *input) {
    if (!input) return 0;
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    const char *keywords[] = {"tìm", "biết", "tính", "giải", "là gì", "thế nào", "công thức"};
    for (int i = 0; i < 7; i++) {
        if (strstr(lower, keywords[i]) != NULL) {
            return 1;
        }
    }
    return 0;
}

char* answer_reasoning_request(const char *input) {
    static char response[512];
    response[0] = '\0';
    
    if (!input || !is_reasoning_request(input)) {
        return NULL;
    }
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    if (strstr(lower, "tìm") != NULL || strstr(lower, "biết") != NULL) {
        snprintf(response, sizeof(response), 
                 "🔍 Tôi đang tìm kiếm thông tin về câu hỏi của bạn.\n"
                 "💡 Hãy hỏi cụ thể hơn để tôi có thể giúp bạn tốt hơn!");
    } else if (strstr(lower, "tính") != NULL) {
        snprintf(response, sizeof(response), 
                 "🧮 Tôi có thể giúp bạn tính toán!\n"
                 "💡 Hãy cho tôi biết bạn muốn tính gì nhé!");
    } else if (strstr(lower, "giải") != NULL) {
        snprintf(response, sizeof(response), 
                 "📝 Tôi sẽ cố gắng giải đáp thắc mắc của bạn.\n"
                 "💡 Hãy mô tả chi tiết vấn đề bạn gặp phải!");
    } else {
        snprintf(response, sizeof(response), 
                 "🤔 Tôi hiểu câu hỏi của bạn.\n"
                 "💡 Hãy hỏi rõ ràng hơn để tôi có thể trả lời chính xác!");
    }
    
    return response;
}

void load_formulas() {
    // BỎ HẾT - KHÔNG LÀM GÌ
    return;
}
