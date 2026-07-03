#!/bin/bash

cat > full_code.txt << 'FULL_CODE'
================================================================================
📁 FILE: src/context.h
================================================================================
#ifndef CONTEXT_H
#define CONTEXT_H

#define MAX_CONTEXT 10
#define MAX_TEXT_LEN 256

typedef struct {
    char user[MAX_TEXT_LEN];
    char ai[MAX_TEXT_LEN];
} DialogueTurn;

typedef struct {
    DialogueTurn turns[MAX_CONTEXT];
    int count;
    int current;
} ConversationContext;

void context_init(ConversationContext *ctx);
void context_add(ConversationContext *ctx, const char *user, const char *ai);
char* context_last_user(ConversationContext *ctx);
char* context_last_ai(ConversationContext *ctx);
void context_get_summary(ConversationContext *ctx, char *buffer, int size);
void context_get_last_n(ConversationContext *ctx, int n, char *buffer, int size);
void context_reset(ConversationContext *ctx);
void context_print(ConversationContext *ctx);

#endif

================================================================================
📁 FILE: src/context.c
================================================================================
#include "context.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void context_init(ConversationContext *ctx) {
    ctx->count = 0;
    ctx->current = 0;
    for (int i = 0; i < MAX_CONTEXT; i++) {
        ctx->turns[i].user[0] = '\0';
        ctx->turns[i].ai[0] = '\0';
    }
}

void context_add(ConversationContext *ctx, const char *user, const char *ai) {
    if (ctx->count < MAX_CONTEXT) {
        strncpy(ctx->turns[ctx->count].user, user, MAX_TEXT_LEN - 1);
        strncpy(ctx->turns[ctx->count].ai, ai, MAX_TEXT_LEN - 1);
        ctx->turns[ctx->count].user[MAX_TEXT_LEN - 1] = '\0';
        ctx->turns[ctx->count].ai[MAX_TEXT_LEN - 1] = '\0';
        ctx->count++;
        ctx->current = ctx->count - 1;
    } else {
        int idx = ctx->current % MAX_CONTEXT;
        strncpy(ctx->turns[idx].user, user, MAX_TEXT_LEN - 1);
        strncpy(ctx->turns[idx].ai, ai, MAX_TEXT_LEN - 1);
        ctx->turns[idx].user[MAX_TEXT_LEN - 1] = '\0';
        ctx->turns[idx].ai[MAX_TEXT_LEN - 1] = '\0';
        ctx->current++;
    }
}

char* context_last_user(ConversationContext *ctx) {
    if (ctx->count == 0) return NULL;
    int idx = (ctx->count < MAX_CONTEXT) ? ctx->count - 1 : (ctx->current - 1) % MAX_CONTEXT;
    return ctx->turns[idx].user;
}

char* context_last_ai(ConversationContext *ctx) {
    if (ctx->count == 0) return NULL;
    int idx = (ctx->count < MAX_CONTEXT) ? ctx->count - 1 : (ctx->current - 1) % MAX_CONTEXT;
    return ctx->turns[idx].ai;
}

void context_get_summary(ConversationContext *ctx, char *buffer, int size) {
    buffer[0] = '\0';
    int start = 0;
    int count = ctx->count;
    if (count > MAX_CONTEXT) {
        start = ctx->current % MAX_CONTEXT;
        count = MAX_CONTEXT;
    }
    for (int i = 0; i < count; i++) {
        int idx = (start + i) % MAX_CONTEXT;
        if (ctx->turns[idx].user[0] != '\0') {
            char line[512];
            snprintf(line, sizeof(line), "Bạn: %s\nCAI: %s\n", 
                     ctx->turns[idx].user, ctx->turns[idx].ai);
            strncat(buffer, line, size - strlen(buffer) - 1);
        }
    }
}

void context_get_last_n(ConversationContext *ctx, int n, char *buffer, int size) {
    buffer[0] = '\0';
    if (n > ctx->count) n = ctx->count;
    if (n <= 0) return;
    int start = ctx->count - n;
    if (start < 0) start = 0;
    if (ctx->count >= MAX_CONTEXT) {
        start = ctx->current - n;
        if (start < 0) start += MAX_CONTEXT;
    }
    for (int i = 0; i < n; i++) {
        int idx = (start + i) % MAX_CONTEXT;
        if (ctx->turns[idx].user[0] != '\0') {
            char line[512];
            snprintf(line, sizeof(line), "Bạn: %s\nCAI: %s\n", 
                     ctx->turns[idx].user, ctx->turns[idx].ai);
            strncat(buffer, line, size - strlen(buffer) - 1);
        }
    }
}

