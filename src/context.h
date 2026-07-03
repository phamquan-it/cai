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
void context_reset(ConversationContext *ctx);

#endif
