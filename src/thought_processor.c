#include "thought_processor.h"
#include "reasoning.h"  // THÊM DÒNG NÀY
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// ============================================
//  HÀM TẠO KẾ HOẠCH SUY NGHĨ
// ============================================

ThoughtPlan* create_thought_plan(const char *input) {
    if (!input) return NULL;
    
    ThoughtPlan *tp = (ThoughtPlan*)malloc(sizeof(ThoughtPlan));
    if (!tp) return NULL;
    
    memset(tp, 0, sizeof(ThoughtPlan));
    tp->step_count = 0;
    tp->confidence_level = 0;
    tp->error_count = 0;
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    // ============================================
    //  BƯỚC 1: Xác định mục tiêu
    // ============================================
    if (strstr(lower, "tìm") != NULL) {
        snprintf(tp->goal, sizeof(tp->goal), "Tìm kiếm thông tin về: %s", input);
    } else if (strstr(lower, "tính") != NULL) {
        snprintf(tp->goal, sizeof(tp->goal), "Tính toán: %s", input);
    } else if (strstr(lower, "giải") != NULL) {
        snprintf(tp->goal, sizeof(tp->goal), "Giải quyết vấn đề: %s", input);
    } else if (strstr(lower, "phân tích") != NULL) {
        snprintf(tp->goal, sizeof(tp->goal), "Phân tích: %s", input);
    } else {
        snprintf(tp->goal, sizeof(tp->goal), "Hiểu và trả lời: %s", input);
    }
    
    // ============================================
    //  BƯỚC 2: Xác định ràng buộc
    // ============================================
    char constraints[512] = "";
    if (strstr(lower, "nếu") != NULL) {
        strcat(constraints, "Có điều kiện: ");
        char *if_pos = strstr(input, "nếu");
        if (if_pos) {
            strcat(constraints, if_pos);
        }
    }
    if (strstr(lower, "cần") != NULL) {
        strcat(constraints, " | Yêu cầu đặc biệt");
    }
    strcpy(tp->constraints, constraints);
    
    // ============================================
    //  BƯỚC 3: Xây dựng các bước suy nghĩ
    // ============================================
    
    // Bước 1: Hiểu câu hỏi
    ThoughtStep step1;
    snprintf(step1.step, sizeof(step1.step), "Phân tích câu hỏi: '%s'", input);
    strcpy(step1.action, "PHÂN_TÍCH");
    strcpy(step1.condition, "Đọc và hiểu nội dung");
    snprintf(step1.expected_result, sizeof(step1.expected_result), "Hiểu được ý chính: %s", input);
    step1.confidence = 80;
    tp->steps[tp->step_count] = step1;
    tp->step_count++;
    
    // Bước 2: Xác định từ khóa
    ThoughtStep step2;
    strcpy(step2.step, "Trích xuất từ khóa và ngữ cảnh");
    strcpy(step2.action, "TRÍCH_XUẤT");
    strcpy(step2.condition, "Tìm từ có nghĩa trong câu");
    
    char keywords[128] = "";
    char temp[512];
    strcpy(temp, lower);
    char *token = strtok(temp, " .,!?;:\n\t");
    int kw_count = 0;
    while (token != NULL && kw_count < 5) {
        if (strlen(token) > 2) {
            strcat(keywords, token);
            strcat(keywords, " ");
            kw_count++;
        }
        token = strtok(NULL, " .,!?;:\n\t");
    }
    snprintf(step2.expected_result, sizeof(step2.expected_result), "Từ khóa: %s", keywords);
    step2.confidence = 85;
    tp->steps[tp->step_count] = step2;
    tp->step_count++;
    
    // Bước 3: Kiểm tra công thức hoặc kiến thức
    ThoughtStep step3;
    strcpy(step3.step, "Tìm kiếm công thức/kiến thức phù hợp");
    strcpy(step3.action, "TRA_CỨU");
    strcpy(step3.condition, "Dựa trên từ khóa đã trích xuất");
    
    int found_knowledge = 0;
    if (strstr(lower, "công thức") != NULL || strstr(lower, "tính") != NULL) {
        strcpy(step3.expected_result, "Tìm thấy công thức trong kho kiến thức");
        found_knowledge = 1;
    } else if (strstr(lower, "con") != NULL || strstr(lower, "chiếc") != NULL) {
        strcpy(step3.expected_result, "Nhận diện thực thể với thuộc tính");
        found_knowledge = 1;
    } else {
        strcpy(step3.expected_result, "Sử dụng kiến thức cơ bản để trả lời");
        found_knowledge = 0;
    }
    step3.confidence = found_knowledge ? 90 : 60;
    tp->steps[tp->step_count] = step3;
    tp->step_count++;
    
    // Bước 4: Suy luận
    ThoughtStep step4;
    strcpy(step4.step, "Tiến hành suy luận và tổng hợp");
    strcpy(step4.action, "SUY_LUẬN");
    strcpy(step4.condition, "Kết hợp các thông tin đã có");
    snprintf(step4.expected_result, sizeof(step4.expected_result), "Đưa ra kết luận dựa trên: %s", keywords);
    step4.confidence = 75;
    tp->steps[tp->step_count] = step4;
    tp->step_count++;
    
    // Bước 5: Kiểm tra lại
    ThoughtStep step5;
    strcpy(step5.step, "Kiểm tra tính hợp lý của kết luận");
    strcpy(step5.action, "KIỂM_TRA");
    strcpy(step5.condition, "So sánh với dữ liệu đã biết");
    strcpy(step5.expected_result, "Xác nhận kết quả hoặc phát hiện sai sót");
    step5.confidence = 70;
    tp->steps[tp->step_count] = step5;
    tp->step_count++;
    
    // ============================================
    //  BƯỚC 4: Kiểm tra lỗi tiềm ẩn
    // ============================================
    if (strstr(lower, "công thức") != NULL && !strstr(lower, "tính") != NULL) {
        strcpy(tp->errors[tp->error_count], "⚠️ Thiếu từ khóa 'tính' - có thể không rõ yêu cầu");
        tp->error_count++;
    }
    if (strlen(keywords) < 5) {
        strcpy(tp->errors[tp->error_count], "⚠️ Quá ít từ khóa - khó xác định chính xác vấn đề");
        tp->error_count++;
    }
    
    // ============================================
    //  BƯỚC 5: Đánh giá độ tin cậy
    // ============================================
    int total_confidence = 0;
    for (int i = 0; i < tp->step_count; i++) {
        total_confidence += tp->steps[i].confidence;
    }
    tp->confidence_level = total_confidence / tp->step_count;
    
    // Giảm điểm nếu có lỗi
    if (tp->error_count > 0) {
        tp->confidence_level -= tp->error_count * 5;
    }
    if (tp->confidence_level < 0) tp->confidence_level = 10;
    
    return tp;
}