void context_reset(ConversationContext *ctx) {
    context_init(ctx);
}

void context_print(ConversationContext *ctx) {
    printf("\n=== NGỮ CẢNH (%d lượt) ===\n", ctx->count);
    char summary[2000];
    context_get_summary(ctx, summary, sizeof(summary));
    printf("%s", summary);
    printf("==========================\n\n");
}

================================================================================
📁 FILE: src/brain.h
================================================================================
#ifndef BRAIN_H
#define BRAIN_H

#include "markov.h"
#include "context.h"

typedef struct {
    Dictionary dict;
    char name[50];
    int conversation_count;
    ConversationContext context;
} Brain;

void brain_init(Brain *b, const char *name);
void brain_free(Brain *b);
void brain_train(Brain *b, const char *data_file);
void brain_learn(Brain *b, const char *sentence);
char* brain_chat(Brain *b, const char *input);
void brain_stats(Brain *b);
void brain_get_context(Brain *b, char *buffer, int size);

#endif

================================================================================
📁 FILE: src/brain.c
================================================================================
#include "brain.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void brain_init(Brain *b, const char *name) {
    markov_init(&b->dict);
    strcpy(b->name, name);
    b->conversation_count = 0;
    context_init(&b->context);
    srand(time(NULL));
}

void brain_free(Brain *b) {
    markov_free(&b->dict);
}

void brain_train(Brain *b, const char *data_file) {
    markov_load_file(&b->dict, data_file);
    if (b->dict.count == 0) {
        printf("📚 Sử dụng kiến thức mặc định...\n");
        const char *default_data[] = {
            "xin chào tôi là cai một trí tuệ nhân tạo",
            "tôi tên là cai và tôi được viết bằng c",
            "học máy rất thú vị và mạnh mẽ",
            "lập trình c rất nhanh và hiệu quả",
            "tôi thích học những điều mới mẻ",
            "lập trình là một nghệ thuật",
            "tôi là một ai ngốc nhưng tôi chạy rất nhanh"
        };
        for (int i = 0; i < 7; i++) {
            markov_learn(&b->dict, default_data[i]);
        }
        printf("✅ Đã tải %d kiến thức mặc định\n", 7);
    }
}

void brain_learn(Brain *b, const char *sentence) {
    if (strlen(sentence) > 5) {
        markov_learn(&b->dict, sentence);
    }
}

char* brain_chat(Brain *b, const char *input) {
    b->conversation_count++;
    char first_word[MAX_WORD_LEN];
    char temp[1000];
    strcpy(temp, input);
    char *token = strtok(temp, " .,!?;:\n\t\"'()[]{}");
    if (!token) {
        return "Xin lỗi, tôi không hiểu!";
    }
    strcpy(first_word, token);
    char *response = markov_generate(&b->dict, first_word);
    context_add(&b->context, input, response);
    return response;
}

void brain_stats(Brain *b) {
    printf("\n╔══════════════════════════════════╗\n");
    printf("║        🧠  THỐNG KÊ CAI          ║\n");
    printf("╠══════════════════════════════════╣\n");
    printf("║ Tên:      %-20s ║\n", b->name);
    printf("║ Từ vựng:  %-20d ║\n", b->dict.count);
    printf("║ Số chat:  %-20d ║\n", b->conversation_count);
    printf("║ Ngữ cảnh: %-20d ║\n", b->context.count);
    printf("╚══════════════════════════════════╝\n");
}

void brain_get_context(Brain *b, char *buffer, int size) {
    context_get_summary(&b->context, buffer, size);
}

================================================================================
📁 FILE: src/markov.h
================================================================================
#ifndef MARKOV_H
#define MARKOV_H

#define MAX_WORDS 1000
#define MAX_WORD_LEN 50
#define MAX_RESPONSE 100

typedef struct MarkovNode {
    char word[MAX_WORD_LEN];
    struct MarkovNode *next[MAX_WORDS];
    int next_count[MAX_WORDS];
    int total_next;
} MarkovNode;

typedef struct {
    MarkovNode *nodes[MAX_WORDS];
    int count;
} Dictionary;

void markov_init(Dictionary *dict);
void markov_free(Dictionary *dict);
void markov_learn(Dictionary *dict, const char *sentence);
char* markov_generate(Dictionary *dict, const char *start_word);
void markov_load_file(Dictionary *dict, const char *filename);
int markov_vocab_size(Dictionary *dict);

#endif

