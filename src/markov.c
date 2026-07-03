#include "markov.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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
    char temp[1000];
    strcpy(temp, sentence);
    char *token = strtok(temp, " .,!?;:\n\t");
    while (token != NULL && word_count < MAX_WORDS) {
        for (int i = 0; token[i]; i++) token[i] = tolower(token[i]);
        strcpy(words[word_count], token);
        word_count++;
        token = strtok(NULL, " .,!?;:\n\t");
    }
    for (int i = 0; i < word_count - 1; i++) {
        add_transition(dict, words[i], words[i + 1]);
    }
}

char* markov_generate(Dictionary *dict, const char *start_word) {
    static char response[MAX_RESPONSE * 10];
    response[0] = '\0';
    char word_lower[MAX_WORD_LEN];
    strcpy(word_lower, start_word);
    for (int i = 0; word_lower[i]; i++) word_lower[i] = tolower(word_lower[i]);
    MarkovNode *current = find_or_add(dict, word_lower);
    if (strcmp(current->word, word_lower) != 0) {
        snprintf(response, sizeof(response), "I don't know about '%s'. Try something else!", start_word);
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
        printf("⚠️  Warning: Cannot open '%s'\n", filename);
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
    printf("📚 Loaded %d sentences from %s\n", count, filename);
}

int markov_vocab_size(Dictionary *dict) {
    return dict->count;
}
