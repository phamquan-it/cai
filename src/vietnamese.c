#include "vietnamese.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

// ============================================
//  HÀM CHUYỂN ĐỔI DẤU TIẾNG VIỆT (UTF-8 safe)
// ============================================

// Mảng thay thế: chuỗi có dấu -> chuỗi không dấu
static const char* accent_replacements[][2] = {
    {"á", "a"}, {"à", "a"}, {"ả", "a"}, {"ã", "a"}, {"ạ", "a"},
    {"ă", "a"}, {"ắ", "a"}, {"ằ", "a"}, {"ẳ", "a"}, {"ẵ", "a"}, {"ặ", "a"},
    {"â", "a"}, {"ấ", "a"}, {"ầ", "a"}, {"ẩ", "a"}, {"ẫ", "a"}, {"ậ", "a"},
    {"é", "e"}, {"è", "e"}, {"ẻ", "e"}, {"ẽ", "e"}, {"ẹ", "e"},
    {"ê", "e"}, {"ế", "e"}, {"ề", "e"}, {"ể", "e"}, {"ễ", "e"}, {"ệ", "e"},
    {"í", "i"}, {"ì", "i"}, {"ỉ", "i"}, {"ĩ", "i"}, {"ị", "i"},
    {"ó", "o"}, {"ò", "o"}, {"ỏ", "o"}, {"õ", "o"}, {"ọ", "o"},
    {"ô", "o"}, {"ố", "o"}, {"ồ", "o"}, {"ổ", "o"}, {"ỗ", "o"}, {"ộ", "o"},
    {"ơ", "o"}, {"ớ", "o"}, {"ờ", "o"}, {"ở", "o"}, {"ỡ", "o"}, {"ợ", "o"},
    {"ú", "u"}, {"ù", "u"}, {"ủ", "u"}, {"ũ", "u"}, {"ụ", "u"},
    {"ư", "u"}, {"ứ", "u"}, {"ừ", "u"}, {"ử", "u"}, {"ữ", "u"}, {"ự", "u"},
    {"ý", "y"}, {"ỳ", "y"}, {"ỷ", "y"}, {"ỹ", "y"}, {"ỵ", "y"},
    {"đ", "d"},
    {"Á", "A"}, {"À", "A"}, {"Ả", "A"}, {"Ã", "A"}, {"Ạ", "A"},
    {"Ă", "A"}, {"Ắ", "A"}, {"Ằ", "A"}, {"Ẳ", "A"}, {"Ẵ", "A"}, {"Ặ", "A"},
    {"Â", "A"}, {"Ấ", "A"}, {"Ầ", "A"}, {"Ẩ", "A"}, {"Ẫ", "A"}, {"Ậ", "A"},
    {"É", "E"}, {"È", "E"}, {"Ẻ", "E"}, {"Ẽ", "E"}, {"Ẹ", "E"},
    {"Ê", "E"}, {"Ế", "E"}, {"Ề", "E"}, {"Ể", "E"}, {"Ễ", "E"}, {"Ệ", "E"},
    {"Í", "I"}, {"Ì", "I"}, {"Ỉ", "I"}, {"Ĩ", "I"}, {"Ị", "I"},
    {"Ó", "O"}, {"Ò", "O"}, {"Ỏ", "O"}, {"Õ", "O"}, {"Ọ", "O"},
    {"Ô", "O"}, {"Ố", "O"}, {"Ồ", "O"}, {"Ổ", "O"}, {"Ỗ", "O"}, {"Ộ", "O"},
    {"Ơ", "O"}, {"Ớ", "O"}, {"Ờ", "O"}, {"Ở", "O"}, {"Ỡ", "O"}, {"Ợ", "O"},
    {"Ú", "U"}, {"Ù", "U"}, {"Ủ", "U"}, {"Ũ", "U"}, {"Ụ", "U"},
    {"Ư", "U"}, {"Ứ", "U"}, {"Ừ", "U"}, {"Ử", "U"}, {"Ữ", "U"}, {"Ự", "U"},
    {"Ý", "Y"}, {"Ỳ", "Y"}, {"Ỷ", "Y"}, {"Ỹ", "Y"}, {"Ỵ", "Y"},
    {"Đ", "D"}
};

