#include "language_play.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_VOCAB 500

static Vocabulary vocab[MAX_VOCAB];
static int vocab_count = 0;

// ============================================
//  KHO TỪ VỰNG TIẾNG VIỆT
// ============================================

void add_word(const char *word, const char *type, const char *meaning) {
    if (vocab_count >= MAX_VOCAB) return;
    strcpy(vocab[vocab_count].word, word);
    strcpy(vocab[vocab_count].type, type);
    strcpy(vocab[vocab_count].meaning, meaning);
    vocab_count++;
}

static void init_vocabulary() {
    if (vocab_count > 0) return;
    
    // ========================================
    //  TỪ GHÉP
    // ========================================
    add_word("học sinh", "ghep", "người đang học");
    add_word("sinh viên", "ghep", "người học đại học");
    add_word("nhà trường", "ghep", "cơ sở giáo dục");
    add_word("giáo viên", "ghep", "người dạy học");
    add_word("học tập", "ghep", "quá trình học");
    add_word("giảng dạy", "ghep", "quá trình dạy");
    add_word("vui vẻ", "ghep", "trạng thái thoải mái");
    add_word("hạnh phúc", "ghep", "cảm giác tốt đẹp");
    add_word("thành công", "ghep", "đạt được mục tiêu");
    add_word("thất bại", "ghep", "không đạt mục tiêu");
    add_word("tình yêu", "ghep", "cảm xúc yêu thương");
    add_word("gia đình", "ghep", "những người thân");
    add_word("bạn bè", "ghep", "những người bạn");
    add_word("công việc", "ghep", "việc làm");
    add_word("cuộc sống", "ghep", "sự tồn tại");
    add_word("thế giới", "ghep", "toàn bộ không gian");
    add_word("thiên nhiên", "ghep", "tự nhiên");
    add_word("công nghệ", "ghep", "kỹ thuật tiên tiến");
    add_word("thông tin", "ghep", "dữ liệu");
    add_word("tương lai", "ghep", "thời gian sắp tới");
    add_word("điện thoại", "ghep", "thiết bị liên lạc");
    add_word("máy tính", "ghep", "thiết bị xử lý thông tin");
    add_word("xe đạp", "ghep", "phương tiện di chuyển");
    add_word("xe máy", "ghep", "phương tiện di chuyển có động cơ");
    add_word("ô tô", "ghep", "xe hơi");
    add_word("nhà cửa", "ghep", "nơi ở");
    add_word("đất đai", "ghep", "tài nguyên");
    add_word("sông hồ", "ghep", "nguồn nước");
    add_word("núi non", "ghep", "địa hình");
    add_word("cây cối", "ghep", "thực vật");
    add_word("chim chóc", "ghep", "loài chim");
    add_word("cá cược", "ghep", "hành động đánh bạc");
    
    // ========================================
    //  TỪ LÁY
    // ========================================
    add_word("lon ton", "lay", "dáng đi nhỏ nhắn");
    add_word("loáng thoáng", "lay", "thoáng qua, mờ nhạt");
    add_word("lấp lánh", "lay", "sáng lấp ló");
    add_word("long lanh", "lay", "sáng trong");
    add_word("lung linh", "lay", "lấp lánh huyền ảo");
    add_word("lơ lửng", "lay", "lưng chừng không gian");
    add_word("lạnh lùng", "lay", "thờ ơ, vô cảm");
    add_word("nóng nảy", "lay", "dễ cáu giận");
    add_word("vội vã", "lay", "gấp gáp");
    add_word("xinh xinh", "lay", "dễ thương");
    add_word("đỏ đỏ", "lay", "màu đỏ nhạt");
    add_word("trắng trắng", "lay", "màu trắng nhạt");
    add_word("đen đen", "lay", "màu đen nhạt");
    add_word("vàng vàng", "lay", "màu vàng nhạt");
    add_word("mềm mại", "lay", "mềm, dễ chịu");
    add_word("cứng cáp", "lay", "chắc chắn, khỏe mạnh");
    add_word("êm ái", "lay", "dịu dàng, êm đềm");
    add_word("thầm thì", "lay", "nói nhỏ, kín đáo");
    add_word("rì rào", "lay", "tiếng gió, tiếng suối");
    add_word("ì ầm", "lay", "tiếng ồn, hỗn loạn");
    add_word("rực rỡ", "lay", "chói lọi, tươi sáng");
    add_word("lộng lẫy", "lay", "đẹp, sang trọng");
    add_word("ngập ngừng", "lay", "do dự, lưỡng lự");
    add_word("lững thững", "lay", "đi chậm, thong thả");
    add_word("hớt hơ hớt hải", "lay", "vội vã, cuống quít");
}

