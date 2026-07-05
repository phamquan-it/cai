#include "knowledge_learner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// ============================================
//  KHỞI TẠO
// ============================================

KnowledgeBase* init_knowledge_base() {
    KnowledgeBase *kb = (KnowledgeBase*)malloc(sizeof(KnowledgeBase));
    if (!kb) return NULL;
    memset(kb, 0, sizeof(KnowledgeBase));
    return kb;
}

void free_knowledge_base(KnowledgeBase *kb) {
    if (kb) free(kb);
}

// ============================================
//  TRÍCH XUẤT TỪ KHÓA
// ============================================

void extract_keywords(const char *sentence, char keywords[][64], int *count) {
    *count = 0;
    if (!sentence) return;
    
    char temp[512];
    strcpy(temp, sentence);
    
    const char *stopwords[] = {"và", "của", "là", "trong", "với", "cho", "khi", "nếu",
                               "để", "mà", "này", "đó", "ở", "từ", "còn", "thì", "sẽ",
                               "đã", "đang", "được", "có", "không", "cũng", "nên", "vì"};
    
    char *token = strtok(temp, " .,!?;:\n\t\"'()[]{}");
    while (token != NULL && *count < 10) {
        char lower[64];
        strcpy(lower, token);
        for (int i = 0; lower[i]; i++) lower[i] = tolower(lower[i]);
        
        int is_stop = 0;
        for (int i = 0; i < 26; i++) {
            if (strcmp(lower, stopwords[i]) == 0) {
                is_stop = 1;
                break;
            }
        }
        
        if (!is_stop && strlen(lower) > 2) {
            strcpy(keywords[*count], lower);
            (*count)++;
        }
        token = strtok(NULL, " .,!?;:\n\t\"'()[]{}");
    }
}

void extract_context(const char *sentence, char *context) {
    context[0] = '\0';
    if (!sentence) return;
    
    char lower[512];
    strcpy(lower, sentence);
    for (int i = 0; lower[i]; i++) lower[i] = tolower(lower[i]);
    
    if (strstr(lower, "học") != NULL || strstr(lower, "trường") != NULL) {
        strcpy(context, "education");
    } else if (strstr(lower, "công thức") != NULL || strstr(lower, "toán") != NULL) {
        strcpy(context, "math");
    } else if (strstr(lower, "lực") != NULL || strstr(lower, "vận tốc") != NULL) {
        strcpy(context, "physics");
    } else if (strstr(lower, "nguyên tố") != NULL || strstr(lower, "hóa") != NULL) {
        strcpy(context, "chemistry");
    } else if (strstr(lower, "lập trình") != NULL || strstr(lower, "code") != NULL) {
        strcpy(context, "coding");
    } else if (strstr(lower, "yêu") != NULL || strstr(lower, "tình cảm") != NULL) {
        strcpy(context, "emotion");
    } else if (strstr(lower, "con") != NULL || strstr(lower, "động vật") != NULL) {
        strcpy(context, "animal");
    } else {
        strcpy(context, "general");
    }
}

// ============================================
//  HỌC TỪ TEXT - CÓ KIỂM TRA NULL
// ============================================

int learn_from_text(KnowledgeBase *kb, const char *text, const char *source, Dictionary *dict) {
    if (!kb || !text) return 0;
    
    char temp[4096];
    strcpy(temp, text);
    
    char *sentences[100];
    int sent_count = 0;
    char *start = temp;
    char *end = temp;
    
    while (*end && sent_count < 100) {
        while (*end && *end != '.' && *end != '?' && *end != '!') {
            end++;
        }
        if (*end) {
            int len = end - start + 1;
            sentences[sent_count] = (char*)malloc(len + 1);
            if (!sentences[sent_count]) break;
            strncpy(sentences[sent_count], start, len);
            sentences[sent_count][len] = '\0';
            sent_count++;
            end++;
            start = end;
        }
    }
    if (*start && sent_count < 100) {
        sentences[sent_count] = strdup(start);
        if (sentences[sent_count]) sent_count++;
    }
    
    int learned = 0;
    for (int i = 0; i < sent_count && kb->count < 500; i++) {
        char *s = sentences[i];
        if (!s) continue;
        while (*s == ' ') s++;
        if (strlen(s) < 5) {
            free(sentences[i]);
            continue;
        }
        
        strcpy(kb->items[kb->count].sentence, s);
        extract_keywords(s, kb->items[kb->count].keywords, &kb->items[kb->count].keyword_count);
        extract_context(s, kb->items[kb->count].context);
        kb->items[kb->count].importance = 5 + rand() % 5;
        strcpy(kb->items[kb->count].source, source ? source : "unknown");
        
        if (dict) {
            markov_learn(dict, s);
        }
        
        kb->count++;
        learned++;
        free(sentences[i]);
    }
    
    if (source && kb->file_count < 10) {
        strcpy(kb->learned_from[kb->file_count], source);
        kb->file_count++;
    }
    
    return learned;
}

