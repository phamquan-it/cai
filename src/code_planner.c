#include "code_planner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ============================================
//  TẠO KẾ HOẠCH CODE THEO YÊU CẦU
// ============================================

static char* generate_plan(const char *func_name, const char *desc, const char *lang) {
    static char plan[4096];
    plan[0] = '\0';
    
    // Xử lý tên hàm
    char clean_name[64];
    strcpy(clean_name, func_name);
    for (int i = 0; clean_name[i]; i++) {
        if (!isalnum(clean_name[i]) && clean_name[i] != '_') {
            clean_name[i] = '_';
        }
    }
    
    // ========================================
    //  C - PLAN
    // ========================================
    if (strstr(lang, "c") != NULL && strstr(lang, "c++") == NULL) {
        snprintf(plan, sizeof(plan),
            "/*\n"
            " * ============================================================\n"
            " *  HÀM: %s\n"
            " *  MÔ TẢ: %s\n"
            " *  NGÔN NGỮ: C\n"
            " * ============================================================\n"
            " */\n\n"
            "#include <stdio.h>\n"
            "#include <stdlib.h>\n\n"
            "// ============================================================\n"
            "//  KẾ HOẠCH TRIỂN KHAI\n"
            "// ============================================================\n"
            "// Bước 1: Khai báo biến và tham số\n"
            "// Bước 2: Kiểm tra dữ liệu đầu vào\n"
            "// Bước 3: Xử lý logic chính\n"
            "// Bước 4: Trả về kết quả\n"
            "// ============================================================\n\n"
            "int %s(/* tham số */) {\n"
            "    // =========================================================\n"
            "    //  BƯỚC 1: Khai báo biến\n"
            "    // =========================================================\n"
            "    int result = 0;\n\n"
            "    // =========================================================\n"
            "    //  BƯỚC 2: Kiểm tra input\n"
            "    // =========================================================\n"
            "    // TODO: Kiểm tra dữ liệu đầu vào\n\n"
            "    // =========================================================\n"
            "    //  BƯỚC 3: Xử lý logic\n"
            "    // =========================================================\n"
            "    // TODO: Thêm logic xử lý ở đây\n"
            "    result = 0; // placeholder\n\n"
            "    // =========================================================\n"
            "    //  BƯỚC 4: Trả về kết quả\n"
            "    // =========================================================\n"
            "    return result;\n"
            "}\n\n"
            "int main() {\n"
            "    // =========================================================\n"
            "    //  TEST HÀM\n"
            "    // =========================================================\n"
            "    int test = %s(/* tham số test */);\n"
            "    printf(\"Kết quả: %%d\\n\", test);\n"
            "    return 0;\n"
            "}\n", clean_name, desc, clean_name, clean_name);
        return plan;
    }
    
    // ========================================
    //  PYTHON - PLAN
    // ========================================
    else if (strstr(lang, "python") != NULL || strstr(lang, "py") != NULL) {
        snprintf(plan, sizeof(plan),
            "# ============================================================\n"
            "#  HÀM: %s\n"
            "#  MÔ TẢ: %s\n"
            "#  NGÔN NGỮ: Python\n"
            "# ============================================================\n\n"
            "# ============================================================\n"
            "#  KẾ HOẠCH TRIỂN KHAI\n"
            "# ============================================================\n"
            "# Bước 1: Khai báo biến và tham số\n"
            "# Bước 2: Kiểm tra dữ liệu đầu vào\n"
            "# Bước 3: Xử lý logic chính\n"
            "# Bước 4: Trả về kết quả\n"
            "# ============================================================\n\n"
            "def %s(tham_số):\n"
            "    # =========================================================\n"
            "    #  BƯỚC 1: Khai báo biến\n"
            "    # =========================================================\n"
            "    ket_qua = 0\n\n"
            "    # =========================================================\n"
            "    #  BƯỚC 2: Kiểm tra input\n"
            "    # =========================================================\n"
            "    # TODO: Kiểm tra dữ liệu đầu vào\n\n"
            "    # =========================================================\n"
            "    #  BƯỚC 3: Xử lý logic\n"
            "    # =========================================================\n"
            "    # TODO: Thêm logic xử lý ở đây\n"
            "    ket_qua = 0  # placeholder\n\n"
            "    # =========================================================\n"
            "    #  BƯỚC 4: Trả về kết quả\n"
            "    # =========================================================\n"
            "    return ket_qua\n\n"
            "if __name__ == \"__main__\":\n"
            "    # =========================================================\n"
            "    #  TEST HÀM\n"
            "    # =========================================================\n"
            "    test = %s(tham_số_test)\n"
            "    print(f\"Kết quả: {test}\")\n", clean_name, desc, clean_name, clean_name);
        return plan;
    }
    
    // ========================================
    //  C++ - PLAN
    // ========================================
    else if (strstr(lang, "c++") != NULL || strstr(lang, "cpp") != NULL) {
        snprintf(plan, sizeof(plan),
            "/*\n"
            " * ============================================================\n"
            " *  HÀM: %s\n"
            " *  MÔ TẢ: %s\n"
            " *  NGÔN NGỮ: C++\n"
            " * ============================================================\n"
            " */\n\n"
            "#include <iostream>\n"
            "using namespace std;\n\n"
            "// ============================================================\n"
            "//  KẾ HOẠCH TRIỂN KHAI\n"
            "// ============================================================\n"
            "// Bước 1: Khai báo biến và tham số\n"
            "// Bước 2: Kiểm tra dữ liệu đầu vào\n"
            "// Bước 3: Xử lý logic chính\n"
            "// Bước 4: Trả về kết quả\n"
            "// ============================================================\n\n"
            "int %s(/* tham số */) {\n"
            "    // =========================================================\n"
            "    //  BƯỚC 1: Khai báo biến\n"
            "    // =========================================================\n"
            "    int result = 0;\n\n"
            "    // =========================================================\n"
            "    //  BƯỚC 2: Kiểm tra input\n"
            "    // =========================================================\n"
            "    // TODO: Kiểm tra dữ liệu đầu vào\n\n"
            "    // =========================================================\n"
            "    //  BƯỚC 3: Xử lý logic\n"
            "    // =========================================================\n"
            "    // TODO: Thêm logic xử lý ở đây\n"
            "    result = 0; // placeholder\n\n"
            "    // =========================================================\n"
            "    //  BƯỚC 4: Trả về kết quả\n"
            "    // =========================================================\n"
            "    return result;\n"
            "}\n\n"
            "int main() {\n"
            "    // =========================================================\n"
            "    //  TEST HÀM\n"
            "    // =========================================================\n"
            "    int test = %s(/* tham số test */);\n"
            "    cout << \"Kết quả: \" << test << endl;\n"
            "    return 0;\n"
            "}\n", clean_name, desc, clean_name, clean_name);
        return plan;
    }
    
    return NULL;
}

