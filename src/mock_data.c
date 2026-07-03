#include "mock_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_MOCK 50

static MockFunction mock_functions[MAX_MOCK];
static int mock_count = 0;

// ============================================
//  KHO MOCK DATA (CODE MẪU ĐÚNG)
// ============================================

void add_mock(const char *name, const char *language, const char *code) {
    if (mock_count >= MAX_MOCK) return;
    strcpy(mock_functions[mock_count].name, name);
    strcpy(mock_functions[mock_count].language, language);
    strcpy(mock_functions[mock_count].template_code, code);
    mock_functions[mock_count].placeholder_count = 0;
    mock_count++;
}

static void init_mock_data() {
    if (mock_count > 0) return;
    
    // ========================================
    //  C - TÍNH TỔNG (CODE HOÀN CHỈNH)
    // ========================================
    add_mock("tinh_tong", "c",
        "#include <stdio.h>\n\n"
        "int tinh_tong(int a, int b) {\n"
        "    int ketqua = a + b;\n"
        "    return ketqua;\n"
        "}\n\n"
        "int main() {\n"
        "    int a = 5, b = 3;\n"
        "    printf(\"Tong: %d\\n\", tinh_tong(a, b));\n"
        "    return 0;\n"
        "}\n");
    
    // ========================================
    //  C - TÍNH NHÂN
    // ========================================
    add_mock("tinh_nhan", "c",
        "#include <stdio.h>\n\n"
        "int tinh_nhan(int a, int b) {\n"
        "    int ketqua = a * b;\n"
        "    return ketqua;\n"
        "}\n\n"
        "int main() {\n"
        "    int a = 5, b = 3;\n"
        "    printf(\"Tich: %d\\n\", tinh_nhan(a, b));\n"
        "    return 0;\n"
        "}\n");
    
    // ========================================
    //  C - FIBONACCI
    // ========================================
    add_mock("fibonacci", "c",
        "#include <stdio.h>\n\n"
        "int fibonacci(int n) {\n"
        "    if (n <= 1) return n;\n"
        "    return fibonacci(n-1) + fibonacci(n-2);\n"
        "}\n\n"
        "int main() {\n"
        "    int n = 10;\n"
        "    printf(\"Fibonacci(%d) = %d\\n\", n, fibonacci(n));\n"
        "    return 0;\n"
        "}\n");
    
    // ========================================
    //  C - TÌM MAX
    // ========================================
    add_mock("tim_max", "c",
        "#include <stdio.h>\n\n"
        "int tim_max(int arr[], int n) {\n"
        "    int max = arr[0];\n"
        "    for (int i = 1; i < n; i++) {\n"
        "        if (arr[i] > max) max = arr[i];\n"
        "    }\n"
        "    return max;\n"
        "}\n\n"
        "int main() {\n"
        "    int arr[] = {1, 5, 3, 8, 2};\n"
        "    int n = 5;\n"
        "    printf(\"Max: %d\\n\", tim_max(arr, n));\n"
        "    return 0;\n"
        "}\n");
    
    // ========================================
    //  Python - TÍNH TỔNG
    // ========================================
    add_mock("tinh_tong", "python",
        "def tinh_tong(a, b):\n"
        "    ketqua = a + b\n"
        "    return ketqua\n\n"
        "if __name__ == \"__main__\":\n"
        "    a, b = 5, 3\n"
        "    print(f\"Tong: {tinh_tong(a, b)}\")\n");
    
    // ========================================
    //  Python - FIBONACCI
    // ========================================
    add_mock("fibonacci", "python",
        "def fibonacci(n):\n"
        "    if n <= 1:\n"
        "        return n\n"
        "    return fibonacci(n-1) + fibonacci(n-2)\n\n"
        "if __name__ == \"__main__\":\n"
        "    n = 10\n"
        "    print(f\"Fibonacci({n}) = {fibonacci(n)}\")\n");
    
    // ========================================
    //  C++ - TÍNH TỔNG
    // ========================================
    add_mock("tinh_tong", "cpp",
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int tinh_tong(int a, int b) {\n"
        "    int ketqua = a + b;\n"
        "    return ketqua;\n"
        "}\n\n"
        "int main() {\n"
        "    int a = 5, b = 3;\n"
        "    cout << \"Tong: \" << tinh_tong(a, b) << endl;\n"
        "    return 0;\n"
        "}\n");
}

void load_mock_data() {
    init_mock_data();
}

// ============================================
//  TÌM CODE MẪU
// ============================================

char* get_mock_code(const char *func_name, const char *language) {
    static char result[4096];
    result[0] = '\0';
    
    if (!func_name || !language) return NULL;
    
    for (int i = 0; i < mock_count; i++) {
        if (strcasestr(mock_functions[i].name, func_name) != NULL ||
            strcasestr(func_name, mock_functions[i].name) != NULL) {
            if (strcasestr(mock_functions[i].language, language) != NULL ||
                strcasestr(language, mock_functions[i].language) != NULL) {
                strcpy(result, mock_functions[i].template_code);
                return result;
            }
        }
    }
    
    // Thử tìm theo tên không cần ngôn ngữ
    for (int i = 0; i < mock_count; i++) {
        if (strcasestr(mock_functions[i].name, func_name) != NULL ||
            strcasestr(func_name, mock_functions[i].name) != NULL) {
            strcpy(result, mock_functions[i].template_code);
            return result;
        }
    }
    
    return NULL;
}

// ============================================
//  HÀM THAY THẾ CODE TRONG KẾ HOẠCH
// ============================================

char* replace_code_in_plan(const char *plan, const char *func_name, const char *language) {
    static char result[8192];
    result[0] = '\0';
    
    char *mock_code = get_mock_code(func_name, language);
    if (!mock_code) return NULL;
    
    // Tìm vị trí "// TODO: CODE VÀO ĐÂY" hoặc tương tự
    char *pos = strstr(plan, "// TODO: CODE VÀO ĐÂY");
    if (!pos) {
        // Thử tìm các pattern khác
        pos = strstr(plan, "# TODO: CODE VÀO ĐÂY");
    }
    if (!pos) {
        pos = strstr(plan, "// TODO:");
    }
    if (!pos) {
        pos = strstr(plan, "# TODO:");
    }
    
    if (pos) {
        // Lấy phần trước và sau TODO
        char before[4096], after[4096];
        *pos = '\0';
        strcpy(before, plan);
        *pos = '/'; // Khôi phục
        strcpy(after, pos + strlen("// TODO: CODE VÀO ĐÂY"));
        
        // Tìm dòng chứa mock code và chỉ lấy phần logic
        char logic[4096];
        logic[0] = '\0';
        char *mock_start = strstr(mock_code, "int tinh_tong");
        if (!mock_start) mock_start = strstr(mock_code, "def tinh_tong");
        if (!mock_start) mock_start = mock_code;
        
        // Tìm phần thân hàm
        char *body_start = strchr(mock_start, '{');
        if (!body_start) body_start = strchr(mock_start, ':');
        if (body_start) {
            body_start++;
            char *body_end = strrchr(body_start, '}');
            if (!body_end) body_end = strchr(body_start, '\0');
            
            char temp[2048];
            strncpy(temp, body_start, body_end - body_start);
            temp[body_end - body_start] = '\0';
            // Loại bỏ dấu { } thừa
            char *trim = temp;
            while (*trim == ' ' || *trim == '\n' || *trim == '\r') trim++;
            strcpy(logic, trim);
        } else {
            strcpy(logic, mock_code);
        }
        
        // Ghép lại
        snprintf(result, sizeof(result), "%s%s%s", before, logic, after);
        return result;
    }
    
    // Nếu không tìm thấy TODO, trả về code mẫu
    return mock_code;
}

// ============================================
//  PHÁT HIỆN VÀ TRẢ LỜI
// ============================================

int is_mock_request(const char *input) {
    if (!input) return 0;
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    const char *keywords[] = {
        "chèn code", "điền code", "mock", "code mẫu", 
        "thay code", "gắn code", "code vào"
    };
    
    for (int i = 0; i < 7; i++) {
        if (strstr(lower, keywords[i]) != NULL) {
            return 1;
        }
    }
    return 0;
}

char* answer_mock_request(const char *input) {
    static char response[8192];
    response[0] = '\0';
    
    if (!is_mock_request(input)) {
        return NULL;
    }
    
    // Phân tích input
    char language[32] = "c";
    char func_name[64] = "tinh_tong";
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    // Phát hiện ngôn ngữ
    if (strstr(lower, "python") != NULL || strstr(lower, "py") != NULL) {
        strcpy(language, "python");
    } else if (strstr(lower, "c++") != NULL || strstr(lower, "cpp") != NULL) {
        strcpy(language, "cpp");
    } else if (strstr(lower, "c") != NULL) {
        strcpy(language, "c");
    }
    
    // Lấy tên hàm
    if (strstr(lower, "tổng") != NULL || strstr(lower, "sum") != NULL) {
        strcpy(func_name, "tinh_tong");
    } else if (strstr(lower, "nhân") != NULL || strstr(lower, "mul") != NULL) {
        strcpy(func_name, "tinh_nhan");
    } else if (strstr(lower, "fibonacci") != NULL || strstr(lower, "fibo") != NULL) {
        strcpy(func_name, "fibonacci");
    } else if (strstr(lower, "max") != NULL) {
        strcpy(func_name, "tim_max");
    }
    
    // Lấy code mẫu
    char *mock = get_mock_code(func_name, language);
    if (mock) {
        snprintf(response, sizeof(response),
                 "🧩 Đã chèn code mẫu cho '%s' vào vị trí TODO:\n\n```%s\n%s\n```\n\n"
                 "✅ Code đã sẵn sàng để chạy!\n"
                 "💡 Bạn có thể sửa đổi logic nếu cần.",
                 func_name, language, mock);
    } else {
        snprintf(response, sizeof(response),
                 "❌ Không có code mẫu cho '%s' trong %s!\n"
                 "💡 Code mẫu có sẵn:\n"
                 "   - tinh_tong (C, Python, C++)\n"
                 "   - tinh_nhan (C)\n"
                 "   - fibonacci (C, Python)\n"
                 "   - tim_max (C)\n"
                 "🔍 Yêu cầu: 'chèn code tinh_tong vào kế hoạch C'",
                 func_name, language);
    }
    
    return response;
}