// ============================================
//  HỌC TỪ FILE - CÓ KIỂM TRA NULL
// ============================================

int learn_from_file(KnowledgeBase *kb, const char *filename, Dictionary *dict) {
    if (!kb || !filename) return 0;
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("⚠️  Không thể mở file: %s\n", filename);
        return 0;
    }
    
    char content[10000];
    content[0] = '\0';
    char line[1000];
    
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;
        strncat(content, line, sizeof(content) - strlen(content) - 1);
    }
    fclose(file);
    
    int learned = learn_from_text(kb, content, filename, dict);
    printf("📚 Đã học %d câu từ %s\n", learned, filename);
    return learned;
}

// ============================================
//  TRUY VẤN - CÓ KIỂM TRA NULL
// ============================================

char* search_knowledge(KnowledgeBase *kb, const char *query) {
    static char result[4096];
    result[0] = '\0';
    
    if (!kb || kb->count == 0) {
        strcpy(result, "📭 Chưa có kiến thức nào được học!");
        return result;
    }
    
    char query_lower[128];
    strcpy(query_lower, query);
    for (int i = 0; query_lower[i]; i++) query_lower[i] = tolower(query_lower[i]);
    
    char query_keywords[10][64];
    int qk_count = 0;
    char temp[128];
    strcpy(temp, query_lower);
    char *token = strtok(temp, " .,!?;:\n\t");
    while (token != NULL && qk_count < 10) {
        if (strlen(token) > 2) {
            strcpy(query_keywords[qk_count], token);
            qk_count++;
        }
        token = strtok(NULL, " .,!?;:\n\t");
    }
    
    int found = 0;
    strcat(result, "🔍 KẾT QUẢ TÌM KIẾM:\n\n");
    
    for (int i = 0; i < kb->count && i < 20; i++) {
        int match = 0;
        for (int j = 0; j < qk_count; j++) {
            for (int k = 0; k < kb->items[i].keyword_count; k++) {
                if (strstr(kb->items[i].keywords[k], query_keywords[j]) != NULL ||
                    strstr(query_keywords[j], kb->items[i].keywords[k]) != NULL) {
                    match = 1;
                    break;
                }
            }
            if (match) break;
        }
        
        if (!match) {
            char ctx[128];
            extract_context(query, ctx);
            if (strcmp(kb->items[i].context, ctx) == 0) {
                match = 1;
            }
        }
        
        if (match) {
            char line[512];
            snprintf(line, sizeof(line), 
                     "📌 [%s] %s\n   🔑 Từ khóa: ", 
                     kb->items[i].source, kb->items[i].sentence);
            strcat(result, line);
            
            for (int k = 0; k < kb->items[i].keyword_count; k++) {
                strcat(result, kb->items[i].keywords[k]);
                if (k < kb->items[i].keyword_count - 1) strcat(result, ", ");
            }
            strcat(result, "\n   📂 Ngữ cảnh: ");
            strcat(result, kb->items[i].context);
            strcat(result, "\n\n");
            found++;
        }
    }
    
    if (!found) {
        strcat(result, "❌ Không tìm thấy kiến thức phù hợp!\n");
        strcat(result, "💡 Hãy học thêm bằng cách: learn [file.txt] hoặc học từ đoạn văn bản.\n");
    }
    
    return result;
}

