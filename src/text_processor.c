#include "text_processor.h"
#include "utils.h"  // THÊM: dùng trim từ utils
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

// ============================================
//  TÁCH CÂU TỪ VĂN BẢN
// ============================================

int split_sentences(const char *text, char sentences[][512], int max_sentences) {
    if (!text) return 0;
    
    char temp[4096];
    strcpy(temp, text);
    int count = 0;
    
    const char *delimiters = ".!?";
    char *start = temp;
    char *end = temp;
    
    while (*end && count < max_sentences) {
        while (*end && !strchr(delimiters, *end)) {
            end++;
        }
        if (*end) {
            int len = end - start + 1;
            if (len < 512) {
                strncpy(sentences[count], start, len);
                sentences[count][len] = '\0';
                trim(sentences[count]);  // Dùng từ utils.h
                count++;
            }
            end++;
            start = end;
        }
    }
    
    if (*start && count < max_sentences) {
        strcpy(sentences[count], start);
        trim(sentences[count]);  // Dùng từ utils.h
        count++;
    }
    
    return count;
}

// ============================================
//  PHÂN TÍCH 1 CÂU
// ============================================

static void analyze_sentence(Sentence *s) {
    s->is_question = 0;
    s->is_exclamation = 0;
    s->word_count = 0;
    s->punctuation[0] = '\0';
    
    int len = strlen(s->original);
    if (len > 0) {
        char last = s->original[len - 1];
        if (last == '.' || last == '?' || last == '!') {
            s->punctuation[0] = last;
            s->punctuation[1] = '\0';
            if (last == '?') s->is_question = 1;
            if (last == '!') s->is_exclamation = 1;
        }
    }
    
    char temp[512];
    strcpy(temp, s->original);
    int temp_len = strlen(temp);
    if (temp_len > 0 && strchr(".!?", temp[temp_len - 1])) {
        temp[temp_len - 1] = '\0';
    }
    
    char *token = strtok(temp, " .,;:\n\t\"'()[]{}");
    while (token != NULL && s->word_count < MAX_WORDS_PER_SENTENCE) {
        strcpy(s->words[s->word_count], token);
        s->word_count++;
        token = strtok(NULL, " .,;:\n\t\"'()[]{}");
    }
}

// ============================================
//  PARAGRAPH
// ============================================

Paragraph* parse_paragraph(const char *text) {
    if (!text) return NULL;
    
    Paragraph *p = (Paragraph*)malloc(sizeof(Paragraph));
    if (!p) return NULL;
    
    memset(p, 0, sizeof(Paragraph));
    strcpy(p->raw_text, text);
    
    char sentences[MAX_SENTENCES][512];
    p->sentence_count = split_sentences(text, sentences, MAX_SENTENCES);
    
    for (int i = 0; i < p->sentence_count; i++) {
        strcpy(p->sentences[i].original, sentences[i]);
        analyze_sentence(&p->sentences[i]);
    }
    
    return p;
}

void free_paragraph(Paragraph *p) {
    if (p) free(p);
}

// ============================================
//  TRÍCH XUẤT THÔNG TIN
// ============================================

ExtractedInfo* extract_info(const char *text) {
    if (!text) return NULL;
    
    ExtractedInfo *info = (ExtractedInfo*)malloc(sizeof(ExtractedInfo));
    if (!info) return NULL;
    
    memset(info, 0, sizeof(ExtractedInfo));
    
    regex_t regex_num;
    regcomp(&regex_num, "[0-9]+(\\.[0-9]+)?", REG_EXTENDED);
    regmatch_t matches[1];
    const char *ptr = text;
    info->number_count = 0;
    
    while (regexec(&regex_num, ptr, 1, matches, 0) == 0 && info->number_count < 20) {
        int start = matches[0].rm_so;
        int end = matches[0].rm_eo;
        int len = end - start;
        if (len < 32) {
            strncpy(info->numbers[info->number_count], ptr + start, len);
            info->numbers[info->number_count][len] = '\0';
            info->number_count++;
        }
        ptr += end;
    }
    regfree(&regex_num);
    
    char temp[2048];
    strcpy(temp, text);
    char *token = strtok(temp, " .,!?;:\n\t");
    info->name_count = 0;
    while (token != NULL && info->name_count < 20) {
        if (strlen(token) > 1 && isupper(token[0])) {
            strcpy(info->names[info->name_count], token);
            info->name_count++;
        }
        token = strtok(NULL, " .,!?;:\n\t");
    }
    
    const char *stopwords[] = {"và", "của", "là", "trong", "với", "cho", "khi", "nếu",
                               "để", "mà", "này", "đó", "ở", "từ", "còn", "thì", "sẽ"};
    char temp2[2048];
    strcpy(temp2, text);
    token = strtok(temp2, " .,!?;:\n\t");
    info->keyword_count = 0;
    while (token != NULL && info->keyword_count < 50) {
        char lower[64];
        strcpy(lower, token);
        for (int i = 0; lower[i]; i++) lower[i] = tolower(lower[i]);
        
        int is_stopword = 0;
        for (int i = 0; i < 17; i++) {
            if (strcmp(lower, stopwords[i]) == 0) {
                is_stopword = 1;
                break;
            }
        }
        
        if (!is_stopword && strlen(token) > 2) {
            strcpy(info->keywords[info->keyword_count], lower);
            info->keyword_count++;
        }
        token = strtok(NULL, " .,!?;:\n\t");
    }
    
    return info;
}

