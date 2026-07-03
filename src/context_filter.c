#include "context_filter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// ============================================
//  KHO TỪ CÓ NGHĨA
// ============================================

// Từ khóa biến (variable)
static const char *variable_keywords[] = {
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
    "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
    "alpha", "beta", "gamma", "delta", "theta", "lambda", "pi", "sigma"
};

// Từ khóa toán học
static const char *math_keywords[] = {
    "tổng", "hiệu", "tích", "thương", "cộng", "trừ", "nhân", "chia",
    "bình phương", "căn", "log", "sin", "cos", "tan", "limit", "tích phân",
    "đạo hàm", "phương trình", "hệ số", "biến số", "hằng số"
};

// Từ khóa vật lý
static const char *physics_keywords[] = {
    "lực", "khối lượng", "vận tốc", "gia tốc", "quãng đường", "thời gian",
    "công", "năng lượng", "động năng", "thế năng", "áp suất", "nhiệt lượng",
    "điện trở", "dòng điện", "hiệu điện thế", "công suất", "bước sóng"
};

// Từ khóa hóa học
static const char *chemistry_keywords[] = {
    "nguyên tử", "phân tử", "nguyên tố", "hợp chất", "phản ứng", "axit",
    "bazơ", "muối", "dung dịch", "nồng độ", "mol", "khối lượng mol",
    "bảng tuần hoàn", "hóa trị", "liên kết hóa học"
};

// Từ khóa lập trình
static const char *coding_keywords[] = {
    "hàm", "biến", "mảng", "con trỏ", "struct", "class", "vòng lặp",
    "điều kiện", "int", "char", "float", "double", "void", "return"
};

// Từ dừng (stopwords - bỏ qua)
static const char *stopwords[] = {
    "và", "của", "là", "trong", "với", "cho", "khi", "nếu", "để", "mà",
    "này", "đó", "ở", "từ", "còn", "thì", "sẽ", "đã", "đang", "được",
    "có", "không", "cũng", "nên", "vì", "bởi", "tại", "như", "ra", "vào"
};

// ============================================
//  HÀM KIỂM TRA
// ============================================

int is_variable_word(const char *word) {
    if (!word) return 0;
    int count = sizeof(variable_keywords) / sizeof(variable_keywords[0]);
    for (int i = 0; i < count; i++) {
        if (strcmp(word, variable_keywords[i]) == 0) return 1;
    }
    // Kiểm tra biến 1 ký tự chữ cái
    if (strlen(word) == 1 && isalpha(word[0])) return 1;
    return 0;
}

int is_keyword(const char *word) {
    if (!word) return 0;
    
    // Kiểm tra trong các danh sách từ khóa
    const char **lists[] = {math_keywords, physics_keywords, chemistry_keywords, coding_keywords};
    int list_sizes[] = {
        sizeof(math_keywords) / sizeof(math_keywords[0]),
        sizeof(physics_keywords) / sizeof(physics_keywords[0]),
        sizeof(chemistry_keywords) / sizeof(chemistry_keywords[0]),
        sizeof(coding_keywords) / sizeof(coding_keywords[0])
    };
    
    for (int list = 0; list < 4; list++) {
        for (int i = 0; i < list_sizes[list]; i++) {
            if (strstr(lists[list][i], word) != NULL || strstr(word, lists[list][i]) != NULL) {
                return 1;
            }
        }
    }
    return 0;
}

int is_stopword(const char *word) {
    if (!word) return 0;
    int count = sizeof(stopwords) / sizeof(stopwords[0]);
    for (int i = 0; i < count; i++) {
        if (strcmp(word, stopwords[i]) == 0) return 1;
    }
    return 0;
}

int is_meaningful_word(const char *word) {
    if (!word || strlen(word) < 1) return 0;
    
    char lower[64];
    strcpy(lower, word);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    // Bỏ qua stopword
    if (is_stopword(lower)) return 0;
    
    // Kiểm tra có chữ số
    int has_alpha = 0;
    for (int i = 0; lower[i]; i++) {
        if (isalpha(lower[i]) || isdigit(lower[i])) {
            has_alpha = 1;
            break;
        }
    }
    if (!has_alpha) return 0;
    
    return 1;
}

// ============================================
//  HÀM LỌC NGỮ CẢNH
// ============================================