================================================================================
📁 FILE: src/markov.c
================================================================================
#include "markov.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Hàm kiểm tra ký tự tiếng Việt có dấu
int is_vietnamese_char(unsigned char c) {
    return c >= 0x80 && c <= 0xFF;
}

// Hàm tách từ tiếng Việt (có dấu)
void tokenize_vietnamese(const char *text, char words[][MAX_WORD_LEN], int *count) {
    *count = 0;
    char temp[1000];
    strcpy(temp, text);
    
    char *token = strtok(temp, " .,!?;:\n\t\"'()[]{}");
    while (token != NULL && *count < MAX_WORDS) {
        strcpy(words[*count], token);
        (*count)++;
        token = strtok(NULL, " .,!?;:\n\t\"'()[]{}");
    }
}

void markov_init(Dictionary *dict) {
    dict->count = 0;
    for (int i = 0; i < MAX_WORDS; i++) dict->nodes[i] = NULL;
}

void markov_free(Dictionary *dict) {
    for (int i = 0; i < dict->count; i++) free(dict->nodes[i]);
    dict->count = 0;
}

static MarkovNode* find_or_add(Dictionary *dict, const char *word) {
    for (int i = 0; i < dict->count; i++) {
        if (strcmp(dict->nodes[i]->word, word) == 0) return dict->nodes[i];
    }
    MarkovNode *new_node = (MarkovNode*)malloc(sizeof(MarkovNode));
    if (!new_node) return NULL;
    strcpy(new_node->word, word);
    new_node->total_next = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        new_node->next[i] = NULL;
        new_node->next_count[i] = 0;
    }
    dict->nodes[dict->count] = new_node;
    dict->count++;
    return new_node;
}

static void add_transition(Dictionary *dict, const char *from, const char *to) {
    MarkovNode *node = find_or_add(dict, from);
    if (!node) return;
    for (int i = 0; i < node->total_next; i++) {
        if (strcmp(node->next[i]->word, to) == 0) {
            node->next_count[i]++;
            return;
        }
    }
    MarkovNode *to_node = find_or_add(dict, to);
    if (to_node) {
        node->next[node->total_next] = to_node;
        node->next_count[node->total_next] = 1;
        node->total_next++;
    }
}

void markov_learn(Dictionary *dict, const char *sentence) {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    tokenize_vietnamese(sentence, words, &word_count);
    for (int i = 0; i < word_count - 1; i++) {
        add_transition(dict, words[i], words[i + 1]);
    }
}

char* markov_generate(Dictionary *dict, const char *start_word) {
    static char response[MAX_RESPONSE * 10];
    response[0] = '\0';
    MarkovNode *current = find_or_add(dict, start_word);
    if (strcmp(current->word, start_word) != 0) {
        snprintf(response, sizeof(response), "Tôi không biết về '%s'. Hãy hỏi điều khác!", start_word);
        return response;
    }
    strcat(response, current->word);
    int generated = 1;
    while (generated < MAX_RESPONSE && current->total_next > 0) {
        int total = 0;
        for (int i = 0; i < current->total_next; i++) total += current->next_count[i];
        int rand_val = rand() % total;
        int cumulative = 0;
        MarkovNode *next_node = NULL;
        for (int i = 0; i < current->total_next; i++) {
            cumulative += current->next_count[i];
            if (rand_val < cumulative) { next_node = current->next[i]; break; }
        }
        if (!next_node) break;
        strcat(response, " ");
        strcat(response, next_node->word);
        current = next_node;
        generated++;
        if (rand() % 8 == 0 && generated > 5) break;
    }
    int len = strlen(response);
    if (len > 0 && response[len-1] != '.' && response[len-1] != '!' && response[len-1] != '?') {
        strcat(response, ".");
    }
    return response;
}

void markov_load_file(Dictionary *dict, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("⚠️  Không thể mở file '%s'\n", filename);
        return;
    }
    char line[1000];
    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        if (line[0] != '\n' && line[0] != '#') {
            markov_learn(dict, line);
            count++;
        }
    }
    fclose(file);
    printf("📚 Đã tải %d câu từ %s\n", count, filename);
}

int markov_vocab_size(Dictionary *dict) {
    return dict->count;
}

================================================================================
📁 FILE: src/utils.h
================================================================================
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

void trim(char *str);
void to_lower(char *str);
int starts_with(const char *str, const char *prefix);
void clear_screen();

#endif

================================================================================
📁 FILE: src/utils.c
================================================================================
#include "utils.h"

void trim(char *str) {
    int i = 0;
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') i++;
    int j = 0;
    while (str[i]) str[j++] = str[i++];
    str[j] = '\0';
    i = strlen(str) - 1;
    while (i >= 0 && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')) {
        str[i--] = '\0';
    }
}