void free_extracted_info(ExtractedInfo *info) {
    if (info) free(info);
}

// ============================================
//  PHÁT HIỆN YÊU CẦU
// ============================================

int is_text_processing_request(const char *input) {
    if (!input) return 0;
    
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    const char *keywords[] = {
        "đọc câu", "trích xuất", "phân tích", "tách câu",
        "thông tin", "trích", "xuất"
    };
    
    for (int i = 0; i < 7; i++) {
        if (strstr(lower, keywords[i]) != NULL) {
            return 1;
        }
    }
    return 0;
}

char* answer_text_processing_request(const char *input) {
    static char response[4096];
    response[0] = '\0';
    
    if (!is_text_processing_request(input)) {
        return NULL;
    }
    
    char text_to_process[2048];
    text_to_process[0] = '\0';
    
    const char *patterns[] = {"câu", "văn bản", "đoạn"};
    char lower[512];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) lower[i] = tolower(lower[i]);
    
    for (int i = 0; i < 3; i++) {
        char *pos = strstr(lower, patterns[i]);
        if (pos) {
            pos += strlen(patterns[i]);
            while (*pos == ' ') pos++;
            strcpy(text_to_process, pos);
            break;
        }
    }
    
    if (strlen(text_to_process) == 0) {
        strcpy(text_to_process, input);
    }
    
    Paragraph *p = parse_paragraph(text_to_process);
    if (!p) {
        return "❌ Không thể phân tích văn bản!";
    }
    
    ExtractedInfo *info = extract_info(text_to_process);
    
    strcat(response, "📝 PHÂN TÍCH VĂN BẢN:\n\n");
    
    char line[128];
    snprintf(line, sizeof(line), "📊 Số câu: %d\n", p->sentence_count);
    strcat(response, line);
    
    strcat(response, "\n📋 DANH SÁCH CÂU:\n");
    for (int i = 0; i < p->sentence_count && i < 10; i++) {
        char sent[256];
        snprintf(sent, sizeof(sent), "   %d. %s\n", i+1, p->sentences[i].original);
        strcat(response, sent);
    }
    
    strcat(response, "\n🔍 THÔNG TIN TRÍCH XUẤT:\n");
    
    if (info->number_count > 0) {
        strcat(response, "   🔢 Số:\n");
        for (int i = 0; i < info->number_count; i++) {
            char num[64];
            snprintf(num, sizeof(num), "      - %s\n", info->numbers[i]);
            strcat(response, num);
        }
    }
    
    if (info->name_count > 0) {
        strcat(response, "   👤 Tên riêng:\n");
        for (int i = 0; i < info->name_count; i++) {
            char name[64];
            snprintf(name, sizeof(name), "      - %s\n", info->names[i]);
            strcat(response, name);
        }
    }
    
    if (info->keyword_count > 0) {
        strcat(response, "   🔑 Từ khóa:\n");
        for (int i = 0; i < info->keyword_count && i < 10; i++) {
            char kw[64];
            snprintf(kw, sizeof(kw), "      - %s\n", info->keywords[i]);
            strcat(response, kw);
        }
    }
    
    free_paragraph(p);
    free_extracted_info(info);
    
    return response;
}