void load_vocabulary() {
    init_vocabulary();
    srand(time(NULL));
}

// ============================================
//  HÀM EXTRACT KEYWORD ĐÚNG
// ============================================

static void extract_keyword(const char *input, char *keyword) {
    keyword[0] = '\0';
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    // Tìm từ trong dấu nháy đơn hoặc nháy kép
    char *single_quote = strchr(lower, '\'');
    if (single_quote) {
        single_quote++;
        int j = 0;
        while (single_quote[j] != '\'' && single_quote[j] != '\0' && j < 63) {
            keyword[j] = single_quote[j];
            j++;
        }
        keyword[j] = '\0';
        if (strlen(keyword) > 0) return;
    }
    
    char *double_quote = strchr(lower, '"');
    if (double_quote) {
        double_quote++;
        int j = 0;
        while (double_quote[j] != '"' && double_quote[j] != '\0' && j < 63) {
            keyword[j] = double_quote[j];
            j++;
        }
        keyword[j] = '\0';
        if (strlen(keyword) > 0) return;
    }
    
    // Tìm từ sau "với", "của", "từ"
    const char *patterns[] = {"với", "của", "từ"};
    for (int i = 0; i < 3; i++) {
        char *pos = strstr(lower, patterns[i]);
        if (pos) {
            pos += strlen(patterns[i]);
            while (*pos == ' ') pos++;
            int j = 0;
            while (pos[j] != ' ' && pos[j] != '\0' && pos[j] != '.' && pos[j] != '?' && pos[j] != ',' && j < 63) {
                keyword[j] = pos[j];
                j++;
            }
            keyword[j] = '\0';
            if (strlen(keyword) > 0 && strcmp(keyword, "từ") != 0 && strcmp(keyword, "ghép") != 0 && strcmp(keyword, "láy") != 0) {
                return;
            }
        }
    }
    
    // Tìm từ cuối cùng (lấy từ cuối, bỏ qua "từ ghép", "từ láy")
    char *last = strrchr(lower, ' ');
    if (last) {
        last++;
        strcpy(keyword, last);
        // Loại bỏ dấu câu
        int len = strlen(keyword);
        if (len > 0 && (keyword[len-1] == '.' || keyword[len-1] == '?' || keyword[len-1] == '!' || keyword[len-1] == ',')) {
            keyword[len-1] = '\0';
        }
        // Nếu là "ghép" hoặc "láy" thì thử lấy từ trước đó
        if (strcmp(keyword, "ghép") == 0 || strcmp(keyword, "láy") == 0) {
            char temp[512];
            strcpy(temp, lower);
            char *tok = strtok(temp, " ");
            char prev_word[64] = "";
            while (tok != NULL) {
                if (strcmp(tok, "ghép") == 0 || strcmp(tok, "láy") == 0) {
                    if (strlen(prev_word) > 0) {
                        strcpy(keyword, prev_word);
                        return;
                    }
                }
                strcpy(prev_word, tok);
                tok = strtok(NULL, " ");
            }
        }
        if (strlen(keyword) > 0 && strcmp(keyword, "từ") != 0 && strcmp(keyword, "ghép") != 0 && strcmp(keyword, "láy") != 0) {
            return;
        }
    }
    
    // Mặc định
    strcpy(keyword, "học");
}