void to_lower(char *str) {
    for (int i = 0; str[i]; i++) str[i] = tolower(str[i]);
}

int starts_with(const char *str, const char *prefix) {
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        printf("\033[2J\033[H");
    #endif
}

================================================================================
📁 FILE: src/main.c
================================================================================
#include <stdio.h>
#include <string.h>
#include "brain.h"
#include "utils.h"

void print_banner() {
    printf("\n");
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║   🧠  CAI - Trí Tuệ Nhân Tạo              ║\n");
    printf("║   Thuần C - 100% Tiếng Việt               ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    printf("\n");
}

void print_help() {
    printf("Lệnh:\n");
    printf("  /help      Hiển thị trợ giúp\n");
    printf("  /stats     Xem thống kê\n");
    printf("  /context   Xem ngữ cảnh hội thoại\n");
    printf("  /clear     Xóa màn hình\n");
    printf("  /reset     Xóa ngữ cảnh\n");
    printf("  /exit      Thoát\n");
    printf("\n");
}

int main() {
    print_banner();
    Brain brain;
    brain_init(&brain, "CAI");
    printf("📚 Đang huấn luyện...\n");
    brain_train(&brain, "data/knowledge.txt");
    printf("✅ Sẵn sàng! Gõ /help để xem lệnh.\n\n");
    char input[1000];
    char context_buf[2000];
    while (1) {
        printf("Bạn: ");
        fgets(input, sizeof(input), stdin);
        trim(input);
        if (strcmp(input, "/exit") == 0 || strcmp(input, "/quit") == 0) {
            printf("🤖 CAI: Tạm biệt! Hẹn gặp lại.\n");
            break;
        }
        else if (strcmp(input, "/help") == 0) {
            print_help();
            continue;
        }
        else if (strcmp(input, "/stats") == 0) {
            brain_stats(&brain);
            continue;
        }
        else if (strcmp(input, "/clear") == 0) {
            clear_screen();
            print_banner();
            continue;
        }
        else if (strcmp(input, "/context") == 0) {
            brain_get_context(&brain, context_buf, sizeof(context_buf));
            printf("📝 Ngữ cảnh hội thoại:\n%s\n", context_buf);
            continue;
        }
        else if (strcmp(input, "/reset") == 0) {
            context_reset(&brain.context);
            printf("✅ Đã xóa ngữ cảnh!\n");
            continue;
        }
        if (strlen(input) == 0) continue;
        brain_learn(&brain, input);
        char *response = brain_chat(&brain, input);
        printf("🤖 CAI: %s\n\n", response);
    }
    brain_free(&brain);
    return 0;
}

================================================================================
📁 FILE: data/knowledge.txt
================================================================================
# Tri thức cơ bản cho CAI - Bản tiếng Việt

xin chào tôi là cai một trí tuệ nhân tạo
tôi tên là cai và tôi được viết bằng ngôn ngữ c
học máy rất thú vị và mạnh mẽ
học sâu sử dụng mạng nơ ron
lập trình c rất nhanh và hiệu quả
python rất tốt cho khoa học dữ liệu
trí tuệ nhân tạo sẽ thay đổi thế giới
tôi thích học những điều mới mẻ
lập trình là một nghệ thuật
mạng nơ ron được lấy cảm hứng từ bộ não con người
tôi là một ai ngốc nhưng tôi chạy rất nhanh
tôi không hiểu nhiều nhưng tôi cố gắng hết sức
trí tuệ nhân tạo viết bằng c rất mạnh mẽ
tôi có thể học từ cuộc trò chuyện
hãy hỏi tôi bất cứ điều gì
tôi yêu việt nam
lập trình viên rất sáng tạo
công nghệ thông tin là tương lai
học lập trình không bao giờ là muộn

================================================================================
📁 FILE: Makefile
================================================================================
CC = gcc
CFLAGS = -Wall -Wextra -O2 -I./src
LDFLAGS = -lm
TARGET = bin/cai
SRCS = src/main.c src/brain.c src/markov.c src/utils.c src/context.c
OBJS = $(SRCS:src/%.c=build/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p bin
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo "✅ Build complete! Run ./$(TARGET)"

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build bin
	@echo "🧹 Cleaned"

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

================================================================================
📁 FILE: .gitignore
================================================================================
build/
bin/
*.o
*.exe
*.out
.vscode/
.idea/
*.swp
.DS_Store
Thumbs.db
*.log
data/logs/

================================================================================
📁 FILE: README.md
================================================================================

