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