char* get_knowledge_summary(KnowledgeBase *kb) {
    static char summary[2048];
    summary[0] = '\0';
    
    if (!kb || kb->count == 0) {
        strcpy(summary, "📭 Chưa có kiến thức nào!");
        return summary;
    }
    
    snprintf(summary, sizeof(summary),
             "📚 TỔNG QUAN KIẾN THỨC:\n"
             "   📊 Số lượng: %d câu\n"
             "   📁 Học từ: ",
             kb->count);
    
    for (int i = 0; i < kb->file_count; i++) {
        strcat(summary, kb->learned_from[i]);
        if (i < kb->file_count - 1) strcat(summary, ", ");
    }
    strcat(summary, "\n\n📌 Một số câu đã học:\n");
    
    for (int i = 0; i < kb->count && i < 10; i++) {
        char line[256];
        snprintf(line, sizeof(line), "   %d. %s\n", i+1, kb->items[i].sentence);
        strcat(summary, line);
    }
    
    if (kb->count > 10) {
        strcat(summary, "   ... và nhiều hơn nữa!\n");
    }
    
    return summary;
}

// ============================================
//  PHÁT HIỆN YÊU CẦU HỌC
// ============================================

int is_learn_request(const char *input) {
    if (!input) return 0;
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) lower[i] = tolower(lower[i]);
    
    const char *keywords[] = {
        "learn", "học", "đọc file", "đọc tài liệu",
        "học từ", "thêm kiến thức", "knowledge"
    };
    
    for (int i = 0; i < 7; i++) {
        if (strstr(lower, keywords[i]) != NULL) {
            return 1;
        }
    }
    return 0;
}

char* answer_learn_request(const char *input, KnowledgeBase *kb, Dictionary *dict) {
    static char response[4096];
    response[0] = '\0';
    
    if (!kb) {
        strcpy(response, "❌ Knowledge Base chưa được khởi tạo!");
        return response;
    }
    
    if (!is_learn_request(input)) {
        return NULL;
    }
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) lower[i] = tolower(lower[i]);
    
    // Học từ file: "learn data.txt" hoặc "học từ data.txt"
    if (strstr(lower, "learn") != NULL || strstr(lower, "học") != NULL) {
        char filename[128];
        filename[0] = '\0';
        
        const char *patterns[] = {"learn", "học từ", "đọc file"};
        for (int i = 0; i < 3; i++) {
            char *pos = strstr(lower, patterns[i]);
            if (pos) {
                pos += strlen(patterns[i]);
                while (*pos == ' ') pos++;
                int j = 0;
                while (pos[j] != ' ' && pos[j] != '\0' && pos[j] != '.' && pos[j] != '?' && j < 127) {
                    filename[j] = pos[j];
                    j++;
                }
                filename[j] = '\0';
                if (strlen(filename) > 0) break;
            }
        }
        
        if (strlen(filename) > 0) {
            int learned = learn_from_file(kb, filename, dict);
            if (learned > 0) {
                snprintf(response, sizeof(response),
                         "✅ Đã học %d câu từ file '%s'!\n"
                         "📚 Tổng số kiến thức hiện tại: %d câu",
                         learned, filename, kb->count);
            } else {
                snprintf(response, sizeof(response),
                         "❌ Không thể học từ file '%s'!\n"
                         "💡 Kiểm tra lại tên file và đường dẫn.",
                         filename);
            }
        } else {
            strcpy(response, "❌ Vui lòng chỉ định tên file!\n"
                             "💡 Ví dụ: learn data/knowledge.txt\n"
                             "💡 Hoặc: học từ data/knowledge.txt");
        }
    }
    // Học từ đoạn văn bản: "học: ..." hoặc "learn: ..."
    else if (strstr(lower, ":") != NULL) {
        char *pos = strchr(input, ':');
        if (pos) {
            pos++;
            while (*pos == ' ') pos++;
            int learned = learn_from_text(kb, pos, "user_input", dict);
            if (learned > 0) {
                snprintf(response, sizeof(response),
                         "✅ Đã học %d câu từ đoạn văn bản!\n"
                         "📚 Tổng số kiến thức hiện tại: %d câu",
                         learned, kb->count);
            } else {
                strcpy(response, "❌ Không tìm thấy nội dung để học!");
            }
        }
    }
    // Xem summary
    else if (strstr(lower, "tổng quan") != NULL || strstr(lower, "summary") != NULL) {
        strcpy(response, get_knowledge_summary(kb));
    }
    // Tìm kiếm
    else {
        strcpy(response, search_knowledge(kb, input));
    }
    
    return response;
}