#define REPLACEMENT_COUNT (sizeof(accent_replacements) / sizeof(accent_replacements[0]))

// ============================================
//  HÀM CHUYỂN ĐỔI DẤU
// ============================================

void remove_accents(const char *src, char *dst) {
    if (!src || !dst) return;
    
    strcpy(dst, src);
    
    // Thay thế từng ký tự có dấu
    for (int i = 0; i < REPLACEMENT_COUNT; i++) {
        char temp[512];
        temp[0] = '\0';
        
        const char *from = accent_replacements[i][0];
        const char *to = accent_replacements[i][1];
        
        // Thay thế tất cả occurrences
        char *pos = strstr(dst, from);
        while (pos != NULL) {
            // Copy phần trước
            *pos = '\0';
            strcat(temp, dst);
            strcat(temp, to);
            strcat(temp, pos + strlen(from));
            strcpy(dst, temp);
            pos = strstr(dst, from);
        }
    }
}

void normalize_vietnamese(char *str) {
    if (!str) return;
    
    // Chuyển sang chữ thường
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
    
    // Xóa dấu
    char no_accents[512];
    remove_accents(str, no_accents);
    strcpy(str, no_accents);
    
    // Xóa khoảng trắng thừa
    char temp[512];
    int j = 0;
    int in_space = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == ' ') {
            if (!in_space) {
                temp[j++] = ' ';
                in_space = 1;
            }
        } else {
            temp[j++] = str[i];
            in_space = 0;
        }
    }
    if (j > 0 && temp[j-1] == ' ') j--;
    temp[j] = '\0';
    strcpy(str, temp);
}

// ============================================
//  HÀM PHÂN TÍCH CÂU
// ============================================

VietnameseSentence* parse_vietnamese(const char *input) {
    if (!input || strlen(input) == 0) return NULL;
    
    VietnameseSentence *vs = (VietnameseSentence*)malloc(sizeof(VietnameseSentence));
    if (!vs) return NULL;
    
    memset(vs, 0, sizeof(VietnameseSentence));
    strcpy(vs->original, input);
    
    // Lấy dấu câu cuối
    int len = strlen(input);
    if (len > 0) {
        char last = input[len - 1];
        if (last == '.' || last == '?' || last == '!') {
            vs->punctuation[0] = last;
            vs->punctuation[1] = '\0';
        } else {
            strcpy(vs->punctuation, ".");
        }
    } else {
        strcpy(vs->punctuation, ".");
    }
    
    // Tách từ
    char temp[512];
    strcpy(temp, input);
    
    // Xóa dấu câu cuối
    for (int i = 0; temp[i]; i++) {
        if (temp[i] == '.' || temp[i] == '?' || temp[i] == '!') {
            temp[i] = '\0';
            break;
        }
    }
    
    char *token = strtok(temp, " .,!?;:\n\t\"'()[]{}");
    vs->word_count = 0;
    while (token != NULL && vs->word_count < 50) {
        strcpy(vs->words[vs->word_count], token);
        vs->word_count++;
        token = strtok(NULL, " .,!?;:\n\t\"'()[]{}");
    }
    
    // Chuẩn hóa
    char normalized[512];
    strcpy(normalized, input);
    normalize_vietnamese(normalized);
    strcpy(vs->normalized, normalized);
    
    return vs;
}

void free_vietnamese(VietnameseSentence *vs) {
    if (vs) free(vs);
}

// ============================================
//  HÀM ĐẶT CÂU TIẾNG VIỆT (đơn giản hóa)
// ============================================

char* capitalize_vietnamese(const char *text) {
    static char result[512];
    result[0] = '\0';
    
    if (!text || strlen(text) == 0) {
        strcpy(result, "");
        return result;
    }
    
    // Tìm chữ cái đầu
    int i = 0;
    while (text[i] && !isalpha(text[i]) && !isdigit(text[i])) i++;
    
    if (text[i]) {
        // Đơn giản: viết hoa chữ cái đầu (chỉ cho ASCII)
        strcpy(result, text);
        if (result[i] >= 'a' && result[i] <= 'z') {
            result[i] = result[i] - 32;
        }
    } else {
        strcpy(result, text);
    }
    
    return result;
}

