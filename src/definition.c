#include "definition.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

#define MAX_DEFINITIONS 200

static Definition dict[MAX_DEFINITIONS];
int def_count = 0;  // THÊM: không còn static để brain.c có thể dùng

// ============================================
//  TỪ ĐIỂN ĐỊNH NGHĨA CƠ BẢN
// ============================================

static void init_definitions() {
    if (def_count > 0) return;
    
    // Công nghệ & AI
    add_definition("AI", "Trí tuệ nhân tạo, là khả năng của máy móc mô phỏng trí thông minh con người", "ChatGPT, Siri, CAI là các AI");
    add_definition("trí tuệ nhân tạo", "Là lĩnh vực khoa học máy tính tạo ra máy móc thông minh", "AI có thể học, suy luận và ra quyết định");
    add_definition("machine learning", "Học máy, là phương pháp để máy tính học từ dữ liệu mà không cần lập trình rõ ràng", "Máy học từ hàng nghìn bức ảnh để nhận diện mặt");
    add_definition("học máy", "Học máy, là phương pháp để máy tính học từ dữ liệu mà không cần lập trình rõ ràng", "Máy học từ hàng nghìn bức ảnh để nhận diện mặt");
    add_definition("deep learning", "Học sâu, là một nhánh của machine learning sử dụng mạng nơ-ron nhiều tầng", "Dùng để nhận dạng giọng nói, hình ảnh");
    add_definition("học sâu", "Học sâu, là một nhánh của machine learning sử dụng mạng nơ-ron nhiều tầng", "Dùng để nhận dạng giọng nói, hình ảnh");
    add_definition("neural network", "Mạng nơ-ron, là mô hình toán học mô phỏng bộ não con người", "Gồm nhiều tầng neuron kết nối với nhau");
    add_definition("mạng nơ ron", "Mạng nơ-ron, là mô hình toán học mô phỏng bộ não con người", "Gồm nhiều tầng neuron kết nối với nhau");
    add_definition("chatgpt", "Là một mô hình ngôn ngữ lớn do OpenAI phát triển, có thể trò chuyện tự nhiên", "Nó được huấn luyện trên hàng tỷ từ");
    add_definition("c", "Ngôn ngữ lập trình cấu trúc, hiệu năng cao, được phát triển bởi Dennis Ritchie", "Linux, Windows kernel được viết bằng C");
    add_definition("c++", "Ngôn ngữ lập trình hướng đối tượng, mở rộng từ C, được phát triển bởi Bjarne Stroustrup", "Game AAA, Chrome được viết bằng C++");
    add_definition("python", "Ngôn ngữ lập trình cấp cao, dễ học, được dùng nhiều trong AI và data science", "TensorFlow, PyTorch viết bằng Python");
    
    // Khoa học & cuộc sống
    add_definition("vũ trụ", "Toàn bộ không gian và thời gian, chứa mọi vật chất và năng lượng", "Vũ trụ bao la với hàng tỷ thiên hà");
    add_definition("sự sống", "Là trạng thái của sinh vật có khả năng sinh trưởng, phát triển và sinh sản", "Con người, động vật, thực vật đều có sự sống");
    add_definition("tình yêu", "Là một cảm xúc mạnh mẽ của sự gắn bó, quan tâm và quyến luyến", "Tình yêu gia đình, tình yêu đôi lứa");
    add_definition("hạnh phúc", "Là trạng thái cảm xúc tích cực khi con người thỏa mãn các nhu cầu tinh thần và vật chất", "Hạnh phúc đến từ những điều nhỏ nhặt");
    add_definition("thành công", "Là đạt được mục tiêu hoặc kết quả mong muốn trong cuộc sống", "Thành công không phải là đích đến mà là hành trình");
    
    // Lập trình
    add_definition("algorithm", "Thuật toán, là một tập hợp các bước có thứ tự để giải quyết một vấn đề", "Sắp xếp, tìm kiếm là các thuật toán cơ bản");
    add_definition("thuật toán", "Thuật toán, là một tập hợp các bước có thứ tự để giải quyết một vấn đề", "Sắp xếp, tìm kiếm là các thuật toán cơ bản");
    add_definition("compiler", "Trình biên dịch, là chương trình dịch mã nguồn sang mã máy", "GCC là compiler của ngôn ngữ C");
    add_definition("trình biên dịch", "Trình biên dịch, là chương trình dịch mã nguồn sang mã máy", "GCC là compiler của ngôn ngữ C");
    add_definition("pointer", "Con trỏ, là biến lưu địa chỉ bộ nhớ của biến khác", "Con trỏ là tính năng mạnh mẽ của C");
    add_definition("con trỏ", "Con trỏ, là biến lưu địa chỉ bộ nhớ của biến khác", "Con trỏ là tính năng mạnh mẽ của C");
    add_definition("memory leak", "Rò rỉ bộ nhớ, là tình trạng chương trình không giải phóng bộ nhớ đã cấp phát", "Gây hao tốn tài nguyên và crash chương trình");
    add_definition("rò rỉ bộ nhớ", "Rò rỉ bộ nhớ, là tình trạng chương trình không giải phóng bộ nhớ đã cấp phát", "Gây hao tốn tài nguyên và crash chương trình");
    
    // Con người
    add_definition("con người", "Là loài linh trưởng thông minh nhất Trái Đất, có khả năng tư duy trừu tượng và sáng tạo", "Con người đã tạo ra văn minh và công nghệ");
    add_definition("dev", "Lập trình viên, là người viết mã nguồn để tạo ra phần mềm ứng dụng", "Dev có thể code cả ngày không biết mệt");
    add_definition("developer", "Lập trình viên, là người viết mã nguồn để tạo ra phần mềm ứng dụng", "Dev có thể code cả ngày không biết mệt");
    add_definition("lập trình viên", "Lập trình viên, là người viết mã nguồn để tạo ra phần mềm ứng dụng", "Dev có thể code cả ngày không biết mệt");
    add_definition("hacker", "Hacker, là người có kỹ năng lập trình và khám phá hệ thống máy tính", "Hacker có thể là white hat (tốt) hoặc black hat (xấu)");
}

