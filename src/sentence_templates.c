#include "sentence_templates.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_TEMPLATES 100

static SentenceTemplate templates[MAX_TEMPLATES];
int template_count = 0;

// ============================================
//  KHO MẪU CÂU
// ============================================

void add_template(const char *id, const char *pattern, const char *category, 
                  const char *slots, const char *example) {
    if (template_count >= MAX_TEMPLATES) return;
    
    strcpy(templates[template_count].id, id);
    strcpy(templates[template_count].pattern, pattern);
    strcpy(templates[template_count].category, category);
    strcpy(templates[template_count].example, example ? example : "");
    
    // Tách slots
    templates[template_count].slot_count = 0;
    char temp[128];
    strcpy(temp, slots);
    char *token = strtok(temp, ",");
    while (token != NULL && templates[template_count].slot_count < 5) {
        while (*token == ' ') token++;
        strcpy(templates[template_count].slots[templates[template_count].slot_count], token);
        templates[template_count].slot_count++;
        token = strtok(NULL, ",");
    }
    
    template_count++;
}

static void init_templates() {
    if (template_count > 0) return;
    
    // ========================================
    //  CHÀO HỎI (greeting)
    // ========================================
    add_template("greet_1", "Xin chào, tôi là {name}.", "greeting", "name", "Xin chào, tôi là CAI.");
    add_template("greet_2", "Chào {name}, rất vui được gặp bạn!", "greeting", "name", "Chào bạn, rất vui được gặp bạn!");
    add_template("greet_3", "Xin chào {name}! Hôm nay bạn thế nào?", "greeting", "name", "Xin chào bạn! Hôm nay bạn thế nào?");
    add_template("greet_4", "Chào {name}, tôi là {ai_name}.", "greeting", "name,ai_name", "Chào bạn, tôi là CAI.");
    add_template("greet_5", "Chào buổi sáng {name}! Một ngày mới tốt lành!", "greeting", "name", "Chào buổi sáng bạn! Một ngày mới tốt lành!");
    
    // ========================================
    //  HỎI ĐÁP (question)
    // ========================================
    add_template("ask_1", "{subject} là gì?", "question", "subject", "AI là gì?");
    add_template("ask_2", "Làm thế nào để {action}?", "question", "action", "Làm thế nào để học lập trình?");
    add_template("ask_3", "Tại sao {subject} lại {property}?", "question", "subject,property", "Tại sao trời lại mưa?");
    add_template("ask_4", "Bạn có biết về {subject} không?", "question", "subject", "Bạn có biết về AI không?");
    add_template("ask_5", "Có phải {subject} là {description}?", "question", "subject,description", "Có phải AI là trí tuệ nhân tạo?");
    add_template("ask_6", "Bạn nghĩ gì về {subject}?", "question", "subject", "Bạn nghĩ gì về tình yêu?");
    add_template("ask_7", "{subject} có nghĩa là gì?", "question", "subject", "Hạnh phúc có nghĩa là gì?");
    add_template("ask_8", "Ai là {subject}?", "question", "subject", "Ai là nhà phát minh ra điện?");
    add_template("ask_9", "Khi nào thì {event} xảy ra?", "question", "event", "Khi nào thì ngày mai đến?");
    add_template("ask_10", "Ở đâu có thể {action}?", "question", "action", "Ở đâu có thể học lập trình?");
    
    // ========================================
    //  TRẢ LỜI (response)
    // ========================================
    add_template("resp_1", "Theo tôi, {subject} là {definition}.", "response", "subject,definition", "Theo tôi, AI là trí tuệ nhân tạo.");
    add_template("resp_2", "Vâng, {subject} đúng là {description}.", "response", "subject,description", "Vâng, điều đó đúng.");
    add_template("resp_3", "Tôi nghĩ {subject} có nghĩa là {meaning}.", "response", "subject,meaning", "Tôi nghĩ đó có nghĩa là...");
    add_template("resp_4", "Thực ra, {subject} là {explanation}.", "response", "subject,explanation", "Thực ra, điều đó là...");
    add_template("resp_5", "Đúng vậy, {subject} được dùng để {purpose}.", "response", "subject,purpose", "Đúng vậy, nó được dùng để...");
    add_template("resp_6", "Tôi hiểu rằng {subject} là {definition}.", "response", "subject,definition", "Tôi hiểu rằng AI là trí tuệ nhân tạo.");
    add_template("resp_7", "Chắc chắn rồi, {subject} là {description}.", "response", "subject,description", "Chắc chắn rồi, đó là sự thật.");
    add_template("resp_8", "Tôi đồng ý, {subject} thật {adjective}.", "response", "subject,adjective", "Tôi đồng ý, nó thật tuyệt vời.");
    
    // ========================================
    //  MIÊU TẢ (description)
    // ========================================
    add_template("desc_1", "{entity} có màu {color}, hình dáng {shape}, và nặng {weight}.", "description", "entity,color,shape,weight", "Quả táo có màu đỏ, hình tròn, nặng 200g.");
    add_template("desc_2", "{entity} là một {type} với đặc điểm {feature}.", "description", "entity,type,feature", "Xe là phương tiện với động cơ.");
    add_template("desc_3", "{entity} được biết đến với {famous}.", "description", "entity,famous", "Việt Nam được biết đến với phở.");
    add_template("desc_4", "Trong {context}, {entity} thường được {action}.", "description", "context,entity,action", "Trong học tập, học sinh thường học bài.");
    add_template("desc_5", "{entity} là một {type} dùng để {purpose}.", "description", "entity,type,purpose", "Máy tính là thiết bị dùng để tính toán.");
    add_template("desc_6", "{entity} có {attribute1} và {attribute2}.", "description", "entity,attribute1,attribute2", "Cây có lá và rễ.");
    add_template("desc_7", "{entity} thường sống ở {habitat}.", "description", "entity,habitat", "Cá thường sống ở dưới nước.");
    
    // ========================================
    //  Ý KIẾN (opinion)
    // ========================================
    add_template("opin_1", "Tôi nghĩ {subject} {adjective}.", "opinion", "subject,adjective", "Tôi nghĩ học tập rất quan trọng.");
    add_template("opin_2", "Theo tôi, {subject} thật {feeling}.", "opinion", "subject,feeling", "Theo tôi, cuộc sống thật đẹp.");
    add_template("opin_3", "Cá nhân tôi thấy {subject} là {evaluation}.", "opinion", "subject,evaluation", "Cá nhân tôi thấy AI là công nghệ tuyệt vời.");
    add_template("opin_4", "Tôi tin rằng {subject} là {belief}.", "opinion", "subject,belief", "Tôi tin rằng tương lai là tươi sáng.");
    add_template("opin_5", "Theo quan điểm của tôi, {subject} là {view}.", "opinion", "subject,view", "Theo quan điểm của tôi, học tập là quan trọng.");
    
    // ========================================
    //  GỢI Ý (suggestion)
    // ========================================
    add_template("sug_1", "Bạn nên {action} để {goal}.", "suggestion", "action,goal", "Bạn nên học để thành công.");
    add_template("sug_2", "Tôi khuyên bạn {action} vì {reason}.", "suggestion", "action,reason", "Tôi khuyên bạn đọc sách vì có nhiều kiến thức.");
    add_template("sug_3", "Cách tốt nhất là {action} thay vì {alternative}.", "suggestion", "action,alternative", "Cách tốt nhất là làm việc thay vì lười biếng.");
    add_template("sug_4", "Bạn có thể {action} nếu bạn muốn {goal}.", "suggestion", "action,goal", "Bạn có thể học nếu bạn muốn giỏi.");
    add_template("sug_5", "Hãy {action} để {result}.", "suggestion", "action,result", "Hãy cố gắng để đạt được thành công.");
    
    // ========================================
    //  CẢM ƠN (thanks)
    // ========================================
    add_template("thank_1", "Cảm ơn bạn vì {reason}!", "thanks", "reason", "Cảm ơn bạn vì đã hỏi!");
    add_template("thank_2", "Xin cảm ơn {name}!", "thanks", "name", "Xin cảm ơn bạn!");
    add_template("thank_3", "Cảm ơn bạn đã {action}!", "thanks", "action", "Cảm ơn bạn đã lắng nghe!");
    
    // ========================================
    //  TẠM BIỆT (farewell)
    // ========================================
    add_template("farewell_1", "Tạm biệt {name}! Hẹn gặp lại!", "farewell", "name", "Tạm biệt bạn! Hẹn gặp lại!");
    add_template("farewell_2", "Chúc {name} một ngày tốt lành!", "farewell", "name", "Chúc bạn một ngày tốt lành!");
    add_template("farewell_3", "Hẹn gặp lại {name}!", "farewell", "name", "Hẹn gặp lại bạn!");
}