char* format_vietnamese_sentence(const char *input) {
    static char result[512];
    result[0] = '\0';
    
    if (!input || strlen(input) == 0) {
        return "Xin loi, ban chua nhap gi!";
    }
    
    // Phân tích câu
    VietnameseSentence *vs = parse_vietnamese(input);
    if (!vs) return "Loi phan tich cau!";
    
    // Chuẩn hóa
    char temp[512];
    strcpy(temp, input);
    
    // Xóa dấu câu cuối
    int len = strlen(temp);
    if (len > 0 && (temp[len-1] == '.' || temp[len-1] == '?' || temp[len-1] == '!')) {
        temp[len-1] = '\0';
    }
    
    // Viết hoa chữ cái đầu
    char *capitalized = capitalize_vietnamese(temp);
    strcpy(result, capitalized);
    
    // Thêm dấu câu phù hợp
    if (strstr(input, "?") != NULL) {
        strcat(result, "?");
    } else if (strstr(input, "!") != NULL) {
        strcat(result, "!");
    } else {
        strcat(result, ".");
    }
    
    free_vietnamese(vs);
    return result;
}

// ============================================
//  HÀM KIỂM TRA
// ============================================

int is_vietnamese_text(const char *text) {
    if (!text) return 0;
    
    // Kiểm tra có dấu tiếng Việt - dùng chuỗi UTF-8
    const char *vietnamese_chars[] = {
        "á", "à", "ả", "ã", "ạ", "ă", "ắ", "ằ", "ẳ", "ẵ", "ặ",
        "â", "ấ", "ầ", "ẩ", "ẫ", "ậ", "é", "è", "ẻ", "ẽ", "ẹ",
        "ê", "ế", "ề", "ể", "ễ", "ệ", "í", "ì", "ỉ", "ĩ", "ị",
        "ó", "ò", "ỏ", "õ", "ọ", "ô", "ố", "ồ", "ổ", "ỗ", "ộ",
        "ơ", "ớ", "ờ", "ở", "ỡ", "ợ", "ú", "ù", "ủ", "ũ", "ụ",
        "ư", "ứ", "ừ", "ử", "ữ", "ự", "ý", "ỳ", "ỷ", "ỹ", "ỵ",
        "đ", "Á", "À", "Ả", "Ã", "Ạ", "Ă", "Ắ", "Ằ", "Ẳ", "Ẵ", "Ặ",
        "Â", "Ấ", "Ầ", "Ẩ", "Ẫ", "Ậ", "É", "È", "Ẻ", "Ẽ", "Ẹ",
        "Ê", "Ế", "Ề", "Ể", "Ễ", "Ệ", "Í", "Ì", "Ỉ", "Ĩ", "Ị",
        "Ó", "Ò", "Ỏ", "Õ", "Ọ", "Ô", "Ố", "Ồ", "Ổ", "Ỗ", "Ộ",
        "Ơ", "Ớ", "Ờ", "Ở", "Ỡ", "Ợ", "Ú", "Ù", "Ủ", "Ũ", "Ụ",
        "Ư", "Ứ", "Ừ", "Ử", "Ữ", "Ự", "Ý", "Ỳ", "Ỷ", "Ỹ", "Ỵ",
        "Đ"
    };
    
    int char_count = sizeof(vietnamese_chars) / sizeof(vietnamese_chars[0]);
    
    for (int i = 0; i < char_count; i++) {
        if (strstr(text, vietnamese_chars[i]) != NULL) {
            return 1;
        }
    }
    return 0;
}

int count_vietnamese_words(const char *text) {
    if (!text) return 0;
    
    VietnameseSentence *vs = parse_vietnamese(text);
    if (!vs) return 0;
    
    int count = vs->word_count;
    free_vietnamese(vs);
    return count;
}
