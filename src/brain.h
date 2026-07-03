#ifndef BRAIN_H
#define BRAIN_H

#include "markov.h"
#include "context.h"
#include "intent.h"
#include "attribute.h"
#include "definition.h"
#include "physics.h"
#include "coder.h"
#include "knowledge.h"
#include "chemistry.h"
#include "vietnamese.h"

typedef struct {
    Dictionary dict;
    char name[50];
    int conversation_count;
    ConversationContext context;
    int enable_intent;
    int enable_attribute;
    int enable_physics;
    int enable_coder;
    int enable_knowledge;
    int enable_chemistry;
    int enable_vietnamese;
    int enable_bridge;  // THÊM: bật/tắt suy nghĩ
} Brain;

void brain_init(Brain *b, const char *name);
void brain_free(Brain *b);
void brain_train(Brain *b, const char *data_file);
void brain_learn(Brain *b, const char *sentence);
char* brain_chat(Brain *b, const char *input);
void brain_stats(Brain *b);
void brain_get_context(Brain *b, char *buffer, int size);
void brain_set_intent(Brain *b, int enable);
void brain_set_attribute(Brain *b, int enable);
void brain_set_physics(Brain *b, int enable);
void brain_set_coder(Brain *b, int enable);
void brain_set_knowledge(Brain *b, int enable);
void brain_set_chemistry(Brain *b, int enable);
void brain_set_vietnamese(Brain *b, int enable);
void brain_set_bridge(Brain *b, int enable);

#endif
