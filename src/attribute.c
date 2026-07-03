#include "attribute.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>

// ============================================
//  DANH SÁCH TỪ KHÓA THUỘC TÍNH
// ============================================

typedef struct {
    const char *keywords[10];
    const char *attr_name;
} AttributePattern;

static AttributePattern attr_patterns[] = {
    // Màu sắc
    {{"màu", "màu sắc", "màu gì", "hồng", "đỏ", "xanh", "vàng", "trắng", "đen", "tím"}, "màu sắc"},
    
    // Kích thước
    {{"to", "nhỏ", "vừa", "lớn", "bé", "khổng lồ", "tí hon", "cao", "thấp", "rộng", "hẹp", "dài", "ngắn"}, "kích thước"},
    
    // Cân nặng
    {{"nặng", "nhẹ", "cân nặng", "nặng bao nhiêu", "nhẹ bao nhiêu", "kg", "gram"}, "cân nặng"},
    
    // Giá cả
    {{"giá", "tiền", "bao nhiêu tiền", "đắt", "rẻ", "mắc", "giá cả", "thành tiền"}, "giá cả"},
    
    // Chất lượng
    {{"tốt", "xấu", "dở", "chất lượng", "ổn", "tệ", "tuyệt vời", "hoàn hảo", "hỏng", "mới", "cũ"}, "chất lượng"},
    
    // Cảm xúc
    {{"vui", "buồn", "hạnh phúc", "cô đơn", "tức giận", "thất vọng", "hào hứng", "lo lắng"}, "cảm xúc"},
    
    // Vị trí
    {{"ở đâu", "đâu", "chỗ nào", "vị trí", "nơi", "địa chỉ", "phía", "gần", "xa"}, "vị trí"},
    
    // Hình dạng
    {{"tròn", "vuông", "tam giác", "chữ nhật", "bầu dục", "cong", "thẳng", "xoắn"}, "hình dạng"},
    
    // Mùi vị
    {{"mùi", "vị", "thơm", "hôi", "chua", "ngọt", "mặn", "đắng", "cay"}, "mùi vị"},
    
    // Chất liệu
    {{"chất liệu", "làm từ", "gỗ", "sắt", "thép", "nhựa", "da", "vải", "kim loại", "gốm"}, "chất liệu"},
};

#define NUM_ATTR_PATTERNS (sizeof(attr_patterns) / sizeof(attr_patterns[0]))

// ============================================
//  HÀM TRÍCH XUẤT
// ============================================

// Tìm tên sự vật (object)
static void extract_object(const char *input, char *object) {
    object[0] = '\0';
    
    // Tìm sau các từ khóa: "cái", "con", "chiếc", "món", "cuốn"
    const char *prefixes[] = {"cái", "con", "chiếc", "món", "cuốn", "tờ", "quyển", "bức"};
    char temp[1000];
    strcpy(temp, input);
    
    for (int i = 0; i < 8; i++) {
        char *pos = strstr(temp, prefixes[i]);
        if (pos) {
            pos += strlen(prefixes[i]);
            while (*pos == ' ') pos++;
            
            int j = 0;
            while (pos[j] != ' ' && pos[j] != '.' && pos[j] != '?' && pos[j] != ',' && pos[j] != '\0' && j < MAX_OBJECT_NAME_LEN - 1) {
                object[j] = pos[j];
                j++;
            }
            object[j] = '\0';
            
            // Nếu tìm thấy object, thoát
            if (strlen(object) > 0) break;
        }
    }
    
    // Nếu không có prefix, lấy từ đầu tiên sau từ "về" hoặc "của"
    if (strlen(object) == 0) {
        char *about = strstr(temp, "về");
        if (about) {
            about += strlen("về");
            while (*about == ' ') about++;
            int j = 0;
            while (about[j] != ' ' && about[j] != '.' && about[j] != '?' && about[j] != '\0' && j < MAX_OBJECT_NAME_LEN - 1) {
                object[j] = about[j];
                j++;
            }
            object[j] = '\0';
        }
    }
    
    // Nếu vẫn không có, lấy từ cuối cùng
    if (strlen(object) == 0) {
        char *last = strrchr(temp, ' ');
        if (last) {
            strcpy(object, last + 1);
        } else {
            strcpy(object, "vật");
        }
    }
}