// ============================================
//  HÀM TẠO TỪ GHÉP
// ============================================

char* generate_compound_word(const char *base) {
    static char result[512];
    result[0] = '\0';
    
    if (!base || strlen(base) == 0) {
        strcpy(result, "❌ Chưa có từ cần ghép!");
        return result;
    }
    
    char found_words[20][64];
    int found_count = 0;
    
    for (int i = 0; i < vocab_count; i++) {
        if (strcmp(vocab[i].type, "ghep") != 0) continue;
        
        char word_copy[128];
        strcpy(word_copy, vocab[i].word);
        char *parts[10];
        int part_count = 0;
        char *token = strtok(word_copy, " ");
        while (token != NULL && part_count < 10) {
            parts[part_count] = token;
            part_count++;
            token = strtok(NULL, " ");
        }
        
        for (int j = 0; j < part_count; j++) {
            if (strcmp(parts[j], base) == 0) {
                if (found_count < 20) {
                    strcpy(found_words[found_count], vocab[i].word);
                    found_count++;
                    break;
                }
            }
        }
    }
    
    if (found_count > 0) {
        int idx = rand() % found_count;
        for (int i = 0; i < vocab_count; i++) {
            if (strcmp(vocab[i].word, found_words[idx]) == 0) {
                snprintf(result, sizeof(result), 
                         "🔗 Từ ghép với '%s':\n   📝 %s\n   📖 Nghĩa: %s", 
                         base, found_words[idx], vocab[i].meaning);
                return result;
            }
        }
        snprintf(result, sizeof(result), 
                 "🔗 Từ ghép với '%s':\n   📝 %s", 
                 base, found_words[idx]);
    } else {
        snprintf(result, sizeof(result), 
                 "🔍 Không tìm thấy từ ghép với '%s'!\n"
                 "💡 Từ ghép có sẵn: học sinh, sinh viên, nhà trường, giáo viên, hạnh phúc...", 
                 base);
    }
    
    return result;
}

// ============================================
//  HÀM TẠO TỪ LÁY
// ============================================

char* generate_reduplicative_word(const char *base) {
    static char result[512];
    result[0] = '\0';
    
    if (!base || strlen(base) == 0) {
        strcpy(result, "❌ Chưa có từ cần láy!");
        return result;
    }
    
    char found_words[20][64];
    int found_count = 0;
    
    for (int i = 0; i < vocab_count; i++) {
        if (strcmp(vocab[i].type, "lay") != 0) continue;
        
        char word_copy[128];
        strcpy(word_copy, vocab[i].word);
        char *parts[10];
        int part_count = 0;
        char *token = strtok(word_copy, " ");
        while (token != NULL && part_count < 10) {
            parts[part_count] = token;
            part_count++;
            token = strtok(NULL, " ");
        }
        
        for (int j = 0; j < part_count; j++) {
            if (strcmp(parts[j], base) == 0) {
                if (found_count < 20) {
                    strcpy(found_words[found_count], vocab[i].word);
                    found_count++;
                    break;
                }
            }
        }
    }
    
    if (found_count > 0) {
        int idx = rand() % found_count;
        for (int i = 0; i < vocab_count; i++) {
            if (strcmp(vocab[i].word, found_words[idx]) == 0) {
                snprintf(result, sizeof(result), 
                         "🔁 Từ láy với '%s':\n   📝 %s\n   📖 Nghĩa: %s", 
                         base, found_words[idx], vocab[i].meaning);
                return result;
            }
        }
        snprintf(result, sizeof(result), 
                 "🔁 Từ láy với '%s':\n   📝 %s", 
                 base, found_words[idx]);
    } else {
        snprintf(result, sizeof(result), 
                 "🔍 Không tìm thấy từ láy với '%s'!\n"
                 "💡 Từ láy có sẵn: lấp lánh, long lanh, lung linh, xinh xinh, đỏ đỏ...", 
                 base);
    }
    
    return result;
}

