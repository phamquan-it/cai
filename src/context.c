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

void context_reset(ConversationContext *ctx) {
    context_init(ctx);
}
