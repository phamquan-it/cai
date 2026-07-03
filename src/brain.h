#ifndef BRAIN_H
#define BRAIN_H

#include "markov.h"

typedef struct {
    Dictionary dict;
    char name[50];
    int conversation_count;
} Brain;

void brain_init(Brain *b, const char *name);
void brain_free(Brain *b);
void brain_train(Brain *b, const char *data_file);
void brain_learn(Brain *b, const char *sentence);
char* brain_chat(Brain *b, const char *input);
void brain_stats(Brain *b);

#endif
