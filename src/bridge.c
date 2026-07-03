#include "bridge.h"
#include "definition.h"
#include "physics.h"
#include "coder.h"
#include "knowledge.h"
#include "chemistry.h"
#include "vietnamese.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static Brain *global_brain = NULL;
static int thought_count = 0;

void bridge_init(Brain *brain) {
    global_brain = brain;
    thought_count = 0;
    srand(time(NULL));
}

void bridge_free() {
    global_brain = NULL;
}

// ============================================
//  TỔNG HỢP KIẾN THỨC (có kiểm tra NULL)
// ============================================

char* synthesize_knowledge(const char *input, Brain *brain) {
    static char knowledge[4096];
    knowledge[0] = '\0';
    
    if (!brain || !input) {
        strcpy(knowledge, "❌ Lỗi: Không có dữ liệu để suy nghĩ!");
        return knowledge;
    }
    
    char temp[512];
    char response_buf[4096];
    int has_knowledge = 0;
    
    // 1. Kiểm tra định nghĩa
    if (is_definition_question(input)) {
        char *def = answer_definition_question(input);
        if (def && strstr(def, "chưa biết") == NULL && strstr(def, "chưa có") == NULL) {
            strcat(knowledge, "📚 Định nghĩa: ");
            strcat(knowledge, def);
            strcat(knowledge, "\n");
            has_knowledge = 1;
        }
    }
    
    // 2. Kiểm tra vật lý
    if (is_physics_question(input)) {
        char *phy = answer_physics_question(input);
        if (phy && strstr(phy, "chưa có công thức") == NULL) {
            strcat(knowledge, "🔬 Vật lý: ");
            strcat(knowledge, phy);
            strcat(knowledge, "\n");
            has_knowledge = 1;
        }
    }
    
    // 3. Kiểm tra hóa học
    if (is_chemistry_question(input)) {
        char *chem = answer_chemistry_question(input);
        if (chem && strstr(chem, "chưa có kiến thức") == NULL) {
            strcat(knowledge, "🧪 Hóa học: ");
            strcat(knowledge, chem);
            strcat(knowledge, "\n");
            has_knowledge = 1;
        }
    }
    
    // 4. Kiểm tra kiến thức C
    if (is_c_knowledge_question(input)) {
        char *c_know = answer_c_knowledge_question(input);
        if (c_know && strstr(c_know, "chưa có kiến thức") == NULL) {
            strcat(knowledge, "💻 Kiến thức C: ");
            strcat(knowledge, c_know);
            strcat(knowledge, "\n");
            has_knowledge = 1;
        }
    }
    
    // 5. Code
    if (is_coding_request(input)) {
        char *code = answer_coding_request(input);
        if (code) {
            strcat(knowledge, "🧑‍💻 Code: ");
            strcat(knowledge, code);
            strcat(knowledge, "\n");
            has_knowledge = 1;
        }
    }
    
    // Nếu không có kiến thức
    if (!has_knowledge) {
        char first_word[64];
        char temp_copy[512];
        strcpy(temp_copy, input);
        char *token = strtok(temp_copy, " .,!?;:\n\t");
        if (token && global_brain) {
            strcpy(first_word, token);
            char *markov_response = markov_generate(&global_brain->dict, first_word);
            snprintf(knowledge, sizeof(knowledge),
                     "🤔 Tôi suy nghĩ về câu hỏi của bạn...\n💡 Từ khóa: '%s'\n📝 %s",
                     first_word, markov_response ? markov_response : "Không có phản hồi!");
        } else {
            strcpy(knowledge, "❓ Tôi không có đủ thông tin để trả lời!");
        }
    }
    
    return knowledge;
}

// ============================================
//  HÀM ĐÁNH GIÁ CÂU TRẢ LỜI
// ============================================

int evaluate_response(const char *response, const char *query) {
    if (!response || !query) return 0;
    
    int score = 0;
    int response_len = strlen(response);
    
    if (response_len > 20 && response_len < 500) score += 2;
    if (strpbrk(response, ".!?")) score += 1;
    if (response_len > strlen(query) * 0.5) score += 2;
    
    return score;
}

// ============================================
//  HÀM SUY NGHĨ (có kiểm tra NULL)
// ============================================

Thought* think(const char *input, Brain *brain) {
    Thought *thought = (Thought*)malloc(sizeof(Thought));
    if (!thought) return NULL;
    
    memset(thought, 0, sizeof(Thought));
    strcpy(thought->query, input);
    thought->alternative_count = 0;
    thought->confidence = 0;
    
    if (!brain) {
        strcpy(thought->best_response, "Chưa có bộ não để suy nghĩ!");
        return thought;
    }
    
    // Thu thập kiến thức
    char *knowledge = synthesize_knowledge(input, brain);
    strncpy(thought->knowledge_summary, knowledge, sizeof(thought->knowledge_summary) - 1);
    
    // Tạo phương án trả lời
    char *responses[5];
    int response_count = 0;
    
    // Phương án 1: Kiến thức tổng hợp
    if (strlen(knowledge) > 0 && response_count < 5) {
        responses[response_count++] = knowledge;
    }
    
    // Phương án 2: Markov fallback
    char first_word[64];
    char temp_copy[512];
    strcpy(temp_copy, input);
    char *token = strtok(temp_copy, " .,!?;:\n\t\"'()[]{}");
    if (token) {
        strcpy(first_word, token);
        char *markov_response = markov_generate(&brain->dict, first_word);
        if (markov_response && response_count < 5) {
            responses[response_count++] = markov_response;
        }
    }
    
    // Đánh giá và chọn câu trả lời tốt nhất
    int best_score = -1;
    int best_index = 0;
    
    for (int i = 0; i < response_count && i < 5; i++) {
        int score = evaluate_response(responses[i], input);
        if (score > best_score) {
            best_score = score;
            best_index = i;
        }
    }
    
    if (response_count > 0 && best_index < response_count) {
        strncpy(thought->best_response, responses[best_index], sizeof(thought->best_response) - 1);
        thought->confidence = best_score > 5 ? 80 : 50;
    } else {
        strcpy(thought->best_response, "Tôi chưa tìm được câu trả lời phù hợp!");
        thought->confidence = 20;
    }
    
    // Thêm ngữ cảnh nếu có
    if (brain->context.count > 0) {
        char context_buf[1024];
        context_get_summary(&brain->context, context_buf, sizeof(context_buf));
        if (strlen(context_buf) > 0) {
            snprintf(thought->context, sizeof(thought->context),
                     "📝 Ngữ cảnh:\n%s", context_buf);
        }
    }
    
    thought_count++;
    return thought;
}

void free_thought(Thought *thought) {
    if (thought) free(thought);
}

char* select_best_response(Thought *thought) {
    if (!thought) return "Không có suy nghĩ nào!";
    
    static char final_response[4096];
    final_response[0] = '\0';
    
    if (strlen(thought->context) > 0) {
        strcat(final_response, thought->context);
        strcat(final_response, "\n");
    }
    
    strcat(final_response, thought->best_response);
    
    if (thought->confidence < 50) {
        strcat(final_response, "\n\n⚠️ Lưu ý: Tôi không chắc lắm về câu trả lời này!");
    }
    
    return final_response;
}