void load_sentence_templates() {
    init_templates();
    srand(time(NULL));
}

// ============================================
//  HÀM TÌM KIẾM
// ============================================

SentenceTemplate* find_template(const char *id) {
    for (int i = 0; i < template_count; i++) {
        if (strcmp(templates[i].id, id) == 0) {
            return &templates[i];
        }
    }
    return NULL;
}

SentenceTemplate* find_template_by_pattern(const char *pattern) {
    for (int i = 0; i < template_count; i++) {
        if (strstr(templates[i].pattern, pattern) != NULL) {
            return &templates[i];
        }
    }
    return NULL;
}

// ============================================
//  HÀM ĐIỀN SLOT
// ============================================

FilledSentence* fill_template(const char *template_id, char *slots[], int slot_count) {
    SentenceTemplate *tmpl = find_template(template_id);
    if (!tmpl) return NULL;
    
    FilledSentence *fs = (FilledSentence*)malloc(sizeof(FilledSentence));
    if (!fs) return NULL;
    
    memset(fs, 0, sizeof(FilledSentence));
    strcpy(fs->template_id, template_id);
    
    fs->filled_count = 0;
    for (int i = 0; i < slot_count && i < 5; i++) {
        if (slots[i] && strlen(slots[i]) > 0) {
            strcpy(fs->filled_slots[fs->filled_count], slots[i]);
            fs->filled_count++;
        }
    }
    
    char result[512];
    strcpy(result, tmpl->pattern);
    
    for (int i = 0; i < fs->filled_count; i++) {
        char slot_name[64];
        snprintf(slot_name, sizeof(slot_name), "{%s}", tmpl->slots[i]);
        
        char *pos = strstr(result, slot_name);
        if (pos) {
            char before[256], after[256];
            strcpy(before, result);
            before[pos - result] = '\0';
            strcpy(after, pos + strlen(slot_name));
            snprintf(result, sizeof(result), "%s%s%s", before, fs->filled_slots[i], after);
        }
    }
    
    strcpy(fs->filled_sentence, result);
    
    return fs;
}

