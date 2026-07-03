#ifndef BRIDGE_H
#define BRIDGE_H

#include "brain.h"

// Cấu trúc suy nghĩ
typedef struct {
    char query[512];
    char context[1024];
    char knowledge_summary[1024];
    int confidence;
    char best_response[2048];
    char alternatives[3][1024];
    int alternative_count;
} Thought;

// Hàm khởi tạo bridge
void bridge_init(Brain *brain);
void bridge_free();

// Hàm suy nghĩ
Thought* think(const char *input, Brain *brain);
void free_thought(Thought *thought);

// Hàm tổng hợp kiến thức
char* synthesize_knowledge(const char *input, Brain *brain);

// Hàm đánh giá câu trả lời
int evaluate_response(const char *response, const char *query);

// Hàm chọn câu trả lời tốt nhất
char* select_best_response(Thought *thought);

#endif