// Kiểm tra từ có phải thuộc tính không
static int is_attribute_word(const char *word, const char **attr_name) {
    for (int i = 0; i < NUM_ATTR_PATTERNS; i++) {
        for (int j = 0; j < 10 && attr_patterns[i].keywords[j] != NULL; j++) {
            if (strcasecmp(word, attr_patterns[i].keywords[j]) == 0) {
                *attr_name = attr_patterns[i].attr_name;
                return 1;
            }
        }
    }
    return 0;
}

// Thêm thuộc tính vào object
static void add_attribute(ObjectAttributes *obj, const char *name, const char *value) {
    if (obj->count >= MAX_ATTRIBUTES) return;
    
    // Kiểm tra trùng
    for (int i = 0; i < obj->count; i++) {
        if (strcmp(obj->attrs[i].name, name) == 0) {
            // Cập nhật nếu trùng
            strcpy(obj->attrs[i].value, value);
            obj->attrs[i].confidence = 0.9;
            return;
        }
    }
    
    // Thêm mới
    strcpy(obj->attrs[obj->count].name, name);
    strcpy(obj->attrs[obj->count].value, value);
    obj->attrs[obj->count].confidence = 0.85;
    obj->count++;
}

// Hàm chính: trích xuất thuộc tính
ObjectAttributes* extract_attributes(const char *input) {
    ObjectAttributes *obj = (ObjectAttributes*)malloc(sizeof(ObjectAttributes));
    if (!obj) return NULL;
    
    obj->count = 0;
    for (int i = 0; i < MAX_ATTRIBUTES; i++) {
        obj->attrs[i].name[0] = '\0';
        obj->attrs[i].value[0] = '\0';
        obj->attrs[i].confidence = 0.0;
    }
    
    // Trích xuất object
    extract_object(input, obj->object);
    if (strlen(obj->object) == 0) {
        strcpy(obj->object, "vật");
    }
    
    // Tokenize và tìm thuộc tính
    char temp[1000];
    strcpy(temp, input);
    char *words[100];
    int word_count = 0;
    
    char *token = strtok(temp, " .,!?;:\n\t\"'()[]{}");
    while (token != NULL && word_count < 100) {
        words[word_count] = strdup(token);
        word_count++;
        token = strtok(NULL, " .,!?;:\n\t\"'()[]{}");
    }
    
    // Duyệt từng cặp từ (i, i+1) để tìm thuộc tính + giá trị
    for (int i = 0; i < word_count - 1; i++) {
        const char *attr_name = NULL;
        if (is_attribute_word(words[i], &attr_name)) {
            // Lấy từ tiếp theo làm giá trị
            char value[MAX_ATTR_VALUE_LEN];
            strcpy(value, words[i + 1]);
            
            // Loại bỏ dấu câu trong value
            for (int j = 0; value[j]; j++) {
                if (!isalpha(value[j]) && !isdigit(value[j]) && value[j] != ' ') {
                    value[j] = '\0';
                    break;
                }
            }
            
            if (strlen(value) > 0) {
                add_attribute(obj, attr_name, value);
            }
        }
        // Kiểm tra nếu từ i+1 là thuộc tính, từ i là giá trị
        else if (i > 0 && is_attribute_word(words[i], &attr_name)) {
            // Tương tự
            char value[MAX_ATTR_VALUE_LEN];
            strcpy(value, words[i - 1]);
            for (int j = 0; value[j]; j++) {
                if (!isalpha(value[j]) && !isdigit(value[j]) && value[j] != ' ') {
                    value[j] = '\0';
                    break;
                }
            }
            if (strlen(value) > 0) {
                add_attribute(obj, attr_name, value);
            }
        }
    }
    
    // Tìm các cụm "thuộc tính + giá trị" dạng "màu đỏ", "to bằng"
    for (int i = 0; i < word_count - 1; i++) {
        // Nếu có "màu" + [từ màu]
        if (strcasecmp(words[i], "màu") == 0) {
            const char *color_words[] = {"đỏ", "xanh", "vàng", "trắng", "đen", "tím", "hồng", "cam", "nâu", "xám"};
            for (int c = 0; c < 10; c++) {
                if (strcasecmp(words[i+1], color_words[c]) == 0) {
                    add_attribute(obj, "màu sắc", color_words[c]);
                    break;
                }
            }
        }
        
        // Nếu có "nặng" + [số + kg] 
        if (strcasecmp(words[i], "nặng") == 0 && i+2 < word_count) {
            if (isdigit(words[i+1][0]) && strcasecmp(words[i+2], "kg") == 0) {
                char value[32];
                snprintf(value, sizeof(value), "%s kg", words[i+1]);
                add_attribute(obj, "cân nặng", value);
            }
        }
        
        // Nếu có "giá" + [số + ...]
        if (strcasecmp(words[i], "giá") == 0 && i+1 < word_count) {
            if (isdigit(words[i+1][0])) {
                char value[32];
                snprintf(value, sizeof(value), "%s", words[i+1]);
                if (i+2 < word_count && strcasecmp(words[i+2], "triệu") == 0) {
                    strcat(value, " triệu");
                } else if (i+2 < word_count && strcasecmp(words[i+2], "ngàn") == 0) {
                    strcat(value, " ngàn");
                }
                add_attribute(obj, "giá cả", value);
            }
        }
    }
    
    // Giải phóng bộ nhớ
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
    
    return obj;
}

