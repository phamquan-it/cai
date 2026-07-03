#include "knowledge.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_KNOWLEDGE 200

static KnowledgeItem knowledge[MAX_KNOWLEDGE];
static int knowledge_count = 0;

// ============================================
//  HÀM THÊM KIẾN THỨC (đặt lên đầu)
// ============================================
void add_knowledge(const char *keyword, const char *category, 
                   const char *desc, const char *example, const char *note) {
    if (knowledge_count >= MAX_KNOWLEDGE) return;
    strcpy(knowledge[knowledge_count].keyword, keyword);
    strcpy(knowledge[knowledge_count].category, category);
    strcpy(knowledge[knowledge_count].description, desc);
    strcpy(knowledge[knowledge_count].example, example ? example : "");
    strcpy(knowledge[knowledge_count].note, note ? note : "");
    knowledge_count++;
}

// ============================================
//  KHO KIẾN THỨC VỀ C
// ============================================
static void init_knowledge() {
    if (knowledge_count > 0) return;
    
    // Từ khóa C
    add_knowledge("int", "keyword", "Kiểu dữ liệu số nguyên, thường chiếm 4 bytes", "int age = 25;", "Có thể dùng short, long");
    add_knowledge("float", "keyword", "Kiểu dữ liệu số thực, chiếm 4 bytes", "float pi = 3.14;", "Độ chính xác ~6-7 chữ số");
    add_knowledge("char", "keyword", "Kiểu dữ liệu ký tự, chiếm 1 byte", "char grade = 'A';", "Dùng nháy đơn cho ký tự");
    add_knowledge("double", "keyword", "Kiểu số thực chính xác cao, chiếm 8 bytes", "double pi = 3.14159265359;", "Độ chính xác ~15-16 chữ số");
    add_knowledge("void", "keyword", "Kiểu rỗng, dùng khi hàm không trả về", "void print_hello() { printf(\"Hello\"); }", "Không thể khai báo biến void");
    add_knowledge("if", "keyword", "Câu lệnh điều kiện rẽ nhánh", "if (x > 0) { printf(\"Dương\"); }", "Kết hợp với else, else if");
    add_knowledge("else", "keyword", "Câu lệnh điều kiện khi if sai", "if (x > 0) {} else {}", "Luôn đi kèm với if");
    add_knowledge("while", "keyword", "Vòng lặp kiểm tra điều kiện trước", "while (i < 10) { i++; }", "Nếu sai ngay từ đầu, không chạy");
    add_knowledge("for", "keyword", "Vòng lặp với khởi tạo, điều kiện, tăng/giảm", "for (int i = 0; i < 10; i++) {}", "Có thể bỏ qua bất kỳ phần nào");
    add_knowledge("do while", "keyword", "Vòng lặp chạy ít nhất 1 lần rồi kiểm tra", "do { i++; } while (i < 10);", "Khác while ở chỗ chạy ít nhất 1 lần");
    add_knowledge("switch", "keyword", "Rẽ nhánh nhiều lựa chọn", "switch (x) { case 1: break; default: }", "Phải có break để không rơi vào case sau");
    add_knowledge("break", "keyword", "Thoát khỏi vòng lặp hoặc switch", "for (int i = 0; i < 10; i++) { if (i == 5) break; }", "Thoát ngay tại vị trí gọi");
    add_knowledge("continue", "keyword", "Bỏ qua phần còn lại và sang lần lặp tiếp", "if (i % 2 == 0) continue;", "Không thoát khỏi vòng lặp");
    add_knowledge("return", "keyword", "Trả về giá trị từ hàm", "int add(int a, int b) { return a + b; }", "Hàm void có thể return; để thoát sớm");
    add_knowledge("const", "keyword", "Biến không thể thay đổi giá trị", "const int MAX = 100;", "Thường dùng cho hằng số");
    add_knowledge("static", "keyword", "Biến tồn tại xuyên suốt chương trình", "static int count = 0;", "Giữ giá trị giữa các lần gọi hàm");
    add_knowledge("struct", "keyword", "Tạo kiểu dữ liệu mới gồm nhiều thành phần", "struct Student { char name[50]; int age; };", "Truy cập bằng dấu .");
    add_knowledge("typedef", "keyword", "Đặt bí danh cho kiểu dữ liệu", "typedef int integer; integer x = 10;", "Giúp code dễ đọc hơn");
    add_knowledge("enum", "keyword", "Tạo kiểu liệt kê các giá trị hằng", "enum Color { RED, GREEN, BLUE };", "Mặc định giá trị bắt đầu từ 0");
    
    // Cú pháp & khái niệm
    add_knowledge("con trỏ", "concept", "Biến lưu địa chỉ bộ nhớ của biến khác", "int x = 10; int *ptr = &x; printf(\"%d\", *ptr);", "Dùng * để khai báo, & để lấy địa chỉ");
    add_knowledge("mảng", "syntax", "Tập hợp các phần tử cùng kiểu trong bộ nhớ liên tiếp", "int arr[5] = {1, 2, 3, 4, 5};", "Chỉ số bắt đầu từ 0");
    add_knowledge("chuỗi ký tự", "syntax", "Mảng ký tự kết thúc bằng '\\0'", "char str[] = \"Hello\";", "Phải có null '\\0' ở cuối");
    add_knowledge("hàm", "concept", "Khối lệnh thực hiện một nhiệm vụ cụ thể", "int add(int a, int b) { return a + b; }", "Cần prototype nếu gọi trước khi định nghĩa");
    add_knowledge("include", "syntax", "Nhúng file header vào chương trình", "#include <stdio.h>", "<> cho hệ thống, \"\" cho tự tạo");
    add_knowledge("define", "syntax", "Định nghĩa macro", "#define PI 3.14", "Thay thế trước khi biên dịch");
    add_knowledge("printf", "function", "Hàm in ra màn hình", "printf(\"Hello %s\\n\", name);", "Định dạng: %d, %f, %c, %s, %p");
    add_knowledge("scanf", "function", "Hàm đọc dữ liệu từ bàn phím", "scanf(\"%d\", &age);", "Luôn dùng & trước biến");
}