char* fill_template_with_string(const char *template_id, const char *slot_values) {
    static char result[512];
    result[0] = '\0';
    
    SentenceTemplate *tmpl = find_template(template_id);
    if (!tmpl) return NULL;
    
    char slots[5][64];
    int slot_count = 0;
    char temp[256];
    strcpy(temp, slot_values);
    char *token = strtok(temp, ",");
    while (token != NULL && slot_count < 5) {
        while (*token == ' ') token++;
        strcpy(slots[slot_count], token);
        slot_count++;
        token = strtok(NULL, ",");
    }
    
    strcpy(result, tmpl->pattern);
    for (int i = 0; i < slot_count && i < tmpl->slot_count; i++) {
        char slot_name[64];
        snprintf(slot_name, sizeof(slot_name), "{%s}", tmpl->slots[i]);
        
        char *pos = strstr(result, slot_name);
        if (pos) {
            char before[256], after[256];
            strcpy(before, result);
            before[pos - result] = '\0';
            strcpy(after, pos + strlen(slot_name));
            snprintf(result, sizeof(result), "%s%s%s", before, slots[i], after);
        }
    }
    
    return result;
}

void free_filled_sentence(FilledSentence *fs) {
    if (fs) free(fs);
}

// ============================================
//  HÀM TẠO CÂU NGẪU NHIÊN
// ============================================

char* generate_random_sentence(const char *category) {
    static char result[512];
    result[0] = '\0';
    
    int indices[100];
    int count = 0;
    
    for (int i = 0; i < template_count; i++) {
        if (strcmp(templates[i].category, category) == 0 || strcmp(category, "all") == 0) {
            indices[count++] = i;
        }
    }
    
    if (count == 0) {
        strcpy(result, "Không có mẫu câu nào!");
        return result;
    }
    
    int idx = indices[rand() % count];
    SentenceTemplate *tmpl = &templates[idx];
    
    if (tmpl->slot_count == 0) {
        strcpy(result, tmpl->pattern);
        return result;
    }
    
    char default_slots[5][32] = {"cái này", "điều đó", "một thứ", "ai đó", "một nơi"};
    strcpy(result, tmpl->pattern);
    
    for (int i = 0; i < tmpl->slot_count; i++) {
        char slot_name[64];
        snprintf(slot_name, sizeof(slot_name), "{%s}", tmpl->slots[i]);
        
        char *pos = strstr(result, slot_name);
        if (pos) {
            char before[256], after[256];
            strcpy(before, result);
            before[pos - result] = '\0';
            strcpy(after, pos + strlen(slot_name));
            snprintf(result, sizeof(result), "%s%s%s", before, default_slots[i], after);
        }
    }
    
    return result;
}