void free_attributes(ObjectAttributes *obj) {
    if (obj) free(obj);
}

// ============================================
//  HÀM SINH MÔ TẢ
// ============================================

char* describe_object(ObjectAttributes *obj) {
    static char description[1000];
    description[0] = '\0';
    
    if (obj->count == 0) {
        snprintf(description, sizeof(description), "Tôi chưa có thông tin gì về %s.", obj->object);
        return description;
    }
    
    strcpy(description, obj->object);
    strcat(description, " có ");
    
    for (int i = 0; i < obj->count; i++) {
        if (i > 0) strcat(description, ", ");
        strcat(description, obj->attrs[i].name);
        strcat(description, " ");
        strcat(description, obj->attrs[i].value);
    }
    strcat(description, ".");
    
    return description;
}

char* generate_attribute_response(ObjectAttributes *obj) {
    static char response[1000];
    response[0] = '\0';
    
    if (obj->count == 0) {
        snprintf(response, sizeof(response), 
                 "Bạn đang nói về %s hả? Tôi chưa có thông tin gì về nó.", 
                 obj->object);
        return response;
    }
    
    // Tạo response tự nhiên
    strcpy(response, "Ah, ");
    strcat(response, obj->object);
    strcat(response, " à! ");
    
    // Thêm các thuộc tính
    for (int i = 0; i < obj->count; i++) {
        if (strcmp(obj->attrs[i].name, "màu sắc") == 0) {
            strcat(response, "Nó có màu ");
            strcat(response, obj->attrs[i].value);
            strcat(response, ". ");
        } else if (strcmp(obj->attrs[i].name, "kích thước") == 0) {
            strcat(response, "Nó có kích thước ");
            strcat(response, obj->attrs[i].value);
            strcat(response, ". ");
        } else if (strcmp(obj->attrs[i].name, "cân nặng") == 0) {
            strcat(response, "Nó nặng ");
            strcat(response, obj->attrs[i].value);
            strcat(response, ". ");
        } else if (strcmp(obj->attrs[i].name, "giá cả") == 0) {
            strcat(response, "Giá của nó là ");
            strcat(response, obj->attrs[i].value);
            strcat(response, ". ");
        } else if (strcmp(obj->attrs[i].name, "chất lượng") == 0) {
            strcat(response, "Chất lượng của nó thì ");
            strcat(response, obj->attrs[i].value);
            strcat(response, ". ");
        } else if (strcmp(obj->attrs[i].name, "cảm xúc") == 0) {
            strcat(response, "Bạn đang ");
            strcat(response, obj->attrs[i].value);
            strcat(response, " phải không? ");
        } else {
            strcat(response, obj->attrs[i].name);
            strcat(response, " là ");
            strcat(response, obj->attrs[i].value);
            strcat(response, ". ");
        }
    }
    
    // Thêm câu hỏi mở
    if (obj->count > 0) {
        strcat(response, "Còn gì khác về ");
        strcat(response, obj->object);
        strcat(response, " bạn muốn hỏi không?");
    }
    
    return response;
}