FilteredContext* filter_context(const char *input) {
    if (!input) return NULL;
    
    FilteredContext *fc = (FilteredContext*)malloc(sizeof(FilteredContext));
    if (!fc) return NULL;
    
    memset(fc, 0, sizeof(FilteredContext));
    strcpy(fc->raw_input, input);
    fc->count = 0;
    strcpy(fc->context_type, "general");
    
    char temp[512];
    strcpy(temp, input);
    
    // Tách từ
    char *token = strtok(temp, " .,!?;:\n\t\"'()[]{}");
    while (token != NULL && fc->count < 50) {
        char clean[64];
        strcpy(clean, token);
        
        // Chuyển sang chữ thường
        for (int i = 0; clean[i]; i++) {
            clean[i] = tolower(clean[i]);
        }
        
        // Kiểm tra từ có nghĩa
        if (is_meaningful_word(clean)) {
            // Xác định loại từ
            char type[32] = "unknown";
            float weight = 0.5;
            
            if (is_variable_word(clean)) {
                strcpy(type, "variable");
                weight = 0.9;
            } else if (is_keyword(clean)) {
                strcpy(type, "keyword");
                weight = 0.8;
            }
            
            // Thêm vào danh sách
            strcpy(fc->words[fc->count].word, clean);
            strcpy(fc->words[fc->count].type, type);
            fc->words[fc->count].weight = weight;
            fc->count++;
        }
        
        token = strtok(NULL, " .,!?;:\n\t\"'()[]{}");
    }
    
    // Detect context type
    strcpy(fc->context_type, detect_context_type(fc));
    
    return fc;
}

void free_filtered_context(FilteredContext *fc) {
    if (fc) free(fc);
}

// ============================================
//  DETECT CONTEXT TYPE
// ============================================

char* detect_context_type(FilteredContext *fc) {
    static char result[32];
    strcpy(result, "general");
    
    if (!fc || fc->count == 0) return result;
    
    int math_score = 0, physics_score = 0, chemistry_score = 0, coding_score = 0;
    
    for (int i = 0; i < fc->count; i++) {
        char *word = fc->words[i].word;
        
        // Kiểm tra toán
        for (int j = 0; j < sizeof(math_keywords)/sizeof(math_keywords[0]); j++) {
            if (strstr(word, math_keywords[j]) != NULL) math_score++;
        }
        
        // Kiểm tra vật lý
        for (int j = 0; j < sizeof(physics_keywords)/sizeof(physics_keywords[0]); j++) {
            if (strstr(word, physics_keywords[j]) != NULL) physics_score++;
        }
        
        // Kiểm tra hóa
        for (int j = 0; j < sizeof(chemistry_keywords)/sizeof(chemistry_keywords[0]); j++) {
            if (strstr(word, chemistry_keywords[j]) != NULL) chemistry_score++;
        }
        
        // Kiểm tra coding
        for (int j = 0; j < sizeof(coding_keywords)/sizeof(coding_keywords[0]); j++) {
            if (strstr(word, coding_keywords[j]) != NULL) coding_score++;
        }
    }
    
    // Chọn loại có điểm cao nhất
    int max_score = math_score;
    char *type = "math";
    
    if (physics_score > max_score) {
        max_score = physics_score;
        type = "physics";
    }
    if (chemistry_score > max_score) {
        max_score = chemistry_score;
        type = "chemistry";
    }
    if (coding_score > max_score) {
        max_score = coding_score;
        type = "coding";
    }
    
    if (max_score > 0) {
        strcpy(result, type);
    }
    
    return result;
}

// ============================================
//  TÍCH HỢP VỚI REASONING
// ============================================

char* reasoning_with_context(const char *input, FilteredContext *fc) {
    static char response[4096];
    response[0] = '\0';
    
    if (!fc || fc->count == 0) {
        return answer_reasoning_request(input);
    }
    
    // Xây dựng câu hỏi đã được lọc
    char clean_query[512];
    clean_query[0] = '\0';
    
    // Lấy các từ khóa quan trọng
    for (int i = 0; i < fc->count && i < 10; i++) {
        if (fc->words[i].weight >= 0.7) {
            strcat(clean_query, fc->words[i].word);
            strcat(clean_query, " ");
        }
    }
    
    if (strlen(clean_query) == 0) {
        strcpy(clean_query, input);
    }
    
    // Gọi reasoning với câu hỏi đã lọc
    char *reasoning_result = answer_reasoning_request(clean_query);
    if (reasoning_result) {
        snprintf(response, sizeof(response),
                 "🧠 Ngữ cảnh: %s\n📊 Từ khóa: %s\n\n%s",
                 fc->context_type, clean_query, reasoning_result);
        return response;
    }
    
    return NULL;
}