void load_c_knowledge() {
    init_knowledge();
}

// ============================================
//  PHÁT HIỆN CÂU HỎI VỀ C
// ============================================
int is_c_knowledge_question(const char *input) {
    char lower[1000];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    const char *keywords[] = {
        "trong c", "c là", "c language", "ngôn ngữ c",
        "từ khóa", "cú pháp", "hàm trong c", "con trỏ",
        "mảng trong c", "chuỗi trong c", "struct", "typedef"
    };
    
    for (int i = 0; i < 12; i++) {
        if (strstr(lower, keywords[i]) != NULL) {
            return 1;
        }
    }
    
    const char *c_keywords[] = {"int", "float", "char", "double", "void", 
        "if", "else", "while", "for", "switch", "break", "continue", 
        "return", "const", "static", "struct", "typedef", "enum"};
    for (int i = 0; i < 18; i++) {
        if (strstr(lower, c_keywords[i]) != NULL) {
            return 1;
        }
    }
    
    return 0;
}

// ============================================
//  TRẢ LỜI CÂU HỎI VỀ C
// ============================================
char* answer_c_knowledge_question(const char *input) {
    static char answer[4096];
    answer[0] = '\0';
    
    if (!is_c_knowledge_question(input)) {
        return NULL;
    }
    
    // Trích xuất từ khóa
    char keyword[128];
    keyword[0] = '\0';
    
    char lower[1000];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    const char *patterns[] = {"từ khóa", "trong c", "cú pháp", "hàm"};
    for (int i = 0; i < 4; i++) {
        char *pos = strstr(lower, patterns[i]);
        if (pos) {
            pos += strlen(patterns[i]);
            while (*pos == ' ') pos++;
            int j = 0;
            while (pos[j] != ' ' && pos[j] != '\0' && pos[j] != '.' && pos[j] != '?' && j < 127) {
                keyword[j] = pos[j];
                j++;
            }
            keyword[j] = '\0';
            if (strlen(keyword) > 0) break;
        }
    }
    
    if (strlen(keyword) == 0) {
        char *last = strrchr(lower, ' ');
        if (last) {
            strcpy(keyword, last + 1);
        } else {
            strcpy(keyword, lower);
        }
    }
    
    char *knowledge_text = get_c_knowledge(keyword);
    if (knowledge_text) {
        return knowledge_text;
    }
    
    snprintf(answer, sizeof(answer),
             "📚 Tôi chưa có kiến thức về '%s' trong C!\n"
             "💡 Nhưng tôi biết về:\n"
             "   - Từ khóa: int, float, char, if, else, while, for\n"
             "   - Cú pháp: con trỏ, mảng, chuỗi, hàm, struct\n"
             "🔍 Hãy hỏi cụ thể hơn: 'từ khóa int trong C'",
             keyword);
    return answer;
}

char* get_c_knowledge(const char *keyword) {
    static char result[4096];
    result[0] = '\0';
    
    if (strlen(keyword) == 0) return NULL;
    
    for (int i = 0; i < knowledge_count; i++) {
        if (strcasestr(knowledge[i].keyword, keyword) != NULL || 
            strcasestr(keyword, knowledge[i].keyword) != NULL) {
            
            snprintf(result, sizeof(result),
                     "╔══════════════════════════════════════════════════╗\n"
                     "║  📚 %-43s ║\n"
                     "╠══════════════════════════════════════════════════╣\n"
                     "║  📂 %-43s ║\n"
                     "║  📖 %-43s ║\n"
                     "║  💻 %-43s ║\n"
                     "║  💡 %-43s ║\n"
                     "╚══════════════════════════════════════════════════╝",
                     knowledge[i].keyword,
                     knowledge[i].category,
                     knowledge[i].description,
                     knowledge[i].example,
                     knowledge[i].note);
            return result;
        }
    }
    
    return NULL;
}