void free_thought_plan(ThoughtPlan *tp) {
    if (tp) free(tp);
}

// ============================================
//  HÀM KIỂM TRA LỖI
// ============================================

int validate_thought(ThoughtPlan *tp) {
    if (!tp) return 0;
    
    // Kiểm tra các bước có hợp lý không
    for (int i = 0; i < tp->step_count; i++) {
        if (strlen(tp->steps[i].step) < 3) {
            return 0;
        }
    }
    
    // Kiểm tra có mục tiêu không
    if (strlen(tp->goal) < 3) {
        return 0;
    }
    
    return 1;
}

char* get_thought_errors(ThoughtPlan *tp) {
    static char result[1024];
    result[0] = '\0';
    
    if (!tp || tp->error_count == 0) {
        strcpy(result, "✅ Không phát hiện lỗi nào");
        return result;
    }
    
    strcat(result, "⚠️ CÁC LỖI TIỀM ẨN:\n");
    for (int i = 0; i < tp->error_count; i++) {
        char line[128];
        snprintf(line, sizeof(line), "   %d. %s\n", i+1, tp->errors[i]);
        strcat(result, line);
    }
    
    return result;
}

// ============================================
//  HÀM SUY NGHĨ CÓ KẾ HOẠCH
// ============================================

char* process_thought(const char *input) {
    static char response[4096];
    response[0] = '\0';
    
    // Tạo kế hoạch suy nghĩ
    ThoughtPlan *tp = create_thought_plan(input);
    if (!tp) {
        return "❌ Không thể tạo kế hoạch suy nghĩ!";
    }
    
    // ============================================
    //  HIỂN THỊ KẾ HOẠCH
    // ============================================
    strcat(response, "🧠 QUÁ TRÌNH SUY NGHĨ CÓ KẾ HOẠCH:\n\n");
    
    // Mục tiêu
    char goal_line[256];
    snprintf(goal_line, sizeof(goal_line), "🎯 Mục tiêu: %s\n", tp->goal);
    strcat(response, goal_line);
    
    // Ràng buộc
    if (strlen(tp->constraints) > 0) {
        char constraint_line[256];
        snprintf(constraint_line, sizeof(constraint_line), "🔒 Ràng buộc: %s\n", tp->constraints);
        strcat(response, constraint_line);
    }
    
    strcat(response, "\n📋 CÁC BƯỚC SUY NGHĨ:\n");
    
    for (int i = 0; i < tp->step_count; i++) {
        char step_line[512];
        snprintf(step_line, sizeof(step_line), 
                 "   %d. %s\n"
                 "      🔹 Hành động: %s\n"
                 "      🔹 Điều kiện: %s\n"
                 "      🔹 Dự kiến: %s\n"
                 "      🔹 Độ tin cậy: %d%%\n\n",
                 i+1,
                 tp->steps[i].step,
                 tp->steps[i].action,
                 tp->steps[i].condition,
                 tp->steps[i].expected_result,
                 tp->steps[i].confidence);
        strcat(response, step_line);
    }
    
    // ============================================
    //  ĐÁNH GIÁ VÀ KẾT LUẬN
    // ============================================
    strcat(response, "📊 ĐÁNH GIÁ:\n");
    
    char conf_line[128];
    snprintf(conf_line, sizeof(conf_line), "   🔹 Độ tin cậy tổng thể: %d%%\n", tp->confidence_level);
    strcat(response, conf_line);
    
    char *errors = get_thought_errors(tp);
    strcat(response, "   🔹 ");
    strcat(response, errors);
    strcat(response, "\n");
    
    // ============================================
    //  KẾT LUẬN
    // ============================================
    strcat(response, "\n✅ KẾT LUẬN:\n");
    
    if (tp->confidence_level >= 80) {
        strcat(response, "   💡 Suy nghĩ đã hoàn chỉnh, có thể đưa ra câu trả lời.\n");
    } else if (tp->confidence_level >= 60) {
        strcat(response, "   ⚡ Cần thêm thông tin để có kết quả chính xác hơn.\n");
    } else {
        strcat(response, "   ❌ Chưa đủ dữ liệu để suy luận, vui lòng cung cấp thêm.\n");
    }
    
    // Gọi reasoning để có kết quả thực tế
    char *reason_result = answer_reasoning_request(input);
    if (reason_result && strstr(reason_result, "Không tìm thấy") == NULL) {
        strcat(response, "\n🧠 KẾT QUẢ SUY LUẬN:\n");
        strcat(response, reason_result);
    }
    
    free_thought_plan(tp);
    return response;
}