// ============================================
//  HÀM ĐẶT CÂU
// ============================================

char* generate_sentence_with_word(const char *word, const char *topic) {
    static char result[512];
    result[0] = '\0';
    
    if (!word || strlen(word) == 0) {
        strcpy(result, "❌ Chưa có từ để đặt câu!");
        return result;
    }
    
    const char *sentence_templates[][3] = {
        {"học sinh", "Học sinh là tương lai của đất nước.", "Học sinh cần chăm chỉ học tập."},
        {"sinh viên", "Sinh viên đang ngồi trong thư viện.", "Sinh viên là lực lượng tri thức trẻ."},
        {"nhà trường", "Nhà trường là nơi ươm mầm tri thức.", "Nhà trường luôn tạo điều kiện tốt nhất."},
        {"giáo viên", "Giáo viên là người truyền đạt kiến thức.", "Cô giáo đang giảng bài."},
        {"học tập", "Học tập là con đường thành công.", "Học tập không bao giờ là đủ."},
        {"vui vẻ", "Cô ấy luôn vui vẻ mỗi ngày.", "Vui vẻ là liều thuốc bổ."},
        {"hạnh phúc", "Hạnh phúc đến từ những điều nhỏ nhặt.", "Gia đình hạnh phúc là điều tuyệt vời."},
        {"thành công", "Thành công không phải là đích đến.", "Thành công là hành trình."},
        {"tình yêu", "Tình yêu là điều kỳ diệu nhất.", "Tình yêu đích thực không cần nói nhiều."},
        {"gia đình", "Gia đình là chỗ dựa vững chắc nhất.", "Gia đình là tất cả."},
        {"bạn bè", "Bạn bè là người đi cùng ta.", "Tình bạn bè thật quý giá."},
        {"công việc", "Công việc mang lại niềm vui.", "Công việc giúp ta sống có ý nghĩa."},
        {"cuộc sống", "Cuộc sống có nhiều điều thú vị.", "Cuộc sống là một hành trình."},
        {"thế giới", "Thế giới rộng lớn và đa dạng.", "Thế giới đang thay đổi từng ngày."},
        {"công nghệ", "Công nghệ thay đổi cuộc sống.", "Công nghệ phát triển rất nhanh."},
        {"tương lai", "Tương lai là của những người trẻ.", "Tương lai đang chờ đợi ta."}
    };
    
    int num_templates = sizeof(sentence_templates) / sizeof(sentence_templates[0]);
    int found = 0;
    
    for (int i = 0; i < num_templates; i++) {
        if (strstr(sentence_templates[i][0], word) != NULL || 
            strstr(word, sentence_templates[i][0]) != NULL) {
            int idx = rand() % 2 + 1;
            snprintf(result, sizeof(result), 
                     "📝 Đặt câu với từ '%s':\n   %s\n\n💡 Gợi ý thêm:\n   %s", 
                     word, sentence_templates[i][idx], sentence_templates[i][idx == 1 ? 2 : 1]);
            found = 1;
            break;
        }
    }
    
    if (!found) {
        char topic_part[128] = "";
        if (topic && strlen(topic) > 0) {
            snprintf(topic_part, sizeof(topic_part), " về %s", topic);
        }
        snprintf(result, sizeof(result), 
                 "📝 Đặt câu với từ '%s':\n   %s là một từ rất hay trong tiếng Việt.%s\n\n💡 Bạn có thể đặt câu khác theo ý mình!", 
                 word, word, topic_part);
    }
    
    return result;
}

// ============================================
//  PHÁT HIỆN YÊU CẦU
// ============================================

int is_language_request(const char *input) {
    if (!input) return 0;
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    const char *keywords[] = {
        "từ ghép", "từ láy", "đặt câu", "tạo từ", 
        "ghép từ", "láy", "câu với từ"
    };
    
    for (int i = 0; i < 7; i++) {
        if (strstr(lower, keywords[i]) != NULL) {
            return 1;
        }
    }
    return 0;
}

