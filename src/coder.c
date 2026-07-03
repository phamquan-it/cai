#include "coder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ============================================
//  KHO CODE MẪU
// ============================================

static char* get_code_template(const char *language, const char *task) {
    static char code[4096];
    code[0] = '\0';
    
    // ========================================
    //  C LANGUAGE
    // ========================================
    if (strcasecmp(language, "c") == 0 || strstr(language, "c") != NULL) {
        if (strstr(task, "hello") || strstr(task, "xin chào")) {
            snprintf(code, sizeof(code),
                "// Chương trình C: Hello World\n"
                "#include <stdio.h>\n\n"
                "int main() {\n"
                "    printf(\"Xin chào! Tôi là CAI viết bằng C!\\n\");\n"
                "    return 0;\n"
                "}\n");
            return code;
        }
        else if (strstr(task, "tính tổng") || strstr(task, "sum")) {
            snprintf(code, sizeof(code),
                "// Chương trình C: Tính tổng 2 số\n"
                "#include <stdio.h>\n\n"
                "int main() {\n"
                "    int a, b, sum;\n"
                "    printf(\"Nhập số thứ nhất: \");\n"
                "    scanf(\"%%d\", &a);\n"
                "    printf(\"Nhập số thứ hai: \");\n"
                "    scanf(\"%%d\", &b);\n"
                "    sum = a + b;\n"
                "    printf(\"Tổng = %%d\\n\", sum);\n"
                "    return 0;\n"
                "}\n");
            return code;
        }
        else if (strstr(task, "fibonacci")) {
            snprintf(code, sizeof(code),
                "// Chương trình C: Dãy Fibonacci\n"
                "#include <stdio.h>\n\n"
                "int fibonacci(int n) {\n"
                "    if (n <= 1) return n;\n"
                "    return fibonacci(n-1) + fibonacci(n-2);\n"
                "}\n\n"
                "int main() {\n"
                "    int n;\n"
                "    printf(\"Nhập n: \");\n"
                "    scanf(\"%%d\", &n);\n"
                "    printf(\"Fibonacci(%%d) = %%d\\n\", n, fibonacci(n));\n"
                "    return 0;\n"
                "}\n");
            return code;
        }
        else {
            snprintf(code, sizeof(code),
                "// Code C mẫu\n"
                "#include <stdio.h>\n\n"
                "int main() {\n"
                "    // Code của bạn ở đây\n"
                "    printf(\"Hello, World!\\n\");\n"
                "    return 0;\n"
                "}\n");
            return code;
        }
    }
    
    // ========================================
    //  PYTHON
    // ========================================
    else if (strcasecmp(language, "python") == 0 || strstr(language, "py") != NULL) {
        if (strstr(task, "hello") || strstr(task, "xin chào")) {
            snprintf(code, sizeof(code),
                "# Chương trình Python: Hello World\n"
                "print(\"Xin chào! Tôi là CAI viết bằng Python!\")\n");
            return code;
        }
        else if (strstr(task, "tính tổng") || strstr(task, "sum")) {
            snprintf(code, sizeof(code),
                "# Chương trình Python: Tính tổng 2 số\n"
                "a = int(input(\"Nhập số thứ nhất: \"))\n"
                "b = int(input(\"Nhập số thứ hai: \"))\n"
                "print(f\"Tổng = {a + b}\")\n");
            return code;
        }
        else if (strstr(task, "fibonacci")) {
            snprintf(code, sizeof(code),
                "# Chương trình Python: Dãy Fibonacci\n"
                "def fibonacci(n):\n"
                "    if n <= 1:\n"
                "        return n\n"
                "    return fibonacci(n-1) + fibonacci(n-2)\n\n"
                "n = int(input(\"Nhập n: \"))\n"
                "print(f\"Fibonacci({n}) = {fibonacci(n)}\")\n");
            return code;
        }
        else {
            snprintf(code, sizeof(code),
                "# Code Python mẫu\n"
                "print(\"Hello, World!\")\n");
            return code;
        }
    }
    
    // ========================================
    //  C++
    // ========================================
    else if (strstr(language, "c++") != NULL || strstr(language, "cpp") != NULL) {
        if (strstr(task, "hello") || strstr(task, "xin chào")) {
            snprintf(code, sizeof(code),
                "// Chương trình C++: Hello World\n"
                "#include <iostream>\n"
                "using namespace std;\n\n"
                "int main() {\n"
                "    cout << \"Xin chào! Tôi là CAI viết bằng C++!\" << endl;\n"
                "    return 0;\n"
                "}\n");
            return code;
        }
        else {
            snprintf(code, sizeof(code),
                "// Code C++ mẫu\n"
                "#include <iostream>\n"
                "using namespace std;\n\n"
                "int main() {\n"
                "    cout << \"Hello, World!\" << endl;\n"
                "    return 0;\n"
                "}\n");
            return code;
        }
    }
    
    // ========================================
    //  HTML
    // ========================================
    else if (strstr(language, "html") != NULL) {
        snprintf(code, sizeof(code),
            "<!-- Code HTML mẫu -->\n"
            "<!DOCTYPE html>\n"
            "<html>\n"
            "<head>\n"
            "    <title>Trang web của CAI</title>\n"
            "</head>\n"
            "<body>\n"
            "    <h1>Chào mừng đến với CAI!</h1>\n"
            "    <p>Trí tuệ nhân tạo viết bằng C</p>\n"
            "</body>\n"
            "</html>\n");
        return code;
    }
    
    return NULL;
}