char* generate_response_with_template(const char *input) {
    static char response[512];
    response[0] = '\0';
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) lower[i] = tolower(lower[i]);
    
    char category[32] = "response";
    
    if (strstr(lower, "chào") != NULL || strstr(lower, "hello") != NULL) {
        strcpy(category, "greeting");
    } else if (strstr(lower, "?") != NULL || strstr(lower, "hỏi") != NULL) {
        strcpy(category, "question");
    } else if (strstr(lower, "mô tả") != NULL || strstr(lower, "giới thiệu") != NULL) {
        strcpy(category, "description");
    } else if (strstr(lower, "nghĩ") != NULL || strstr(lower, "cảm thấy") != NULL) {
        strcpy(category, "opinion");
    } else if (strstr(lower, "nên") != NULL || strstr(lower, "khuyên") != NULL) {
        strcpy(category, "suggestion");
    } else if (strstr(lower, "cảm ơn") != NULL) {
        strcpy(category, "thanks");
    } else if (strstr(lower, "tạm biệt") != NULL || strstr(lower, "bye") != NULL) {
        strcpy(category, "farewell");
    }
    
    return generate_random_sentence(category);
}

// ============================================
//  PHÁT HIỆN YÊU CẦU
// ============================================

int is_template_request(const char *input) {
    if (!input) return 0;
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    const char *keywords[] = {
        "mẫu câu", "câu mẫu", "template", "tạo câu",
        "ví dụ câu", "câu có sẵn", "mẫu câu chào",
        "mẫu câu hỏi", "mẫu câu trả lời"
    };
    
    for (int i = 0; i < 9; i++) {
        if (strstr(lower, keywords[i]) != NULL) {
            return 1;
        }
    }
    return 0;
}

char* answer_template_request(const char *input) {
    static char response[4096];
    response[0] = '\0';
    
    if (!is_template_request(input)) {
        return NULL;
    }
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) lower[i] = tolower(lower[i]);
    
    char category[32] = "all";
    
    if (strstr(lower, "chào") != NULL) {
        strcpy(category, "greeting");
    } else if (strstr(lower, "hỏi") != NULL || strstr(lower, "?") != NULL) {
        strcpy(category, "question");
    } else if (strstr(lower, "mô tả") != NULL || strstr(lower, "giới thiệu") != NULL) {
        strcpy(category, "description");
    } else if (strstr(lower, "nghĩ") != NULL || strstr(lower, "cảm thấy") != NULL) {
        strcpy(category, "opinion");
    } else if (strstr(lower, "gợi ý") != NULL || strstr(lower, "nên") != NULL || strstr(lower, "khuyên") != NULL) {
        strcpy(category, "suggestion");
    } else if (strstr(lower, "cảm ơn") != NULL) {
        strcpy(category, "thanks");
    } else if (strstr(lower, "tạm biệt") != NULL || strstr(lower, "bye") != NULL) {
        strcpy(category, "farewell");
    }
    
    strcat(response, "📝 MẪU CÂU CÓ SẴN - ");
    strcat(response, category);
    strcat(response, ":\n\n");
    
    int found = 0;
    for (int i = 0; i < template_count; i++) {
        if (strcmp(templates[i].category, category) == 0 || strcmp(category, "all") == 0) {
            char line[256];
            snprintf(line, sizeof(line), "🔹 [%s] %s\n", templates[i].id, templates[i].pattern);
            strcat(response, line);
            found = 1;
            
            if (strlen(templates[i].example) > 0) {
                char ex[128];
                snprintf(ex, sizeof(ex), "   💡 Ví dụ: %s\n", templates[i].example);
                strcat(response, ex);
            }
            strcat(response, "\n");
        }
    }
    
    if (!found) {
        strcat(response, "Không có mẫu câu nào trong danh mục này!\n");
    }
    
    strcat(response, "\n📌 TẠO CÂU NGẪU NHIÊN:\n");
    char *random = generate_random_sentence(category);
    strcat(response, "   💬 ");
    strcat(response, random);
    strcat(response, "\n\n");
    
    strcat(response, "💡 Hướng dẫn điền slot:\n");
    strcat(response, "   - Sử dụng cú pháp: fill [template_id] [value1,value2,...]\n");
    strcat(response, "   - Ví dụ: fill greet_1 CAI\n");
    strcat(response, "   - Ví dụ: fill ask_1 AI\n");
    
    return response;
}