// ============================================
//  XỬ LÝ YÊU CẦU
// ============================================

char* answer_language_request(const char *input) {
    static char response[1024];
    response[0] = '\0';
    
    if (!is_language_request(input)) {
        return NULL;
    }
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    // ============================================
    //  EXTRACT KEYWORD ĐÚNG
    // ============================================
    char keyword[64] = "";
    char topic[64] = "";
    
    // Tìm từ trong dấu nháy
    char *single_quote = strchr(input, '\'');
    if (single_quote) {
        single_quote++;
        int j = 0;
        while (single_quote[j] != '\'' && single_quote[j] != '\0' && j < 63) {
            keyword[j] = single_quote[j];
            j++;
        }
        keyword[j] = '\0';
    } else {
        char *double_quote = strchr(input, '"');
        if (double_quote) {
            double_quote++;
            int j = 0;
            while (double_quote[j] != '"' && double_quote[j] != '\0' && j < 63) {
                keyword[j] = double_quote[j];
                j++;
            }
            keyword[j] = '\0';
        }
    }
    
    // Nếu không có dấu nháy, extract theo cách khác
    if (strlen(keyword) == 0) {
        char temp[512];
        strcpy(temp, lower);
        
        // Tìm sau "với", "của", "từ"
        const char *patterns[] = {"với", "của", "từ"};
        for (int i = 0; i < 3; i++) {
            char *pos = strstr(temp, patterns[i]);
            if (pos) {
                pos += strlen(patterns[i]);
                while (*pos == ' ') pos++;
                int j = 0;
                while (pos[j] != ' ' && pos[j] != '\0' && pos[j] != '.' && pos[j] != '?' && pos[j] != ',' && j < 63) {
                    keyword[j] = pos[j];
                    j++;
                }
                keyword[j] = '\0';
                if (strlen(keyword) > 0 && strcmp(keyword, "từ") != 0 && strcmp(keyword, "ghép") != 0 && strcmp(keyword, "láy") != 0) {
                    break;
                }
            }
        }
    }
    
    // Nếu vẫn chưa có keyword, lấy từ cuối
    if (strlen(keyword) == 0) {
        char *last = strrchr(lower, ' ');
        if (last) {
            last++;
            strcpy(keyword, last);
            // Loại bỏ dấu câu
            int len = strlen(keyword);
            if (len > 0 && (keyword[len-1] == '.' || keyword[len-1] == '?' || keyword[len-1] == '!' || keyword[len-1] == ',')) {
                keyword[len-1] = '\0';
            }
        } else {
            strcpy(keyword, lower);
        }
    }
    
    // Tìm chủ đề (sau "về")
    char *ve = strstr(lower, "về");
    if (ve) {
        ve += 2;
        while (*ve == ' ') ve++;
        int j = 0;
        while (ve[j] != ' ' && ve[j] != '\0' && ve[j] != '.' && ve[j] != '?' && j < 63) {
            topic[j] = ve[j];
            j++;
        }
        topic[j] = '\0';
    }
    
    // Xác định loại yêu cầu
    if (strstr(lower, "từ ghép") != NULL || strstr(lower, "ghép từ") != NULL) {
        char *result = generate_compound_word(keyword);
        strcpy(response, result);
    }
    else if (strstr(lower, "từ láy") != NULL || strstr(lower, "láy") != NULL) {
        char *result = generate_reduplicative_word(keyword);
        strcpy(response, result);
    }
    else if (strstr(lower, "đặt câu") != NULL || strstr(lower, "câu với") != NULL) {
        char *result = generate_sentence_with_word(keyword, topic);
        strcpy(response, result);
    }
    else {
        snprintf(response, sizeof(response),
                 "🔍 Yêu cầu không rõ!\n"
                 "💡 Hãy thử:\n"
                 "   - 'từ ghép với [từ]'\n"
                 "   - 'từ láy với [từ]'\n"
                 "   - 'đặt câu với [từ]'");
    }
    
    return response;
}