// ============================================
//  PHÁT HIỆN YÊU CẦU TẠO KẾ HOẠCH
// ============================================

int is_plan_request(const char *input) {
    if (!input) return 0;
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    const char *keywords[] = {
        "kế hoạch", "plan", "comment", "framework",
        "tạo khung", "skeleton", "outline", "dàn ý",
        "triển khai", "implement"
    };
    
    for (int i = 0; i < 9; i++) {
        if (strstr(lower, keywords[i]) != NULL) {
            return 1;
        }
    }
    return 0;
}

// ============================================
//  TRẢ LỜI YÊU CẦU
// ============================================

char* answer_plan_request(const char *input) {
    static char response[8192];
    response[0] = '\0';
    
    if (!is_plan_request(input)) {
        return NULL;
    }
    
    // Phân tích input
    char language[32] = "c";
    char func_name[64] = "ten_ham";
    char desc[256] = "Mô tả chức năng của hàm";
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    // Phát hiện ngôn ngữ
    if (strstr(lower, "python") != NULL || strstr(lower, "py") != NULL) {
        strcpy(language, "python");
    } else if (strstr(lower, "c++") != NULL || strstr(lower, "cpp") != NULL) {
        strcpy(language, "c++");
    } else if (strstr(lower, "c") != NULL) {
        strcpy(language, "c");
    }
    
    // Lấy tên hàm
    const char *patterns[] = {"tên", "hàm", "function", "func"};
    for (int i = 0; i < 4; i++) {
        char *pos = strstr(lower, patterns[i]);
        if (pos) {
            pos += strlen(patterns[i]);
            while (*pos == ' ') pos++;
            int j = 0;
            while (pos[j] != ' ' && pos[j] != '\0' && pos[j] != '.' && pos[j] != '?' && j < 63) {
                func_name[j] = pos[j];
                j++;
            }
            func_name[j] = '\0';
            if (strlen(func_name) > 0 && strcmp(func_name, patterns[i]) != 0) {
                break;
            }
        }
    }
    
    if (strlen(func_name) == 0 || strcmp(func_name, "là") == 0) {
        strcpy(func_name, "ten_ham");
    }
    
    // Lấy mô tả - lấy phần sau "làm", "thực hiện", "để"
    const char *desc_patterns[] = {"làm", "thực hiện", "để", "mô tả"};
    char desc_temp[256];
    desc_temp[0] = '\0';
    for (int i = 0; i < 4; i++) {
        char *pos = strstr(lower, desc_patterns[i]);
        if (pos) {
            pos += strlen(desc_patterns[i]);
            while (*pos == ' ') pos++;
            int j = 0;
            while (pos[j] != '\0' && pos[j] != '.' && pos[j] != '?' && j < 200) {
                desc_temp[j] = pos[j];
                j++;
            }
            desc_temp[j] = '\0';
            if (strlen(desc_temp) > 0 && strcmp(desc_temp, desc_patterns[i]) != 0) {
                strcpy(desc, desc_temp);
                break;
            }
        }
    }
    
    // Tạo kế hoạch
    char *plan = generate_plan(func_name, desc, language);
    if (plan) {
        snprintf(response, sizeof(response),
                 "🧩 Đã tạo kế hoạch triển khai hàm '%s' (%s):\n\n%s\n\n"
                 "💡 Hướng dẫn:\n"
                 "   1. Điền logic vào các bước TODO\n"
                 "   2. Thay đổi tham số cho phù hợp\n"
                 "   3. Test kỹ trước khi dùng",
                 func_name, language, plan);
    } else {
        snprintf(response, sizeof(response),
                 "❌ Không hỗ trợ ngôn ngữ '%s'!\n"
                 "💡 Hỗ trợ: C, Python, C++", language);
    }
    
    return response;
}