// ============================================
//  PHÁT HIỆN YÊU CẦU CODE
// ============================================

int is_coding_request(const char *input) {
    char lower[1000];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    const char *keywords[] = {
        "viết code", "code", "chương trình", "program",
        "hàm", "function", "viết hàm", "tạo hàm",
        "in ra", "print", "hello world"
    };
    
    for (int i = 0; i < 10; i++) {
        if (strstr(lower, keywords[i]) != NULL) {
            return 1;
        }
    }
    return 0;
}

// ============================================
//  TRÍCH XUẤT NGÔN NGỮ VÀ YÊU CẦU
// ============================================

static void extract_coding_params(const char *input, char *language, char *task) {
    language[0] = '\0';
    task[0] = '\0';
    
    char lower[1000];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    // Phát hiện ngôn ngữ
    const char *langs[] = {"c", "c++", "cpp", "python", "py", "java", "html", "javascript", "js"};
    for (int i = 0; i < 9; i++) {
        if (strstr(lower, langs[i]) != NULL) {
            strcpy(language, langs[i]);
            break;
        }
    }
    
    // Nếu không có, mặc định C
    if (strlen(language) == 0) {
        strcpy(language, "c");
    }
    
    // Trích xuất task (lấy phần còn lại)
    char *pos = strstr(lower, "viết");
    if (pos) {
        strcpy(task, pos + strlen("viết"));
    } else {
        strcpy(task, input);
    }
}

// ============================================
//  TRẢ LỜI YÊU CẦU CODE
// ============================================

char* answer_coding_request(const char *input) {
    static char response[4096];
    response[0] = '\0';
    
    if (!is_coding_request(input)) {
        return NULL;
    }
    
    char language[32];
    char task[512];
    extract_coding_params(input, language, task);
    
    char *code = get_code_template(language, task);
    if (code) {
        snprintf(response, sizeof(response),
                 "🧑‍💻 Đây là code %s cho yêu cầu của bạn:\n\n```%s\n%s\n```\n\n"
                 "💡 Lưu ý: Đây là code mẫu, bạn có thể chỉnh sửa cho phù hợp!",
                 language, language, code);
        return response;
    }
    
    snprintf(response, sizeof(response),
             "🧑‍💻 Tôi chưa có code mẫu cho ngôn ngữ '%s' và task '%s'!\n"
             "🔍 Hãy thử yêu cầu: 'viết code C in ra Hello World'",
             language, task);
    return response;
}