// ============================================
//  HÀM QUẢN LÝ TỪ ĐIỂN
// ============================================

void add_definition(const char *keyword, const char *def, const char *example) {
    if (def_count >= MAX_DEFINITIONS) return;
    strcpy(dict[def_count].keyword, keyword);
    strcpy(dict[def_count].definition, def);
    strcpy(dict[def_count].example, example ? example : "Không có ví dụ");
    def_count++;
}

void load_definitions() {
    init_definitions();
}

void free_definitions() {
    // Không cần free vì là mảng tĩnh
}

// ============================================
//  HÀM XỬ LÝ CÂU HỎI "LÀ GÌ"
// ============================================

int is_definition_question(const char *input) {
    char lower[1000];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    const char *patterns[] = {
        "là gì",
        "là cái gì",
        "là thế nào",
        "là ai",
        "là như thế nào",
        "định nghĩa",
        "nghĩa là gì",
        "thế nào là",
        "gọi là gì",
        "được gọi là"
    };
    
    for (int i = 0; i < 10; i++) {
        if (strstr(lower, patterns[i]) != NULL) {
            return 1;
        }
    }
    return 0;
}

char* extract_keyword(const char *input) {
    static char keyword[MAX_KEYWORD_LEN];
    keyword[0] = '\0';
    
    char lower[1000];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    const char *question_words[] = {"là gì", "là cái gì", "là thế nào", "là ai", "thế nào là", "định nghĩa", "nghĩa là gì"};
    
    for (int i = 0; i < 7; i++) {
        char *pos = strstr(lower, question_words[i]);
        if (pos) {
            int len = pos - lower;
            if (len > 0) {
                while (len > 0 && lower[len - 1] == ' ') len--;
                strncpy(keyword, lower, len);
                keyword[len] = '\0';
                char *last_word = strrchr(keyword, ' ');
                if (last_word) {
                    strcpy(keyword, last_word + 1);
                }
                return keyword;
            }
        }
    }
    
    char *last = strrchr(lower, ' ');
    if (last) {
        strcpy(keyword, last + 1);
    } else {
        strcpy(keyword, lower);
    }
    
    return keyword;
}

char* get_definition(const char *keyword) {
    static char result[MAX_DEF_LEN * 2];
    result[0] = '\0';
    
    if (strlen(keyword) == 0) {
        return NULL;
    }
    
    for (int i = 0; i < def_count; i++) {
        if (strcasecmp(dict[i].keyword, keyword) == 0) {
            snprintf(result, sizeof(result), 
                     "📖 %s: %s\n💡 Ví dụ: %s", 
                     dict[i].keyword, dict[i].definition, dict[i].example);
            return result;
        }
    }
    
    return NULL;
}

char* answer_definition_question(const char *input) {
    static char answer[MAX_DEF_LEN * 2];
    answer[0] = '\0';
    
    if (!is_definition_question(input)) {
        return NULL;
    }
    
    char *keyword = extract_keyword(input);
    if (strlen(keyword) == 0) {
        snprintf(answer, sizeof(answer), "Bạn hỏi về cái gì thế? Nói rõ tên đi!");
        return answer;
    }
    
    char *def = get_definition(keyword);
    if (def) {
        return def;
    }
    
    snprintf(answer, sizeof(answer), 
             "🙈 Tôi chưa biết về '%s'!\n🔍 Hãy dạy tôi bằng cách nói:\n   'dạy tôi biết [từ] là [định nghĩa]'", 
             keyword);
    return answer;
}
